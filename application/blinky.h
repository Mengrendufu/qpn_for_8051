#ifndef BLINKY_H
#define BLINKY_H

/**
******************************************************************************
                            Ticks rate (ms)
******************************************************************************
**/
#define BLINKY_TICKS_PER_SEC  10U

/**
******************************************************************************
                        Blinky private event signals
******************************************************************************
**/

// event queue size
#define BLINKY_EVT_QUEUE_SIZE  10U

enum BlinkySignals {
    Blinky_dummy = Q_USER_SIG,

    BLINKY_PRINT_DEBUG_INFO_SIG,

    MAX_SIG  /* the last signal */
};

/**
******************************************************************************
    Events from Receiver_debug to print out debug info steply.
******************************************************************************
**/
typedef struct {
    QEvt super;

    uint32_t debug_code;
} Blinky_Debug_Evt;

/**
******************************************************************************
                        AO class (declared by the QM)
******************************************************************************
**/
typedef struct Blinky {  /* the Blinky active object */
    QActive super;  /* inherit QActive */

    uint8_t blinky_is_on;
    uint16_t ticks;  // running ticks
} Blinky;
QState Blinky_initial(Blinky * const me);
QState Blinky_active(Blinky * const me);

/**
******************************************************************************
                            Instance of AO Blinky
******************************************************************************
**/
extern Blinky AO_Blinky;

/**
******************************************************************************
                                AO constructor
******************************************************************************
**/
void Blinky_ctor(Blinky *me);

#endif /* BLINKY_H */
