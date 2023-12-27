/*==========================================================================*/
#include "qpn_conf.h" /* QP-nano configuration file (from the application) */
#include "qfn_port.h" /* QF-nano port from the port directory */
#include "qassert.h"  /* embedded systems-friendly assertions */

Q_DEFINE_THIS_MODULE("qepn")

/*
******************************************************************************
                                define
******************************************************************************
*/
QEvt *p_global_evt_dispatch_;
QEvt *p_global_evt_post_;

/****************************************************************************/
/*! empty signal for internal use only */
#define QEP_EMPTY_SIG_        ((QSignal)0)

/*! maximum depth of state nesting in HSMs (including the top level),
* must be >= 2
*/
#define QHSM_MAX_NEST_DEPTH_  ((int_fast8_t)4)

/**
******************************************************************************
                        Transition entry path
    QHsm_init_ and QHsm_dispatch_ use the same one path.
******************************************************************************
**/
static QStateHandler path[QHSM_MAX_NEST_DEPTH_];

/****************************************************************************/
/**
* @protected @memberof QHsm
* @description
* Performs the first step of HSM initialization by assigning the initial
* pseudostate to the currently active state of the state machine.
*
* @param[in,out] me      pointer (see @ref oop)
* @param[in]     initial pointer to the top-most initial state-handler
*                        function in the derived state machine
* @note
* Must be called only by the constructors of the derived state machines.
*
* @note
* Must be called only ONCE before QHSM_INIT().
*
* @usage
* The following example illustrates how to invoke QHsm_ctor() in the
* "constructor" of a derived state machine:
* @include qepn_qhsm_ctor.c
*/
void QHsm_ctor(QHsm * const me,
    QStateHandler initial)
{
    me->state = Q_STATE_CAST(&QHsm_top);
    me->temp  = initial;

    return;
}

/****************************************************************************/
/**
* @private @memberof QHsm
* @description
* Executes the top-most initial transition in a HSM.
*
* @param[in,out] me pointer (see @ref oop)
*
* @note
* Must be called only ONCE after the QHsm_ctor().
*/
void QHsm_init_(QHsm * const me) {
    static QStateHandler t;
    static QState r;
    static int_fast8_t ip; /* transition entry path index */

    /*----------------------------------------------------------------------*/
    static QEvt global_evt_init_;

    p_global_evt_dispatch_ = &global_evt_init_;
    t = me->state;
    ip = 0;

    /** @pre the virtual pointer must be initialized, the top-most initial
    * transition must be initialized, and the initial transition must not
    * be taken yet.
    */
    Q_REQUIRE_ID(200,
        (me->temp != Q_STATE_CAST(0)) && (t == Q_STATE_CAST(&QHsm_top)));

    r = (*me->temp)(me); /* execute the top-most initial transition */

    /* the top-most initial transition must be taken */
    Q_ASSERT_ID(210,
        r == Q_RET_TRAN);

    /* drill down into the state hierarchy with initial transitions... */
    do {
        path[0] = me->temp;
        Q_SIG(me) = QEP_EMPTY_SIG_;
        (void)(*me->temp)(me);
        while (me->temp != t) {
            ++ip;
            Q_ASSERT_ID(220,
                ip < (int_fast8_t)Q_DIM(path));
            path[ip] = me->temp;
            (void)(*me->temp)(me);
        }
        me->temp = path[0];

        /* retrace the entry path in reverse (desired) order... */
        Q_SIG(me) = Q_ENTRY_SIG;
        do {
            (void)(*path[ip])(me); /* enter path[ip] */
            --ip;
        } while (ip >= 0);

        t = path[0]; /* current state becomes the new source */

        Q_SIG(me) = Q_INIT_SIG;
        r = (*t)(me);
    } while (r == Q_RET_TRAN);

    me->state = t; /* change the current active state */
    me->temp  = t; /* mark the configuration as stable */

    return;
}

