/**
******************************************************************************
                                Includes
******************************************************************************
**/
#include "m_fsm_port.h"
#include "qmpool.h"
#include "queue.h"
#include "receiver.h"
#include "receiver_debug.h"

/**
******************************************************************************
                                Instances
******************************************************************************
**/
Receiver Receiver_debug;

static QMPool Receiver_debug_frame_pool;
static QF_MPOOL_EL(Receiver_Debug_block)
    Receiver_debug_frame_pool_sto[RECEIVER_DEBUG_FRAME_POOL_SIZE];

static CircularQueue Receiver_debug_recv_queue;
static void *Receiver_debug_recv_queue_sto[RECEIVER_DEBUG_RECV_QUEUE_SIZE];

/**
******************************************************************************
                            Implementations
******************************************************************************
**/
void Receiver_debug_ctor(Receiver *me) {
    Receiver_ctor(me,
        &Receiver_debug_frame_pool,
        Receiver_debug_frame_pool_sto,
        sizeof(Receiver_debug_frame_pool_sto),
        sizeof(Receiver_debug_frame_pool_sto[0]),
        &Receiver_debug_recv_queue,
        Receiver_debug_recv_queue_sto,
        RECEIVER_DIM(Receiver_debug_recv_queue_sto),
        RECEIVER_DEBUG_IDX,
        RECEIVER_DEBUG_MSG_LEN);

    return;
}
