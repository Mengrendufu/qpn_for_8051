/*==========================================================================*/
#include "qpn_conf.h"
#include "qfn_port.h"

/*
******************************************************************************
                                ready_set define
******************************************************************************
*/
Q_ReadySet volatile QF_readySet_;

/**
******************************************************************************
                        The system event pool instances
******************************************************************************
**/
QMPool g_sys_evt_pool;
QF_MPOOL_EL(SysEvtBlock) g_sys_evt_pool_sto[SYS_EVT_POOL_SIZE];

/*
******************************************************************************
                                clz operation
******************************************************************************
*/
uint_fast8_t Q_LOG2(Q_ReadySet x) {
    static uint8_t const log2LUT[16] = {
        0U,                             /* [0b0000] */
        1U,                             /* [0b0001] */
        2U, 2U,                         /* [0b001x] */
        3U, 3U, 3U, 3U,                 /* [0b01xx] */
        4U, 4U, 4U, 4U, 4U, 4U, 4U, 4U  /* [0b1xxx] */
    };
    uint_fast8_t n;
    Q_ReadySet tmp;
    Q_ReadySet x_;

    n = 0U;
    x_ = x;

#if (MAX_ACTIVE > 16U)
    tmp = (Q_ReadySet)(x_ >> 16U);
    if (tmp != 0U) {
        n += 16U;
        x_ = tmp;
    }
#endif

#if (MAX_ACTIVE > 8U)
    tmp = (x_ >> 8U);
    if (tmp != 0U) {
        n += 8U;
        x_ = tmp;
    }
#endif

    tmp = (x_ >> 4U);
    if (tmp != 0U) {
        n += 4U;
        x_ = tmp;
    }

    n += log2LUT[x_];

    return n;
}
