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

    me->blinky_is_on = 0xFF;
    me->ticks = 0U;

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

    return Q_TRAN(&Blinky_active);
}
QState Blinky_active(Blinky * const me) {
    QState status;
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
            QF_INT_DISABLE();
            //------------0123456789012----
            PRINTF_DEBUG("BlinkyActive \r\n");
            QF_INT_ENABLE();

            me->blinky_is_on = 0xFF;
            me->ticks = 0U;

            status = Q_HANDLED();
            break;
        }
        case Q_TIMEOUT3_SIG: {
            // me->ticks:: 1U ~ 65535U
            CYC_RST_COUNTER(me->ticks,
                1U,
                65535U);

            // Every 30s
            if (me->ticks % 1000U == 0U) {
                me->blinky_is_on ^= 0xFF;

                // led toggle
                if (me->blinky_is_on) {
                    BSP_ledOn();
                }
                else {
                    BSP_ledOff();
                }
            }

            status = Q_HANDLED();
            break;
        }
        case BLINKY_PRINT_DEBUG_INFO_SIG: {
            // trigger by Receiver_debug, monitor something via the msg
            // uint32_t :: debug_code
            switch (Q_DOWN_CAST(Blinky_Debug_Evt *, Q_EVT(me))->debug_code) {
                case 0x00000000: {
                    // print some interesting sys information.
                    QF_INT_DISABLE();

                    PRINTF_DEBUG_START()
                    //-------------0123456789012----
                    PRINTF_DEBUG_("QF_maxAct   :\r\n");
                    PRINTF_DEBUG_ASSIGN(12) = QF_maxActive_;
                    PRINTF_DEBUG_END()

                    PRINTF_DEBUG_START()
                    //-------------0123456789012----
                    PRINTF_DEBUG_("SysEvtPool:: \r\n");
                    PRINTF_DEBUG_END()

                    PRINTF_DEBUG_START()
                    //-------------0123456789012----
                    PRINTF_DEBUG_("    nTot     \r\n");
                    PRINTF_DEBUG_ASSIGN(11) = (uint8_t)(g_sys_evt_pool.nTot >> 8);
                    PRINTF_DEBUG_ASSIGN(12) = (uint8_t)(g_sys_evt_pool.nTot >> 0);
                    PRINTF_DEBUG_END()

                    PRINTF_DEBUG_START()
                    //-------------0123456789012----
                    PRINTF_DEBUG_("    nFree    \r\n");
                    PRINTF_DEBUG_ASSIGN(11) = (uint8_t)(g_sys_evt_pool.nFree >> 8);
                    PRINTF_DEBUG_ASSIGN(12) = (uint8_t)(g_sys_evt_pool.nFree >> 0);
                    PRINTF_DEBUG_END()

                    PRINTF_DEBUG_START()
                    //-------------0123456789012----
                    PRINTF_DEBUG_("    nMin     \r\n");
                    PRINTF_DEBUG_ASSIGN(11) = (uint8_t)(g_sys_evt_pool.nMin >> 8);
                    PRINTF_DEBUG_ASSIGN(12) = (uint8_t)(g_sys_evt_pool.nMin >> 0);
                    PRINTF_DEBUG_END()

                    PRINTF_DEBUG_START()
                    //-------------0123456789012----
                    PRINTF_DEBUG_("PrtrDbgPool::\r\n");
                    PRINTF_DEBUG_END()

                    PRINTF_DEBUG_START()
                    //-------------0123456789012----
                    PRINTF_DEBUG_("    nTot     \r\n");
                    PRINTF_DEBUG_ASSIGN(11) = (uint8_t)(Printer_debug.frame_pool->nTot >> 8);
                    PRINTF_DEBUG_ASSIGN(12) = (uint8_t)(Printer_debug.frame_pool->nTot >> 0);
                    PRINTF_DEBUG_END()

                    PRINTF_DEBUG_START()
                    //-------------0123456789012----
                    PRINTF_DEBUG_("    nMin     \r\n");
                    PRINTF_DEBUG_ASSIGN(11) = (uint8_t)(Printer_debug.frame_pool->nMin >> 8);
                    PRINTF_DEBUG_ASSIGN(12) = (uint8_t)(Printer_debug.frame_pool->nMin >> 0);
                    PRINTF_DEBUG_END()

                    QF_INT_ENABLE();
                    break;
                }
                case 0x00000001: {

                    break;
                }
                default: {
                    break;
                }
            }

            status = Q_HANDLED();
            break;
        }
        default: {
            status = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status;
}
