/*==========================================================================*/
#include "qpn.h"
#include "bsp.h"
#include "application.h"

/* Local-scope objects -----------------------------------------------------*/
static QEvt *l_blinky_q_sto[BLINKY_EVT_QUEUE_SIZE];

/* QF_active[] array defines all active object control blocks --------------*/
QActiveCB const Q_ROM QF_active[] = {
    {(QActive *)0,
        (QEvt **)0,
        0U},

    {(QActive *)&AO_Blinky,
        (QEvt **)l_blinky_q_sto,
        Q_DIM(l_blinky_q_sto)}
};

/**
******************************************************************************
                            AO construct
                                ||
                                ||
                                VV
                        QF-nano static init
                                ||
                                ||
                                VV
                    BSP tasks init (BSP_init())
                                ||
                                ||
                                VV
                    BSP chip init (BSP_init())
                                ||
                                ||
                                VV
                            SM QHsm_init_
                                ||
                                ||
                                VV
                            QF_onStartup
                                ||
                                ||
                                VV
            background super loop tasks & event loop & on_idle
******************************************************************************
**/
void main(void) {
    Blinky_ctor(&AO_Blinky); /* instantiate all Blinky AO */

    QF_init(Q_DIM(QF_active)); /* initialize the QF-nano framework */
    BSP_init();      /* initialize the Board Support Package */

    QF_run(); /* transfer control to QF-nano */

    return;
}
