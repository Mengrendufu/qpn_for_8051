#ifndef BLINKY_H
#define BLINKY_H

/**
******************************************************************************
                            Ticks rate (ms)
******************************************************************************
**/
#define BLINKY_TICKS_PER_SEC  500U

/**
******************************************************************************
                        Blinky private event signals
******************************************************************************
**/
enum BlinkySignals {
    Blinky_dummy = Q_USER_SIG,

    MAX_SIG  /* the last signal */
};

// event queue size
#define BLINKY_EVT_QUEUE_SIZE  10U

/**
******************************************************************************
                        AO class (declared by the QM)
******************************************************************************
**/
typedef struct Blinky {  /* the Blinky active object */
    QActive super;  /* inherit QActive */
} Blinky;
QState Blinky_initial(Blinky * const me);
QState Blinky_off(Blinky * const me);
QState Blinky_on(Blinky * const me);

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
