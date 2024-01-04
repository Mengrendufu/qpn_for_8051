/*==========================================================================*/
#include "qpn_conf.h" /* QP-nano configuration file (from the application) */
#include "qfn_port.h" /* QF-nano port from the port directory */
#include "qassert.h"  /* embedded systems-friendly assertions */

Q_DEFINE_THIS_MODULE("qfn")

/* Global-scope objects *****************************************************/

/**
* @description
* This variable stores the number of active objects in the application.
* This is the number of elements (dimension of) the QF_active[] array.
*/
uint_fast8_t QF_maxActive_;

/**
* @description
* The QF-nano ready set keeps track of active objects that are ready to run.
* The ready set represents each active object as a bit, with the bits
* assigned according to priorities of the active objects. The bit is set
* if the corresponding active object is ready to run (i.e., has one or
* more events in its event queue) and zero if the event queue is empty.
* The QF-nano ready set is one byte-wide, which corresponds to 8 active
* objects maximum.
*/
// use Q_ReadySet instead
// uint_fast8_t volatile QF_readySet_;

// #ifndef QF_LOG2
//     uint8_t const Q_ROM QF_log2Lkup[16] = {
//         0U, 1U, 2U, 2U, 3U, 3U, 3U, 3U,
//         4U, 4U, 4U, 4U, 4U, 4U, 4U, 4U
//     };
// #endif /* QF_LOG2 */

/****************************************************************************/
/**
* @protected @memberof QActive
*/
void QActive_ctor(QActive * const me,
    QStateHandler initial)
{
    /**
    * @note QActive inherits QActive, so by the @ref oop convention
    * it should call the constructor of the superclass, i.e., QActive_ctor().
    * However, this would pull in the QActiveVtable, which in turn will pull
    * in the code for QHsm_init_() and QHsm_dispatch_() implemetations,
    * which is expensive. To avoid this code size penalty, in case ::QHsm is
    * not used in a given project, the call to QHsm_ctor() avoids pulling
    * in the code for QHsm.
    */
    QHsm_ctor(&me->super,
        initial);

    return;
}

/****************************************************************************/
/**
* @private @memberof QActive
* @description
* Direct event posting is the simplest asynchronous communication method
* available in QF-nano.
*
* @attention
* This function should be called only via the macro QACTIVE_POST()
* or QACTIVE_POST_X(). This function should be only used in the
* __task__ context.
*
* @param[in,out] me     pointer (see @ref oop)
* @param[in]     margin number of required free slots in the queue after
*                       posting the event. The special value #QF_NO_MARGIN
*                       means that this function will assert if posting fails.
* @param[in]     sig    signal of the event to be posted
* @param[in]     par    parameter of the event to be posted
*
* @usage
* @include qfn_postx.c
*/
bool QActive_postX_(QActive * const me,
    uint_fast8_t margin,
    QEvt *e_)
{
    QActiveCB const *acb;
    uint_fast8_t qlen;
    bool can_post;

    acb = &QF_active[me->prio];
    qlen = acb->qlen;
    can_post = false;

    if (e_) {
        if (margin == QF_NO_MARGIN) {
            if (qlen > (uint_fast8_t)me->nUsed) {
                can_post = true; /* can post */
            }
            else {
                can_post = false; /* cannot post */
#ifndef Q_NASSERT
                QF_INT_ENABLE();
                /* must be able to post event : Q_ERROR_ID(310) */
                Q_onAssert(Q_this_module_,
                    310);
#endif
            }
        }
        else if ((qlen - (uint_fast8_t)me->nUsed) > margin) {
            can_post = true; /* can post */
        }
        else {
            can_post = false; /* cannot post */
        }

        if (can_post) { /* can post the event? */
            /* insert event into the ring buffer (FIFO) */
            //   QF_ROM_QUEUE_AT_(acb, me->head).sig = (QSignal)sig;
            QF_ROM_QUEUE_AT_(acb, me->head) = e_;
            if (me->head == 0U) {
                me->head = (uint8_t)qlen; /* wrap the head */
            }
            --me->head;
            ++me->nUsed;

            /* is this the first event? */
            if (me->nUsed == 1U) {

                /* set the corresponding bit in the ready set */
                QSet_insert(&QF_readySet_,
                    me->prio);
            }
        }
        else { /* can not post the event */
            // otherwise the event will no longer be used
            Q_GC_EVT(&g_sys_evt_pool,
                e_);
        }
    }

    return can_post;
}

