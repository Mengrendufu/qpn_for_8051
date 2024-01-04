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
						Receiver::param-polymorphism
******************************************************************************
**/
void Receiver_received_frame_process(Receiver *me) {
	switch (me->receiver_idx) {
		case System_Idx_Uart_0: {
			uint32_t debug_recv_code;
			debug_recv_code =
				(((uint32_t)(me->process_frame[2])) << 24) |
					(((uint32_t)(me->process_frame[3])) << 16) |
						(((uint32_t)(me->process_frame[4])) << 8) |
							(((uint32_t)(me->process_frame[5])) << 0);

			// printf the recv code
			// PRINTF_DEBUG_START()
			// //-------------0123456789012----
			// PRINTF_DEBUG_("Recv:::::    \r\n");
			// PRINTF_DEBUG_ASSIGN(9) 	= me->process_frame[2];
			// PRINTF_DEBUG_ASSIGN(10) = me->process_frame[3];
			// PRINTF_DEBUG_ASSIGN(11) = me->process_frame[4];
			// PRINTF_DEBUG_ASSIGN(12) = me->process_frame[5];
			// PRINTF_DEBUG_END()

			// pass recv code to AO_Blinky to log out debug info
			p_global_evt_post_ = (QEvt *)0;
			p_global_evt_post_ = Q_NEW_EVT(&g_sys_evt_pool);
			if (p_global_evt_post_) {
				p_global_evt_post_->sig = BLINKY_PRINT_DEBUG_INFO_SIG;
				Q_DOWN_CAST(Blinky_Debug_Evt *, p_global_evt_post_)->debug_code = debug_recv_code;
			}
			QACTIVE_POST(&AO_Blinky.super,
				p_global_evt_post_);

			break;
		}
		case System_Idx_Uart_1: {
			//------------0123456789012---
			// PRINTF_DEBUG("Uart1FrmRecv. \n");
			break;
		}
		case System_Idx_Uart_2: {
			//------------0123456789012---
			// PRINTF_DEBUG("Uart2FrmRecv. \n");
			break;
		}
		case System_Idx_Uart_3: {
			//------------0123456789012---
			// PRINTF_DEBUG("Uart3FrmRecv. \n");
			break;
		}
		case System_Idx_Uart_4: {
			//------------0123456789012---
			// PRINTF_DEBUG("Uart4FrmRecv. \n");
			break;
		}
		case System_Idx_Uart_5: {
			//------------0123456789012---
			// PRINTF_DEBUG("Uart5FrmRecv. \n");
			break;
		}
		default: {  // error
			break;
		}
	}

	return;
}
/*..........................................................................*/
void Receiver_data_download(Receiver *me) {
	switch (me->receiver_idx) {
		case System_Idx_Uart_0: {
			me->ISR_recv_data = SYSTEM_IDX_UART_0_RX_BUF;
			break;
		}
		case System_Idx_Uart_1: {
			// me->ISR_recv_data = SYSTEM_IDX_UART_1_RX_BUF;
			break;
		}
		case System_Idx_Uart_2: {
			me->ISR_recv_data = SYSTEM_IDX_UART_2_RX_BUF;
			break;
		}
		case System_Idx_Uart_3: {
			// me->ISR_recv_data = SYSTEM_IDX_UART_3_RX_BUF;
			break;
		}
		case System_Idx_Uart_4: {
			// me->ISR_recv_data = SYSTEM_IDX_UART_4_RX_BUF;
			break;
		}
		case System_Idx_Uart_5: {
			// me->ISR_recv_data = SYSTEM_IDX_UART_5_RX_BUF;
			break;
		}
		default: {
			break;
		}
	}

	return;
}
/*..........................................................................*/
void Receiver_recv_code_head_process(Receiver *me) {
	switch (me->receiver_idx) {
		case System_Idx_Uart_0: {
			// receive data ==> idx check ==> code head content check
			if 		(me->receiver_msg_len - me->recv_frame_cnt == 1U) {
				// check code head content
				if (me->recv_frame[me->receiver_msg_len -
						me->recv_frame_cnt - 1U] != 0xAA)
				{
					me->ISR_state_entry_is_ready = 0xFF;
					me->ISR_call_state = Receiver_ISR_byte_missing;
				}
			}
			else if (me->receiver_msg_len - me->recv_frame_cnt == 2U) {
				// check code head content
				if (me->recv_frame[me->receiver_msg_len -
						me->recv_frame_cnt - 1U] != 0x55)
				{
					me->ISR_state_entry_is_ready = 0xFF;
					me->ISR_call_state = Receiver_ISR_byte_missing;
				}
			}
			// code head is done
			else {
				me->ISR_state_entry_is_ready = 0xFF;
				me->ISR_call_state = Receiver_ISR_receiving;
			}
			break;
		}
		case System_Idx_Uart_1: {
			// // receive data ==> idx check ==> code head content check
			// if 		(me->receiver_msg_len - me->recv_frame_cnt == 1U) {
			// 	// check code head content
			// 	if (me->recv_frame[me->receiver_msg_len -
			// 			me->recv_frame_cnt - 1U] != 0xAA)
			// 	{
			// 		me->ISR_state_entry_is_ready = 0xFF;
			// 		me->ISR_call_state = Receiver_ISR_byte_missing;
			// 	}
			// }
			// else if (me->receiver_msg_len - me->recv_frame_cnt == 2U) {
			// 	// check code head content
			// 	if (me->recv_frame[me->receiver_msg_len -
			// 			me->recv_frame_cnt - 1U] != 0x55)
			// 	{
			// 		me->ISR_state_entry_is_ready = 0xFF;
			// 		me->ISR_call_state = Receiver_ISR_byte_missing;
			// 	}
			// }
			// // code head is done
			// else {
			// 	me->ISR_state_entry_is_ready = 0xFF;
			// 	me->ISR_call_state = Receiver_ISR_receiving;
			// }
			break;
		}
		case System_Idx_Uart_2: {
			// // receive data ==> idx check ==> code head content check
			// if 		(me->receiver_msg_len - me->recv_frame_cnt == 1U) {
			// 	// check code head content
			// 	if (me->recv_frame[me->receiver_msg_len -
			// 			me->recv_frame_cnt - 1U] != 0xAA)
			// 	{
			// 		me->ISR_state_entry_is_ready = 0xFF;
			// 		me->ISR_call_state = Receiver_ISR_byte_missing;
			// 	}
			// }
			// else if (me->receiver_msg_len - me->recv_frame_cnt == 2U) {
			// 	// check code head content
			// 	if (me->recv_frame[me->receiver_msg_len -
			// 			me->recv_frame_cnt - 1U] != 0x55)
			// 	{
			// 		me->ISR_state_entry_is_ready = 0xFF;
			// 		me->ISR_call_state = Receiver_ISR_byte_missing;
			// 	}
			// }
			// // code head is done
			// else {
			// 	me->ISR_state_entry_is_ready = 0xFF;
			// 	me->ISR_call_state = Receiver_ISR_receiving;
			// }
			break;
		}
		case System_Idx_Uart_3: {
			// // receive data ==> idx check ==> code head content check
			// if 		(me->receiver_msg_len - me->recv_frame_cnt == 1U) {
			// 	// check code head content
			// 	if (me->recv_frame[me->receiver_msg_len -
			// 			me->recv_frame_cnt - 1U] != 0xAA)
			// 	{
			// 		me->ISR_state_entry_is_ready = 0xFF;
			// 		me->ISR_call_state = Receiver_ISR_byte_missing;
			// 	}
			// }
			// else if (me->receiver_msg_len - me->recv_frame_cnt == 2U) {
			// 	// check code head content
			// 	if (me->recv_frame[me->receiver_msg_len -
			// 			me->recv_frame_cnt - 1U] != 0x55)
			// 	{
			// 		me->ISR_state_entry_is_ready = 0xFF;
			// 		me->ISR_call_state = Receiver_ISR_byte_missing;
			// 	}
			// }
			// // code head is done
			// else {
			// 	me->ISR_state_entry_is_ready = 0xFF;
			// 	me->ISR_call_state = Receiver_ISR_receiving;
			// }
			break;
		}
		case System_Idx_Uart_4: {
			// // receive data ==> idx check ==> code head content check
			// if 		(me->receiver_msg_len - me->recv_frame_cnt == 1U) {
			// 	// check code head content
			// 	if (me->recv_frame[me->receiver_msg_len -
			// 			me->recv_frame_cnt - 1U] != 0xAA)
			// 	{
			// 		me->ISR_state_entry_is_ready = 0xFF;
			// 		me->ISR_call_state = Receiver_ISR_byte_missing;
			// 	}
			// }
			// else if (me->receiver_msg_len - me->recv_frame_cnt == 2U) {
			// 	// check code head content
			// 	if (me->recv_frame[me->receiver_msg_len -
			// 			me->recv_frame_cnt - 1U] != 0x55)
			// 	{
			// 		me->ISR_state_entry_is_ready = 0xFF;
			// 		me->ISR_call_state = Receiver_ISR_byte_missing;
			// 	}
			// }
			// // code head is done
			// else {
			// 	me->ISR_state_entry_is_ready = 0xFF;
			// 	me->ISR_call_state = Receiver_ISR_receiving;
			// }
			break;
		}
		case System_Idx_Uart_5: {
			// // receive data ==> idx check ==> code head content check
			// if 		(me->receiver_msg_len - me->recv_frame_cnt == 1U) {
			// 	// check code head content
			// 	if (me->recv_frame[me->receiver_msg_len -
			// 			me->recv_frame_cnt - 1U] != 0xAA)
			// 	{
			// 		me->ISR_state_entry_is_ready = 0xFF;
			// 		me->ISR_call_state = Receiver_ISR_byte_missing;
			// 	}
			// }
			// else if (me->receiver_msg_len - me->recv_frame_cnt == 2U) {
			// 	// check code head content
			// 	if (me->recv_frame[me->receiver_msg_len -
			// 			me->recv_frame_cnt - 1U] != 0x55)
			// 	{
			// 		me->ISR_state_entry_is_ready = 0xFF;
			// 		me->ISR_call_state = Receiver_ISR_byte_missing;
			// 	}
			// }
			// // code head is done
			// else {
			// 	me->ISR_state_entry_is_ready = 0xFF;
			// 	me->ISR_call_state = Receiver_ISR_receiving;
			// }
			break;
		}
		default: {
			break;
		}
	}

	return;
}
/*..........................................................................*/
uint8_t Receiver_recv_is_code_head(Receiver *me) {
	switch (me->receiver_idx) {
		case System_Idx_Uart_0: {
			if (me->ISR_recv_data == 0xAA) {
				return 1U;
			}
			break;
		}
		case System_Idx_Uart_1: {
			// if (me->ISR_recv_data == 0xAA) {
			// 	return 1U;
			// }
			break;
		}
		case System_Idx_Uart_2: {
			// if (me->ISR_recv_data == 0xAA) {
			// 	return 1U;
			// }
			break;
		}
		case System_Idx_Uart_3: {
			// if (me->ISR_recv_data == 0xAA) {
			// 	return 1U;
			// }
			break;
		}
		case System_Idx_Uart_4: {
			// if (me->ISR_recv_data == 0xAA) {
			// 	return 1U;
			// }
			break;
		}
		case System_Idx_Uart_5: {
			// if (me->ISR_recv_data == 0xAA) {
			// 	return 1U;
			// }
			break;
		}
		default: {
			break;
		}
	}

	return 0U;
}
/*..........................................................................*/
void Receiver_init_checksum(Receiver *me) {
	switch (me->receiver_idx) {
		case System_Idx_Uart_0: {
			me->yield_checksum = (uint32_t)(INIT_CRC16_CCITT_FALSE);
			break;
		}
		case System_Idx_Uart_1: {
			// me->yield_checksum = (uint32_t)(INIT_CRC16_CCITT_FALSE);
			break;
		}
		case System_Idx_Uart_2: {
			// me->yield_checksum = (uint32_t)(INIT_CRC16_CCITT_FALSE);
			break;
		}
		case System_Idx_Uart_3: {
			// me->yield_checksum = (uint32_t)(INIT_CRC16_CCITT_FALSE);
			break;
		}
		case System_Idx_Uart_4: {
			// me->yield_checksum = (uint32_t)(INIT_CRC16_CCITT_FALSE);
			break;
		}
		case System_Idx_Uart_5: {
			// me->yield_checksum = (uint32_t)(INIT_CRC16_CCITT_FALSE);
			break;
		}
		default: {
			break;
		}
	}

	return;
}
/*..........................................................................*/
void Receiver_yiled_checksum(Receiver *me) {
	switch (me->receiver_idx) {
		case System_Idx_Uart_0: {
			// when to yield the checksum
			if (me->recv_frame_cnt >= 2) {
				me->yield_checksum = (uint32_t)update_crc16_ccitt_false(
					me->yield_checksum,
					me->ISR_recv_data);
			}
			// break;
		}
		case System_Idx_Uart_1: {
			// when to yield the checksum
			// if (me->recv_frame_cnt >= 2) {
				// me->yield_checksum = (uint32_t)update_crc16_ccitt_false(
					// me->yield_checksum,
					// me->ISR_recv_data);
			// }
			break;
		}
		case System_Idx_Uart_2: {
			// when to yield the checksum
			// if (me->recv_frame_cnt >= 2) {
				// me->yield_checksum = (uint32_t)update_crc16_ccitt_false(
					// me->yield_checksum,
					// me->ISR_recv_data);
			// }
			break;
		}
		case System_Idx_Uart_3: {
			// when to yield the checksum
			// if (me->recv_frame_cnt >= 2) {
				// me->yield_checksum = (uint32_t)update_crc16_ccitt_false(
					// me->yield_checksum,
					// me->ISR_recv_data);
			// }
			break;
		}
		case System_Idx_Uart_4: {
			// when to yield the checksum
			// if (me->recv_frame_cnt >= 2) {
				// me->yield_checksum = (uint32_t)update_crc16_ccitt_false(
					// me->yield_checksum,
					// me->ISR_recv_data);
			// }
			break;
		}
		case System_Idx_Uart_5: {
			// when to yield the checksum
			// if (me->recv_frame_cnt >= 2) {
				// me->yield_checksum = (uint32_t)update_crc16_ccitt_false(
					// me->yield_checksum,
					// me->ISR_recv_data);
			// }
			break;
		}
		default: {
			break;
		}
	}

	return;
}
/*..........................................................................*/
uint8_t Receiver_checksum_is_same(Receiver *me) {
	uint32_t receiver_recv_checksum;

	switch (me->receiver_idx) {
		case System_Idx_Uart_0: {
			// how to get the checksum from the recv_frame
			receiver_recv_checksum =
				((uint32_t)
					(((uint16_t)
						(me->recv_frame[RECEIVER_DEBUG_MSG_LEN - 1 -1]))
							<< 8))
					| ((uint32_t)
						(((uint16_t)
							(me->recv_frame[RECEIVER_DEBUG_MSG_LEN - 1]))
								<< 0));
			// compare
			if (receiver_recv_checksum == me->yield_checksum) {
				return 1U;
			}
			break;
		}
		case System_Idx_Uart_1: {
			// how to get the checksum from the recv_frame
			// receiver_recv_checksum =
			// 	((uint32_t)
			// 		(((uint16_t)
			// 			(me->recv_frame[RECEIVER_DEBUG_MSG_LEN - 1 -1]))
			// 				<< 8))
			// 		| ((uint32_t)
			// 			(((uint16_t)
			// 				(me->recv_frame[RECEIVER_DEBUG_MSG_LEN - 1]))
			// 					<< 0));
			// // compare
			// if (receiver_recv_checksum == me->yield_checksum) {
			// 	return 1U;
			// }
			break;
		}
		case System_Idx_Uart_2: {
			// how to get the checksum from the recv_frame
			// receiver_recv_checksum =
			// 	((uint32_t)
			// 		(((uint16_t)
			// 			(me->recv_frame[RECEIVER_DEBUG_MSG_LEN - 1 -1]))
			// 				<< 8))
			// 		| ((uint32_t)
			// 			(((uint16_t)
			// 				(me->recv_frame[RECEIVER_DEBUG_MSG_LEN - 1]))
			// 					<< 0));
			// // compare
			// if (receiver_recv_checksum == me->yield_checksum) {
			// 	return 1U;
			// }
			break;
		}
		case System_Idx_Uart_3: {
			// how to get the checksum from the recv_frame
			// receiver_recv_checksum =
			// 	((uint32_t)
			// 		(((uint16_t)
			// 			(me->recv_frame[RECEIVER_DEBUG_MSG_LEN - 1 -1]))
			// 				<< 8))
			// 		| ((uint32_t)
			// 			(((uint16_t)
			// 				(me->recv_frame[RECEIVER_DEBUG_MSG_LEN - 1]))
			// 					<< 0));
			// // compare
			// if (receiver_recv_checksum == me->yield_checksum) {
			// 	return 1U;
			// }
			break;
		}
		case System_Idx_Uart_4: {
			// how to get the checksum from the recv_frame
			// receiver_recv_checksum =
			// 	((uint32_t)
			// 		(((uint16_t)
			// 			(me->recv_frame[RECEIVER_DEBUG_MSG_LEN - 1 -1]))
			// 				<< 8))
			// 		| ((uint32_t)
			// 			(((uint16_t)
			// 				(me->recv_frame[RECEIVER_DEBUG_MSG_LEN - 1]))
			// 					<< 0));
			// // compare
			// if (receiver_recv_checksum == me->yield_checksum) {
			// 	return 1U;
			// }
			break;
		}
		case System_Idx_Uart_5: {
			// how to get the checksum from the recv_frame
			// receiver_recv_checksum =
			// 	((uint32_t)
			// 		(((uint16_t)
			// 			(me->recv_frame[RECEIVER_DEBUG_MSG_LEN - 1 -1]))
			// 				<< 8))
			// 		| ((uint32_t)
			// 			(((uint16_t)
			// 				(me->recv_frame[RECEIVER_DEBUG_MSG_LEN - 1]))
			// 					<< 0));
			// // compare
			// if (receiver_recv_checksum == me->yield_checksum) {
			// 	return 1U;
			// }
			break;
		}
		default: {
			break;
		}
	}

	return 0U;
}
