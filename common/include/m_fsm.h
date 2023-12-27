#ifndef m_fsm_h_
#define m_fsm_h_

/**
******************************************************************************
                    M_Fsm do the bsp jobs periodically
******************************************************************************
**/


/**
******************************************************************************
                            Used inside
******************************************************************************
**/
#define M_INT_DISABLE() QF_INT_DISABLE()
#define M_INT_ENABLE()  QF_INT_ENABLE()

/**
******************************************************************************
                            M_Fsm super class
******************************************************************************
**/
typedef unsigned char M_State; /* max 255 states per one */
typedef unsigned char M_Flag;
typedef unsigned int  M_Ticks;
typedef struct M_Fsm {
    M_State state;
    M_Flag  entry_is_ready;
    M_Flag  exit_is_ready;
    M_Flag  run_is_ready;
    M_Ticks run_ticks;
    M_Ticks ticks;
} M_Fsm;

/**
******************************************************************************
                            Constructor
******************************************************************************
**/
void M_Fsm_ctor(M_Fsm * const me,
    M_Ticks run_ticks,
    M_State initial_state);

/**
******************************************************************************
                            Internal handlers
******************************************************************************
**/
#define M_Fsm_run_is_ready(me_)  \
    ((me_)->run_is_ready)
#define M_Fsm_run_set_ready(me_)  \
    ((me_)->run_is_ready = 0xFF)
#define M_Fsm_runClear(me_)  \
    ((me_)->run_is_ready = 0x00)
#define M_Fsm_tick(me_,  \
    perirod_)  \
do {  \
    if ((me_)->run_ticks != 0U) {  \
        --(me_)->run_ticks;  \
        if ((me_)->run_ticks == 0U) {  \
            (me_)->run_ticks = perirod_;  \
            M_Fsm_run_set_ready(me_);  \
        }  \
    }  \
} while (0)

/**
******************************************************************************
                            Running ticks
******************************************************************************
**/
#define M_TICKS_PERIOD   0xFFFF
#define M_ticks_run_period(ticks_,  \
    perirod_)  \
do { \
    if ((ticks_) == (perirod_)) { \
        (ticks_) = 0; \
    } \
    else { \
        ++(ticks_); \
    } \
} while (0)

/**
******************************************************************************
                            FSM state handle
******************************************************************************
**/
#define entry_exec_start_(me_) do { \
    /* should execute */ \
    if ((me_)->entry_is_ready != (M_Flag)0U) { \
        (me_)->entry_is_ready = 0x00;
#define entry_exec_end_(me_) }} while (0);

#define state_process_begin_(me_)  do {
#define state_process_end_(me_)    } while (0);

#define state_tran(target_state_) do { \
    /* tran and trigger the entry and the exit behaviour */ \
    (me->super).state = target_state_; \
    (me->super).entry_is_ready = 0xFF; \
    (me->super).exit_is_ready = 0xFF; \
} while (0)

#define exit_exec_start_(me_) do { \
    if ((me_)->exit_is_ready != (M_Flag)0) { \
        (me_)->exit_is_ready = 0x00;
#define exit_exec_end_(me_) }} while (0);

/**
******************************************************************************
                                Events
******************************************************************************
**/
/* used as flag to let the FSM AO polling */
typedef uint8_t M_Signal;
typedef struct M_Evt {
    M_Flag evt_is_updated;
    M_Signal sig;
} M_Evt;

/* sig */
#define M_Evt_sig(me_)       ((me_)->sig)
#define M_Evt_is_updated(me_) ((me_)->evt_is_updated)
/* Equals post... */
#define M_Evt_post(me_) ((me_)->evt_is_updated = 0xFF)
/* Equals dequeue... */
#define M_Evt_recv(me_) ((me_)->evt_is_updated = 0x00)

/**
******************************************************************************
                    Single buffer, used like setting flag
******************************************************************************
**/
bool M_Fsm_post(M_Evt *evt,
    M_Signal signal);

/**
******************************************************************************
                        Init and super loop
    Implementation: m_bsp.c
******************************************************************************
**/
void M_Fsm_ctors(void);
void M_Fsm_runs(void);

/**
******************************************************************************
                        In system tick timer
    Implementation: m_bsp.c
    called: Timer ISR
******************************************************************************
**/
void M_Fsm_ticks(void);

#endif /* m_fsm_h_ */
