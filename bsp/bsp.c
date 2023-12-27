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
	UART1_Send_Byte(0x4C);
	UART1_Send_Byte(0x65);
	UART1_Send_Byte(0x64);
	UART1_Send_Byte(0x4F);
	UART1_Send_Byte(0x66);
	UART1_Send_Byte(0x66);
	UART1_Send_Byte(0x0D);
	UART1_Send_Byte(0x0A);

	return;
}
/*..........................................................................*/
void BSP_ledOn(void) {
	UART1_Send_Byte(0x4C);
	UART1_Send_Byte(0x65);
	UART1_Send_Byte(0x64);
	UART1_Send_Byte(0x4F);
	UART1_Send_Byte(0x6E);
	UART1_Send_Byte(0x0D);
	UART1_Send_Byte(0x0A);

	return;
}

/**
******************************************************************************
                            QAssert callbacks
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
	return;
}
/*..........................................................................*/
void super_loop_tasks(void) {
	QF_INT_ENABLE();

	// do the m_fsm job and feed the watch dog if necessary
	WDT_CTRL = 7;  // clear WatchDog

	QF_INT_DISABLE();  // must return with interrupt disable
	return;
}
