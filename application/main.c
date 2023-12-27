/*==========================================================================*/
#include "qpn.h"
#include "bsp.h"
#include "application.h"

/* Local-scope objects -----------------------------------------------------*/
static QEvt *l_blinky_q_sto[5]; /* Event queue storage for Blinky */

/* QF_active[] array defines all active object control blocks --------------*/
QActiveCB const Q_ROM QF_active[] = {
    {(QActive *)0,
        (QEvt **)0,
        0U},

    {(QActive *)&AO_Blinky,
        (QEvt **)l_blinky_q_sto,
        Q_DIM(l_blinky_q_sto)}
};

/*..........................................................................*/
void main(void) {
    Blinky_ctor(&AO_Blinky); /* instantiate all Blinky AO */

    QF_init(Q_DIM(QF_active)); /* initialize the QF-nano framework */
    BSP_init();      /* initialize the Board Support Package */

    QF_run(); /* transfer control to QF-nano */

    return;
}
