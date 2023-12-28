#ifndef QFN_PORT_H
#define QFN_PORT_H

/**
******************************************************************************
                                    cpu port
******************************************************************************
**/
#include "cpu_port.h"

/**
******************************************************************************
                                    qfn
******************************************************************************
**/
#define QF_INT_DISABLE()  \
do {  \
    CPU_INT_DISABLE();  \
} while (false)

#define QF_INT_ENABLE()  \
do {  \
    CPU_INT_ENABLE();  \
} while (false)

#define Q_ROM code

/*
******************************************************************************
                            common packages
******************************************************************************
*/
#include "common.h"

/*
******************************************************************************
                            super loop tasks
                    declare here, definition in the bsp
******************************************************************************
*/
void super_loop_tasks_init(void);
void super_loop_tasks(void);

/*
******************************************************************************
                        bigger ready_set for AOs
                    self-defined log2 to perform clz
******************************************************************************
*/
typedef uint32_t Q_ReadySet;
extern Q_ReadySet volatile QF_readySet_;
uint_fast8_t Q_LOG2(Q_ReadySet x);
#define QF_LOG2(x)  (Q_LOG2((x)))

/*
******************************************************************************
                        dynamic event strategy
******************************************************************************
*/
// behind the qmpool.h
extern QMPool g_sys_evt_pool;
#define SYS_EVT_POOL_SIZE  32U
#define SYS_EVT_POOL_UNIT_SIZE  6U  // Exact width of a evt in the pool.
typedef struct SysEvtBlock {
    void *size_padding_unit[Q_CEIL(SYS_EVT_POOL_UNIT_SIZE, Q_PTR_SIZE)];
} SysEvtBlock;
extern QF_MPOOL_EL(SysEvtBlock) g_sys_evt_pool_sto[SYS_EVT_POOL_SIZE];

#include "qepn.h"        /* QEP-nano platform-independent public interface */
#include "qfn.h"         /* QF-nano  platform-independent public interface */
#define Q_NEW_EVT(m_pool_)  \
    ((QEvt *)QMPool_get(m_pool_))
#define Q_GC_EVT(m_pool_, \
    e_) \
    QMPool_put(m_pool_, \
        e_);
#include "qvn.h"         /* QV-nano  platform-independent public interface */

#endif /* QFN_PORT_H */
