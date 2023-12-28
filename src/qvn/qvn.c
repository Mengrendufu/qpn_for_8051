/*==========================================================================*/
#include "qpn_conf.h" /* QP-nano configuration file (from the application) */
#include "qfn_port.h" /* QF-nano port from the port directory */
#include "qassert.h"  /* embedded systems-friendly assertions */

Q_DEFINE_THIS_MODULE("qvn")

/* protection against including this source file in a wrong project */
#ifndef QVN_H
    #error "Source file included in a project NOT based on the QV-nano kernel"
#endif /* QVN_H */

/****************************************************************************/
/**
* @description
* QF_run() is typically called from your startup code after you initialize
* the QF and start at least one active object with QActive_start().
* This implementation of QF_run() is for the cooperative Vanilla kernel.
*
* @returns QF_run() typically does not return in embedded applications.
* However, when QP runs on top of an operating system,  QF_run() might
* return and in this case the return represents the error code (0 for
* success). Typically the value returned from QF_run() is subsequently
* passed on as return from main().
*/
void QF_run(void) {
    static uint_fast8_t p;
    static QActive *a;
    static QActiveCB const *acb;

#ifdef QF_MAX_ACTIVE /* deprecated constant provided? */
#if (QF_MAX_ACTIVE < 1) || (32 < QF_MAX_ACTIVE)
    #error "QF_MAX_ACTIVE not defined or out of range. Valid range is 1..32"
#endif
    QF_maxActive_ = (uint_fast8_t)QF_MAX_ACTIVE;
#else
    /** @pre the number of active objects must be initialized by calling:
    * QF_init(Q_DIM(QF_active));
    */
    Q_REQUIRE_ID(100,
        (1U <= QF_maxActive_) && (QF_maxActive_ <= 32U));
#endif

    /* set priorities all registered active objects... */
    for (p = 1U; p <= QF_maxActive_; ++p) {
        a = QF_ROM_ACTIVE_GET_(p);

        /* QF_active[p] must be initialized */
        Q_ASSERT_ID(810,
            a != (QActive *)0);

        a->prio = (uint8_t)p; /* set the priority of the active object */
    }

    /* trigger initial transitions in all registered active objects... */
    for (p = 1U; p <= QF_maxActive_; ++p) {
        a = QF_ROM_ACTIVE_GET_(p);
        QHSM_INIT(&a->super); /* take the initial transition in the SM */
    }

    QF_onStartup(); /* invoke startup callback */

    /*
    **************************************************************************
            the event loop of the cooperative QV-nano kernel...
    **************************************************************************
    */
    QF_INT_DISABLE();
    for (;;) {
        /*
        **********************************************************************
                        super loop tasker (background)
        **********************************************************************
        */
        super_loop_tasks();

        /*
        **********************************************************************
                                event dispatch
        **********************************************************************
        */
        if (QF_readySet_ != 0U) {
            p = QF_LOG2(QF_readySet_);
            acb = &QF_active[p];
            a = QF_ROM_ACTIVE_GET_(p);

            /* some unsuded events must be available */
            Q_ASSERT_ID(820,
                a->nUsed > 0U);

            --a->nUsed;
            // Q_SIG(a) = QF_ROM_QUEUE_AT_(acb, a->tail).sig;
            Q_EVT(a) = QF_ROM_QUEUE_AT_(acb, a->tail);
            if (a->tail == 0U) { /* wrap around? */
                a->tail = Q_ROM_BYTE(acb->qlen);
            }
            --a->tail;
            QF_INT_ENABLE();

            QHSM_DISPATCH(&a->super); /* dispatch to the HSM (RTC step) */

            QF_INT_DISABLE();

            // gc dispatch event
            Q_GC_EVT(&g_sys_evt_pool,
                Q_EVT(a));

            /* empty queue? */
            if (a->nUsed == 0U) {
                /* clear the bit corresponding to 'p' */
                QF_readySet_ &= (~(((Q_ReadySet)(1U)) << (p - 1U)));
            }
        }
        else {
            /* QV_onIdle() must be called with interrupts DISABLED because
            * the determination of the idle condition (no events in the
            * queues) can change at any time by an interrupt posting events
            * to a queue. QV_onIdle() MUST enable interrupts internally,
            * perhaps at the same time as putting the CPU into a power-saving
            * mode.
            */
            QV_onIdle();

            QF_INT_DISABLE();
        }
    }

    return;
}
