/*==========================================================================*/
#include "qpn.h"
#include "bsp.h"
#include "application.h"

/**
******************************************************************************
                                Global objects
******************************************************************************
**/
Blinky AO_Blinky;   /* the single instance of the Blinky AO */

/**
******************************************************************************
                                AO constructors
******************************************************************************
**/
void Blinky_ctor(Blinky *me) {
    QActive_ctor(&me->super,
        Q_STATE_CAST(&Blinky_initial));

    return;
}

/**
******************************************************************************
                            AO definition by QM
******************************************************************************
**/
QState Blinky_initial(Blinky * const me) {
    QActive_armX((QActive *)me,
        QF_TICK_RATE_3,
        BLINKY_TICKS_PER_SEC,
        BLINKY_TICKS_PER_SEC);

    return Q_TRAN(&Blinky_on);
}
/*..........................................................................*/
QState Blinky_off(Blinky * const me) {
    QState status;
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
            BSP_ledOff();
            status = Q_HANDLED();
            break;
        }
        case Q_TIMEOUT3_SIG: {
            status = Q_TRAN(&Blinky_on);
            break;
        }
        default: {
            status = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status;
}
/*..........................................................................*/
QState Blinky_on(Blinky * const me) {
    QState status;
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
            BSP_ledOn();
            status = Q_HANDLED();
            break;
        }
        case Q_TIMEOUT3_SIG: {
            status = Q_TRAN(&Blinky_off);
            break;
        }
        default: {
            status = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status;
}