/****************************************************************************/
/**
* @private @memberof QActive
* @description
* Direct event posting is the simplest asynchronous communication method
* available in QF-nano.
*
* @attention
* This function should be called only via the macro QACTIVE_POST_ISR()
* or QACTIVE_POST_X_ISR(). This function should be only used in the
* __ISR__ context.
*
* @param[in,out] me     pointer (see @ref oop)
* @param[in]     margin number of required free slots in the queue after
*                       posting the event. The special value #QF_NO_MARGIN
*                       means that this function will assert if posting fails.
* @param[in]     sig    signal of the event to be posted
* @param[in]     par    parameter of the event to be posted
*
* @usage
* @include qfn_postx.c
*/
bool QActive_postXISR_(QActive * const me,
    uint_fast8_t margin,
    QEvt *e_)
{
    QActiveCB const *acb;
    uint_fast8_t qlen;
    bool can_post;

    acb = &QF_active[me->prio];
    qlen = acb->qlen;
    can_post = false;

    if (e_) {
        if (margin == QF_NO_MARGIN) {
            if (qlen > (uint_fast8_t)me->nUsed) {
                can_post = true; /* can post */
            }
            else {
                can_post = false; /* cannot post */
#ifndef Q_NASSERT
                QF_INT_ENABLE();
                /* must be able to post event : Q_ERROR_ID(410) */
                Q_onAssert(Q_this_module_,
                    410);
#endif
            }
        }
        else if ((qlen - (uint_fast8_t)me->nUsed) > margin) {
            can_post = true; /* can post */
        }
        else {
            can_post = false; /* cannot post */
        }

        if (can_post) { /* can post the event? */
            /* insert event into the ring buffer (FIFO) */
            //   QF_ROM_QUEUE_AT_(acb, me->head).sig = (QSignal)sig;
            QF_ROM_QUEUE_AT_(acb, me->head) = e_;
            if (me->head == 0U) {
                me->head = (uint8_t)qlen; /* wrap the head */
            }
            --me->head;
            ++me->nUsed;
            /* is this the first event? */
            if (me->nUsed == 1U) {
                /* set the bit */
                QSet_insert(&QF_readySet_,
                    me->prio);
            }
        }
        else {
            // otherwise the event will no longer be used
            Q_GC_EVT(&g_sys_evt_pool,
                e_);
        }
    }

    return can_post;
}

/****************************************************************************/
/**
* @description
* The function QF_init() initializes the number of active objects to be
* managed by the framework and clears the internal QF-nano variables as well
* as all registered active objects to zero, which is needed in case when
* the startup code does not clear the uninitialized data (in violation of
* the C Standard).
*
* @note
* The intended use of the function is to call as follows:
* QF_init(Q_DIM(QF_active));
*/
void QF_init(uint_fast8_t maxActive) {
    QActive *a;
    uint_fast8_t p;
    uint_fast8_t n;

    /** @pre the number of active objects must be in range */
    Q_REQUIRE_ID(100,
        (1U < maxActive) && (maxActive <= 33U));
    QF_maxActive_ = (uint_fast8_t)maxActive - 1U;

    // QF_readySet_ = 0U;
    QSet_set_empty(&QF_readySet_);

    // init the sys event pool
    QMPool_init(&g_sys_evt_pool,
        (void *)g_sys_evt_pool_sto,
        sizeof(g_sys_evt_pool_sto),
        sizeof(g_sys_evt_pool_sto[0]));

    /* clear all registered active objects... */
    for (p = 1U; p <= QF_maxActive_; ++p) {
        a = QF_ROM_ACTIVE_GET_(p);

        /* QF_active[p] must be initialized */
        Q_ASSERT_ID(110,
            a != (QActive *)0);

        a->head    = 0U;
        a->tail    = 0U;
        a->nUsed   = 0U;
#if (QF_TIMEEVT_CTR_SIZE != 0U)
        for (n = 0U; n < (uint_fast8_t)QF_MAX_TICK_RATE; ++n) {
            a->tickCtr[n].nTicks   = 0U;
#ifdef QF_TIMEEVT_PERIODIC
            a->tickCtr[n].interval = 0U;
#endif /* def QF_TIMEEVT_PERIODIC */
        }
#endif /* (QF_TIMEEVT_CTR_SIZE != 0U) */
    }

    return;
}

/****************************************************************************/
/****************************************************************************/
#if (QF_TIMEEVT_CTR_SIZE != 0U)