/****************************************************************************/
/**
* @protected @memberof QHsm
* @description
* QHsm_top() is the ultimate root of state hierarchy in all HSMs derived
* from ::QHsm.
*
* @param[in] me pointer (see @ref oop)
*
* @returns
* Always returns #Q_RET_IGNORED, which means that the top state ignores
* all events.
*
* @note
* The parameter @p me to this state handler is not used. It is provided for
* conformance with the state-handler function signature ::QStateHandler.
*/
QState QHsm_top(void const * const me) {
    (void)me; /* suppress the "unused parameter" compiler warning */
    return Q_RET_IGNORED; /* the top state ignores all events */
}

/****************************************************************************/
/**
* @private @memberof QHsm
* @description
* Dispatches an event for processing to a hierarchical state machine (HSM).
* The processing of an event represents one run-to-completion (RTC) step.
*
* @param[in,out] me pointer (see @ref oop)
*
* @note
* This function should be called only via the virtual table (see
* QHSM_DISPATCH()) and should NOT be called directly in the applications.
*/
void QHsm_dispatch_(QHsm * const me) {
    static QStateHandler t;
    static QStateHandler s;
    static QState r;
    static QSignal prev_sig;  // for unhandled situation
    static int_fast8_t ip;  /* transition entry path index */
    static int_fast8_t iq;  /* helper transition entry path index */

    t = me->state;

    /** @pre the current state must be initialized and
    * the state configuration must be stable
    */
    Q_REQUIRE_ID(400,
        (t != Q_STATE_CAST(0)) && (t == me->temp));

    /* process the event hierarchically... */
    do {
        s = me->temp;
        r = (*s)(me); /* invoke state handler s */

        if (r == Q_RET_UNHANDLED) { /* unhandled due to a guard? */
            prev_sig = Q_SIG(me); /* save the original signal */
            Q_SIG(me) = QEP_EMPTY_SIG_; /* find the superstate */
            r = (*s)(me); /* invoke state handler s */
            Q_SIG(me) = prev_sig; /* restore the original signal */
        }
    } while (r == Q_RET_SUPER);

    /* transition taken? */
    if (r >= Q_RET_TRAN) {
        path[0] = me->temp; /* save the target of the transition */
        path[1] = t;
        path[2] = s;

        /* exit current state to transition source s... */
        for (; t != s; t = me->temp) {
            Q_SIG(me) = Q_EXIT_SIG; /* find superstate of t */

            /* take the exit action and check if it was handled? */
            if ((*t)(me) == Q_RET_HANDLED) {
                Q_SIG(me) = QEP_EMPTY_SIG_;
                (void)(*t)(me); /* find superstate of t */
            }
        }

        /**
        **********************************************************************
                            take the state transition
        **********************************************************************
        **/
        ip = (int_fast8_t)(-1);
        t = path[0];
        s = path[2];

        /* (a) check source==target (transition to self) */
        if (s == t) {
            Q_SIG(me) = Q_EXIT_SIG;
            (void)(*s)(me);      /* exit the source */
            ip = 0; /* enter the target */
        }
        else {
            Q_SIG(me) = QEP_EMPTY_SIG_;
            (void)(*t)(me); /* find superstate of target */
            t = me->temp;

            /* (b) check source==target->super */
            if (s == t) {
                ip = 0; /* enter the target */
            }
            else {
                Q_SIG(me) = QEP_EMPTY_SIG_;
                (void)(*s)(me); /* find superstate of source */

                /* (c) check source->super==target->super */
                if (me->temp == t) {
                    Q_SIG(me) = Q_EXIT_SIG;
                    (void)(*s)(me);     /* exit the source */
                    ip = 0; /* enter the target */
                }
                else {
                    /* (d) check source->super==target */
                    if (me->temp == path[0]) {
                        Q_SIG(me) = Q_EXIT_SIG;
                        (void)(*s)(me); /* exit the source */
                    }
                    else {
                        /* (e) check rest of source==target->super->super..
                        * and store the entry path along the way
                        */
                        iq = 0; /* indicate that LCA not found */
                        ip = 1; /* enter target and its superstate */
                        path[1] = t; /* save the superstate of target */
                        t = me->temp; /* save source->super */

                        /* find target->super->super... */
                        Q_SIG(me) = QEP_EMPTY_SIG_;
                        r = (*path[1])(me);
                        while (r == Q_RET_SUPER) {
                            ++ip;
                            path[ip] = me->temp; /* store the entry path */
                            if (me->temp == s) { /* is it the source? */
                                iq = 1; /* indicate that LCA found */

                                /* entry path must not overflow */
                                Q_ASSERT_ID(510,
                                    ip < QHSM_MAX_NEST_DEPTH_);
                                --ip; /* do not enter the source */
                                r = Q_RET_HANDLED; /* terminate loop */
                            }
                            /* it is not the source, keep going up */
                            else {
                                r = (*me->temp)(me); /* superstate of t */
                            }
                        }

                        /* the LCA not found yet? */
                        if (iq == 0) {

                            /* entry path must not overflow */
                            Q_ASSERT_ID(520,
                                ip < QHSM_MAX_NEST_DEPTH_);

                            Q_SIG(me) = Q_EXIT_SIG;
                            (void)(*s)(me); /* exit the source */

                            /* (f) check the rest of source->super
                            *                  == target->super->super...
                            */
                            iq = ip;
                            r = Q_RET_IGNORED; /* LCA NOT found */
                            do {
                                if (t == path[iq]) { /* is this the LCA? */
                                    r = Q_RET_HANDLED; /* LCA found */

                                    /* do not enter LCA */
                                    ip = (int_fast8_t)(iq - 1);
                                    iq = -1; /* force loop termination */
                                }
                                else {
                                    --iq; /* try lower superstate of target */
                                }
                            } while (iq >= 0);

                            /* LCA not found? */
                            if (r != Q_RET_HANDLED) {
                                /* (g) check each source->super->...
                                * for each target->super...
                                */
                                r = Q_RET_IGNORED; /* keep looping */
                                do {
                                    /* exit t unhandled? */
                                    Q_SIG(me) = Q_EXIT_SIG;
                                    if ((*t)(me) == Q_RET_HANDLED) {
                                        Q_SIG(me) = QEP_EMPTY_SIG_;
                                        (void)(*t)(me); /* find super of t */
                                    }
                                    t = me->temp; /* set to super of t */
                                    iq = ip;
                                    do {
                                        /* is this LCA? */
                                        if (t == path[iq]) {
                                            /* do not enter LCA */
                                            ip = (int_fast8_t)(iq - 1);
                                            /* break out of inner loop */
                                            iq = -1;
                                            r = Q_RET_HANDLED; /* break */
                                        }
                                        else {
                                            --iq;
                                        }
                                    } while (iq >= 0);
                                } while (r != Q_RET_HANDLED);
                            }
                        }
                    }
                }
            }
        }

        /* retrace the entry path in reverse (desired) order... */
        Q_SIG(me) = Q_ENTRY_SIG;
        for (; ip >= 0; --ip) {
            (void)(*path[ip])(me); /* enter path[ip] */
        }
        t = path[0];      /* stick the target into register */
        me->temp = t; /* update the current state */

        /* drill into the target hierarchy... */
        Q_SIG(me) = Q_INIT_SIG;
        while ((*t)(me) == Q_RET_TRAN) {
            ip = 0;

            path[0] = me->temp;
            Q_SIG(me) = QEP_EMPTY_SIG_;
            (void)(*me->temp)(me); /* find the superstate */
            while (me->temp != t) {
                ++ip;
                path[ip] = me->temp;
                (void)(*me->temp)(me); /* find the superstate */
            }
            me->temp = path[0];

            /* entry path must not overflow */
            Q_ASSERT_ID(410,
                ip < QHSM_MAX_NEST_DEPTH_);

            /* retrace the entry path in reverse (correct) order... */
            Q_SIG(me) = Q_ENTRY_SIG;
            do {
                (void)(*path[ip])(me); /* enter path[ip] */
                --ip;
            } while (ip >= 0);

            t = path[0];
            Q_SIG(me) = Q_INIT_SIG;
        }
    }

    me->state = t; /* change the current active state */
    me->temp  = t; /* mark the configuration as stable */

    return;
}
