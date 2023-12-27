#ifndef QFN_H
#define QFN_H

/**
* @description
* This header file must be included in all modules that use QP-nano.
* Typically, this header file is included indirectly through the
* header file qpn_port.h.
*/

/****************************************************************************/
#ifndef QF_TIMEEVT_CTR_SIZE
    /*! macro to override the default QTimeEvtCtr size.
    * Valid values 0U, 1U, 2U, or 4U; default 0U
    */
    #define QF_TIMEEVT_CTR_SIZE 0U
#endif
#if (QF_TIMEEVT_CTR_SIZE == 0U)
    /* no time events */
#elif (QF_TIMEEVT_CTR_SIZE == 1U)
    typedef uint8_t QTimeEvtCtr;
#elif (QF_TIMEEVT_CTR_SIZE == 2U)
    /*! type of the Time Event counter, which determines the dynamic
    * range of the time delays measured in clock ticks.
    */
    /**
    * @description
    * This typedef is configurable via the preprocessor switch
    * #QF_TIMEEVT_CTR_SIZE. The other possible values of this type are
    * as follows: @n
    * none when (QF_TIMEEVT_CTR_SIZE not defined or == 0U), @n
    * uint8_t  when (QF_TIMEEVT_CTR_SIZE == 1U); @n
    * uint16_t when (QF_TIMEEVT_CTR_SIZE == 2U); and @n
    * uint32_t when (QF_TIMEEVT_CTR_SIZE == 4U).
    */
    typedef uint16_t QTimeEvtCtr;
#elif (QF_TIMEEVT_CTR_SIZE == 4U)
    typedef uint32_t QTimeEvtCtr;
#else
    #error "QF_TIMER_SIZE defined incorrectly, expected 1U, 2U, or 4U"
#endif

#if (QF_TIMEEVT_CTR_SIZE != 0U)
    /*! Timer structure the active objects */
    typedef struct {
        QTimeEvtCtr nTicks;   /*!< timer tick counter */
#ifdef QF_TIMEEVT_PERIODIC
        QTimeEvtCtr interval; /*!< timer interval */
#endif /* QF_TIMEEVT_PERIODIC */
    } QTimer;
#endif /* (QF_TIMEEVT_CTR_SIZE != 0U) */

#ifndef QF_MAX_TICK_RATE
    /*! Default value of the macro configurable value in qpn_port.h */
    #define QF_MAX_TICK_RATE     1U
#elif (QF_MAX_TICK_RATE > 4U)
    #error "QF_MAX_TICK_RATE exceeds the 4U limit"
#endif

/**
******************************************************************************
                            tick_rate_0 ~ tick_rate_1
    Do like this: QActive_armX(AO_Blinky,
                    QF_TICK_RATE_3,
                    BLINKY_TICKS_PER_SEC,
                    BLINKY_TICKS_PER_SEC);
******************************************************************************
**/
#define QF_TICK_RATE_0 ((uint_fast8_t)0U)
#define QF_TICK_RATE_1 ((uint_fast8_t)1U)
#define QF_TICK_RATE_2 ((uint_fast8_t)2U)
#define QF_TICK_RATE_3 ((uint_fast8_t)3U)


/****************************************************************************/
/*! QActive active object (based on QHsm-implementation)
* @extends QHsm
*/
/**
* @description
* QActive is the base structure for derivation of active objects. Active
* objects in QF-nano are encapsulated tasks (each embedding a state machine
* and an event queue) that communicate with one another asynchronously by
* sending and receiving events. Within an active object, events are
* processed sequentially in a run-to-completion (RTC) fashion, while QF
* encapsulates all the details of thread-safe event exchange and queuing.
*
* @note ::QActive is not intended to be instantiated directly, but rather
* serves as the base structure for derivation of active objects in the
* application code.
*
* @usage
* The following example illustrates how to derive an active object from
* ::QActive. Please note that the ::QActive member super_ is defined as
* the __first__ member of the derived struct.
* @include qfn_qactive.c
*/
typedef struct QActive {
    QHsm super; /**< derives from the ::QHsm base class */

#if (QF_TIMEEVT_CTR_SIZE != 0U)
    /*! Timer for the active object */
    QTimer tickCtr[QF_MAX_TICK_RATE];
#endif /* (QF_TIMEEVT_CTR_SIZE != 0U) */

    /*! priority of the active object (1..8) */
    uint8_t prio;

    /*! offset to where next event will be inserted into the buffer */
    uint8_t volatile head;

    /*! offset of where next event will be extracted from the buffer */
    uint8_t volatile tail;

    /*! number of events currently present in the queue
    * (events in the ring buffer + 1 event in the state machine)
    */
    uint8_t volatile nUsed;

} QActive;

/*! protected "constructor" of an QActive active object. */
void QActive_ctor(QActive * const me,
    QStateHandler initial);


/*! special value of margin that causes asserting failure in case
* event posting fails.
*/
#define QF_NO_MARGIN ((uint_fast8_t)0xFF)