/****************************************************************************/
/**
* @description
* This function must be called periodically from a time-tick ISR or from
* an ISR so that QF-nano can manage the timeout events assigned to the given
* system clock tick rate.
*
* @param[in]  tickRate  system clock tick rate serviced in this call.
*
* @note Each system tick rate posts timeout events with a different signal
* as follows:@n
* tickRate==0  Q_TIMEOUT_SIG@n
* tickRate==1  Q_TIMEOUT1_SIG@n
* tickRate==2  Q_TIMEOUT2_SIG@n
* tickRate==3  Q_TIMEOUT3_SIG
*
* @note The calls to QF_tickXISR() with different tick rate parameter can
* preempt each other. For example, higher clock tick rates might be serviced
* from interrupts that can preempt lower-priority interrupts.
*/
void QF_tickXISR(uint_fast8_t const tickRate) {
    uint_fast8_t p;
    QActive *a;
    QTimer *t;

    p = QF_maxActive_;
    do {
        a = QF_ROM_ACTIVE_GET_(p);
        t = &a->tickCtr[tickRate];
        if (t->nTicks != 0U) {
            --t->nTicks;
            if (t->nTicks == 0U) {

#ifdef QF_TIMEEVT_PERIODIC
                if (t->interval != 0U) {
                    t->nTicks = t->interval; /* re-arm the periodic timer */
                }
#endif /* QF_TIMEEVT_PERIODIC */

                p_global_evt_post_ = (QEvt *)0;
                p_global_evt_post_ = Q_NEW_EVT(&g_sys_evt_pool);
                if (p_global_evt_post_) {
                    p_global_evt_post_->sig =
                        (QSignal)Q_TIMEOUT_SIG + (QSignal)tickRate;
                }
                QACTIVE_POST_ISR(a,
                    p_global_evt_post_);
            }
        }
        --p;
    } while (p != 0U);

    return;
}

/****************************************************************************/
/**
* @public @memberof QActive
* @description
* Arms a time event to fire in a specified number of clock ticks at the
* specified tick rate. The timeout signal gets directly posted (using the
* FIFO policy) into the event queue of the active object calling this
* function.
*
* @param[in,out] me       pointer (see @ref oop)
* @param[in]     tickRate tick rate .
* @param[in]     nTicks   number of clock ticks (at the associated rate)
*                         to rearm the time event with.
*
* @note Each system tick rate posts timeout events with a different signal
* as follows:@n
* tickRate==0  Q_TIMEOUT_SIG@n
* tickRate==1  Q_TIMEOUT1_SIG@n
* tickRate==2  Q_TIMEOUT2_SIG@n
* tickRate==3  Q_TIMEOUT3_SIG
*
* @note After posting, a one-shot time event gets automatically disarmed.
*
* @note A time event can be disarmed at any time by calling the
* QActive_disarmX() function.
*
* @usage
* The following example shows how to arm a time event from a state
* machine of an active object:
* @include qfn_armx.c
*/
#ifdef QF_TIMEEVT_PERIODIC
void QActive_armX(QActive * const me,
    uint_fast8_t const tickRate,
    QTimeEvtCtr const nTicks,
    QTimeEvtCtr const interval)
#else
void QActive_armX(QActive * const me,
    uint_fast8_t const tickRate,
    QTimeEvtCtr const nTicks)
#endif
{
    QF_INT_DISABLE();
    me->tickCtr[tickRate].nTicks = nTicks;
#ifdef QF_TIMEEVT_PERIODIC
    me->tickCtr[tickRate].interval = interval;
#endif /* QF_TIMEEVT_PERIODIC */
    QF_INT_ENABLE();

    return;
}

/****************************************************************************/
/**
* @public @memberof QActive
* @description
* The time event of the active object gets disarmed (stopped).
*
* @param[in,out] me       pointer (see @ref oop)
* @param[in]     tickRate tick rate
*
* @note You should __not__ assume that the timeout event will not
* arrive after you disarm the time event. The timeout event could be
* already in the event queue.
*/
void QActive_disarmX(QActive * const me,
    uint_fast8_t const tickRate)
{
    QF_INT_DISABLE();
    me->tickCtr[tickRate].nTicks = 0U;
#ifdef QF_TIMEEVT_PERIODIC
    me->tickCtr[tickRate].interval = 0U;
#endif /* QF_TIMEEVT_PERIODIC */
    QF_INT_ENABLE();

    return;
}

#endif /* #if (QF_TIMEEVT_CTR_SIZE != 0U) */
