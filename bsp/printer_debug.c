/**
******************************************************************************
                                Includes
******************************************************************************
**/
#include "m_fsm_port.h"
#include "qmpool.h"
#include "queue.h"
#include "printer.h"
#include "printer_debug.h"

/**
******************************************************************************
                                Instances
******************************************************************************
**/
Printer Printer_debug;

static QMPool Printer_debug_frame_pool;
static QF_MPOOL_EL(Printer_Debug_Block)
    Printer_debug_frame_pool_sto[PRINTER_DEBUG_FRAME_POOL_SIZE];

static CircularQueue Printer_debug_tran_queue;
static void *Printer_debug_tran_queue_sto[PRINTER_DEBUG_TRAN_QUEUE_SIZE];

uint8_t Printer_debug_tmp_frame[PRINTER_DEBUG_MSG_LEN];

/**
******************************************************************************
                                Implementations
******************************************************************************
**/
void Printer_debug_ctor(Printer *me) {
    Printer_ctor(me,
        &Printer_debug_frame_pool,
        Printer_debug_frame_pool_sto,
        sizeof(Printer_debug_frame_pool_sto),
        sizeof(Printer_debug_frame_pool_sto[0]),
        &Printer_debug_tran_queue,
        Printer_debug_tran_queue_sto,
        PRINTER_DIM(Printer_debug_tran_queue_sto),
        PRINTER_DEBUG_IDX, /* Printer_debug corsespond uart_1 */
        PRINTER_DEBUG_MSG_LEN);

    return;
}