/**
******************************************************************************
                        post QEvt *
******************************************************************************
**/
#define QACTIVE_POST(me_, \
    e_)  \
    QActive_postX_((me_),  \
        QF_NO_MARGIN,  \
        (e_));

#define QACTIVE_POST_ISR(me_, \
    e_)  \
    QActive_postXISR_((me_),  \
        QF_NO_MARGIN,  \
        (e_));

/**
******************************************************************************
                        Post with margin control
    Both of them used inside CRITICAL SECTION.
    If the post is not allowed, the event got from the system event pool will
be gc immediately.
******************************************************************************
**/
bool QActive_postX_(QActive * const me,
    uint_fast8_t margin,
    QEvt *e_);

bool QActive_postXISR_(QActive * const me,
    uint_fast8_t margin,
    QEvt *e_);


#if (QF_TIMEEVT_CTR_SIZE != 0U)

    /*! Processes all armed time events at every clock tick. */
    void QF_tickXISR(uint_fast8_t const tickRate);

#ifdef QF_TIMEEVT_PERIODIC
    /*! Arm the QP-nano one-shot time event.
    * @public @memberof QActive
    */
    void QActive_armX(QActive * const me,
        uint_fast8_t const tickRate,
        QTimeEvtCtr const nTicks,
        QTimeEvtCtr const interval);
#else
    /*! Arm the QP-nano one-shot time event.
    * @public @memberof QActive
    */
    void QActive_armX(QActive * const me,
        uint_fast8_t const tickRate,
        QTimeEvtCtr const nTicks);
#endif

    /*! Disarm a time event. Since the tick counter
    * @public @memberof QActive
    */
    void QActive_disarmX(QActive * const me,
        uint_fast8_t const tickRate);

#endif /* (QF_TIMEEVT_CTR_SIZE != 0U) */


/****************************************************************************/
/* QF-nano protected methods ...*/

/*! QF-nano initialization. */
void QF_init(uint_fast8_t maxActive);

/*! QF-nano termination. */
/**
* @description
* This function terminates QF and performs any necessary cleanup.
* In QF-nano this function is defined in the BSP. Many QF ports might not
* require implementing QF_stop() at all, because many embedded applications
* don't have anything to exit to.
*/
void QF_stop(void);

/*! Startup QF-nano callback. */
/**
* @description
* The time line for calling QF_onStartup() depends on the particular
* QF port. In most cases, QF_onStartup() is called from QF_run(), right
* before starting any multitasking kernel or the background loop.
*
* @sa QF initialization example for ::QActiveCB.
*/
void QF_onStartup(void);

/*! Transfers control to QF-nano to run the application. */
// int_t QF_run(void);
void QF_run(void);

/****************************************************************************/
/*! QActive Control Block
*
* QActiveCB represents the read-only information that the QF-nano needs to
* manage the active object. QActiveCB objects are grouped in the array
* QF_active[], which typically can be placed in ROM.
*
* @usage
* The following example illustrates how to allocate and initialize the
* ::QActive control blocks in the array QF_active[].
* @include qfn_main.c
*/
typedef struct {
    QActive  *act;   /*!< pointer to the active object structure */
    QEvt     **queue; /*!< pointer to the event queue buffer */
    uint8_t   qlen;  /*!< the length of the queue ring buffer */
} QActiveCB;

/** active object control blocks */
/*lint -save -e9067 MISRA-C:2012 Rule 8.11, extern array declared without size */
extern QActiveCB const Q_ROM QF_active[];
/*lint -restore */

/*! number of active objects in the application (# elements in QF_active[]) */
extern uint_fast8_t QF_maxActive_;

/*! Ready set of QF-nano. */
// extern uint_fast8_t volatile QF_readySet_;

#ifndef QF_LOG2

    /*! Lookup table for (log2(n) + 1), where n is the index into the table.
    * This lookup delivers the 1-based number of the most significant 1-bit
    * of a nibble.
    */
    extern uint8_t const Q_ROM QF_log2Lkup[16];

    /*! Lookup table for ~(1 << (n - 1)), where n is the index into the table. */
    extern uint8_t const Q_ROM QF_invPow2Lkup[9];

#endif


/****************************************************************************/
/*! This macro encapsulates accessing the active object queue at a
* given index, which violates MISRA-C 2004 rules 17.4(req) and 11.4(adv).
* This macro helps to localize this deviation.
*/
#define QF_ROM_QUEUE_AT_(ao_, i_) (((QEvt **)Q_ROM_PTR((ao_)->queue))[(i_)])

/*! This macro encapsulates accessing the active object control block,
* which violates MISRA-C 2004 rule 11.4(adv). This macro helps to localize
* this deviation.
*/
#define QF_ROM_ACTIVE_GET_(p_) ((QActive *)Q_ROM_PTR(QF_active[(p_)].act))

/*! This macro encapsulates the upcast to QActive*
*
* This macro encapsulates up-casting a pointer to a subclass of ::QActive
* to the base class ::QActive, which violates MISRA-C 2004 rule 11.4(adv).
* This macro helps to localize this deviation.
*/
#define QF_ACTIVE_CAST(a_)     ((QActive *)(a_))

#endif /* QFN_H */
