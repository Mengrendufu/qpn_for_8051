/*==========================================================================*/
#include "qpn.h"
#include "bsp.h"
#include "application.h"

/**
******************************************************************************
                            BSP functions
******************************************************************************
**/
void BSP_init(void) {
	super_loop_tasks_init();

	MCU_Init();
	return;
}
/*..........................................................................*/
void BSP_ledOff(void) {
	QF_INT_DISABLE();
	//            0123456789012----
	PRINTF_DEBUG("LedOff       \r\n");
	QF_INT_ENABLE();

	return;
}
/*..........................................................................*/
void BSP_ledOn(void) {
	QF_INT_DISABLE();
	//            0123456789012----
	PRINTF_DEBUG("LedOn        \r\n");
	QF_INT_ENABLE();

	return;
}

/**
******************************************************************************
                            QAssert callbacks
	Not used here temporarily
******************************************************************************
**/
Q_NORETURN Q_onAssert(char_t const Q_ROM * const module,
    int_t const location)
{
	(void)module;
	(void)location;

	return;
}

/**
******************************************************************************
								QF callbacks
******************************************************************************
**/
void QF_onStartup(void) {
	/**
	**************************************************************************
				Static info when the framework is ready to run
	**************************************************************************
	**/
	QF_INT_DISABLE();  // enter critical section

	//            0123456789012----
	PRINTF_DEBUG("Constructors \r\n");
	PRINTF_DEBUG("QF_init      \r\n");
	PRINTF_DEBUG("BSP_init     \r\n");
	PRINTF_DEBUG("  SpLpTskInit\r\n");
	PRINTF_DEBUG("  MCU_init   \r\n");
	PRINTF_DEBUG("StaticAllDone\r\n");
	PRINTF_DEBUG("DynFromNow   \r\n");
	PRINTF_DEBUG("QHsm_init_   \r\n");
	PRINTF_DEBUG("QF_onStartup \r\n");

	PRINTF_DEBUG_START()
	//             0123456789012----
	PRINTF_DEBUG_("SEP:         \r\n");
	PRINTF_DEBUG_ASSIGN(4) = (uint8_t)(g_sys_evt_pool.blockSize);
	PRINTF_DEBUG_ASSIGN(5) = (uint8_t)(g_sys_evt_pool.nTot);
	PRINTF_DEBUG_ASSIGN(6) = (uint8_t)(g_sys_evt_pool.nFree);
	PRINTF_DEBUG_ASSIGN(7) = (uint8_t)(g_sys_evt_pool.nMin);
	PRINTF_DEBUG_END()

	PRINTF_DEBUG_START()
	//             0123456789012----
	PRINTF_DEBUG_("PFP:         \r\n");
	PRINTF_DEBUG_ASSIGN(4) = Printer_debug.frame_pool->blockSize;
	PRINTF_DEBUG_ASSIGN(5) = Printer_debug.frame_pool->nTot;
	PRINTF_DEBUG_ASSIGN(6) = Printer_debug.frame_pool->nFree;
	PRINTF_DEBUG_ASSIGN(7) = Printer_debug.frame_pool->nMin;
	PRINTF_DEBUG_END()

	PRINTF_DEBUG_START()
	//             0123456789012----
	PRINTF_DEBUG_("PTQ:         \r\n");
	PRINTF_DEBUG_ASSIGN(4) = Printer_debug.tran_queue->q_len;
	PRINTF_DEBUG_ASSIGN(5) = Printer_debug.tran_queue->n_used;
	PRINTF_DEBUG_END()

	QF_INT_ENABLE();  // quit critical section

	return;
}
/*..........................................................................*/
void QF_stop(void) {
	return;
}
/*..........................................................................*/
void QV_onIdle(void) { /* NOTE: called with interrutps DISABLED, see NOTE1 */
	QF_INT_ENABLE();
	return;
}

/**
******************************************************************************
								super loop task
******************************************************************************
**/
void super_loop_tasks_init(void) {
	// all the m_fsm class
	M_Fsm_ctors();

	return;
}
/*..........................................................................*/
void super_loop_tasks(void) {
	QF_INT_ENABLE();  // MUST enable the interrupts when enter

	/**
	**************************************************************************
				do the m_fsm job and feed the watch dog if necessary
	**************************************************************************
	**/

	// clear WatchDog
	WATCHDOG_CLEAR();

	// Background M_Fsm class
	M_Fsm_runs();

	QF_INT_DISABLE();  // MUST return with interrrupts disable
	return;
}

/**
******************************************************************************
								M_Fsm callback
******************************************************************************
**/
void M_Fsm_ctors(void) {
	// Printer_debug
	Printer_debug_ctor(&Printer_debug);
	// Receiver_debug
	Receiver_debug_ctor(&Receiver_debug);

	return;
}
/*..........................................................................*/
void M_Fsm_runs(void) {
	// Printer_debug
	Printer_run(&Printer_debug);
	// Receiver_debug
	Receiver_run(&Receiver_debug);

	return;
}
/*..........................................................................*/
void M_Fsm_ticks(void) {
	// Printer_debug
	M_Fsm_tick(&Printer_debug.super,
		PRINTER_TICKS_PER_SEC);
	// Receiver_debug
	M_Fsm_tick(&Receiver_debug.super,
		RECEIVER_RUN_TICKS_PER_SEC);

	return;
}
