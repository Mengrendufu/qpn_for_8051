/*==========================================================================*/
#include "qpn_conf.h"
#include "qfn_port.h"

/*
******************************************************************************
                                ready_set define
******************************************************************************
*/
volatile QSet QF_readySet_;

/**
******************************************************************************
                        The system event pool instances
******************************************************************************
**/
QMPool g_sys_evt_pool;
QF_MPOOL_EL(SysEvtBlock) g_sys_evt_pool_sto[SYS_EVT_POOL_SIZE];
