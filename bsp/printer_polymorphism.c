/**
******************************************************************************
                                Includes
******************************************************************************
**/
#include "qpn.h"
#include "bsp.h"
#include "application.h"

/**
******************************************************************************
						Printer::polymorphism handlers
******************************************************************************
**/
void Printer_data_upload(Printer *me) {
	switch (me->printer_idx) {
		case System_Idx_Uart_0: {
			SYSTEM_IDX_UART_0_TX_BUF =
				me->tran_frame[me->printer_msg_len - me->tran_frame_cnt];
			break;
		}
		// Printer Printer_debug;
		case System_Idx_Uart_1: {
			// UART1_BUF =
			// 	me->tran_frame[me->printer_msg_len - me->tran_frame_cnt];
			break;
		}
		case System_Idx_Uart_2: {
			break;
		}
		case System_Idx_Uart_3: {
			break;
		}
		case System_Idx_Uart_4: {
			break;
		}
		case System_Idx_Uart_5: {
			break;
		}
		default: {
			break;
		}
	}

	return;
}
