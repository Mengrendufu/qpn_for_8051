#ifndef printer_debug_h_
#define printer_debug_h_

/**
******************************************************************************
                        Printer_debug params
    How long, how big, what is the size of yours.
******************************************************************************
**/
#define PRINTER_DEBUG_IDX  System_Idx_Uart_0

#define PRINTER_DEBUG_MSG_LEN          15U
#define PRINTER_DEBUG_FRAME_POOL_SIZE  16U
#define PRINTER_DEBUG_TRAN_QUEUE_SIZE  PRINTER_DEBUG_FRAME_POOL_SIZE

/**
******************************************************************************
                        Printer_debug instance
    Printer_xx
******************************************************************************
**/
extern Printer Printer_debug;

/**
******************************************************************************
                    Frame memory pool unit block
******************************************************************************
**/
typedef struct Printer_Debug_Block {
    void *size_padding_unit[PRINTER_CEIL(PRINTER_DEBUG_MSG_LEN, Q_PTR_SIZE)];
} Printer_Debug_Block;

/**
******************************************************************************
                        Printer_debug constructor
    Package, call Printer_ctor() inside;
******************************************************************************
**/
void Printer_debug_ctor(Printer *me);

/**
******************************************************************************
                                Debug output
    Macro packages, it's about Printer_memcpy() and Printer_printf()
******************************************************************************
**/

/**
******************************************************************************
    Only do this:
//------------0123456789012---
PRINTF_DEBUG("Hello, World! \n");
******************************************************************************
**/
#define PRINTF_DEBUG(msg_)  \
do {  \
    Printer_printf(&Printer_debug,  \
        (uint8_t *)(msg_));  \
} while (0U)

/**
******************************************************************************
    We do this:
PRINTF_DEBUG_START()
//-------------0123456789012---
PRINTF_DEBUG_("Hello, World! \n");
PRINTF_DEBUG_ASSIGN(12) = 0x88;
PRINTF_DEBUG_END()
******************************************************************************
**/
extern uint8_t Printer_debug_tmp_frame[PRINTER_DEBUG_MSG_LEN];
#define PRINTF_DEBUG_START()  \
do {

#define PRINTF_DEBUG_(msg_)  \
do {  \
    Printer_memcpy((msg_),  \
        Printer_debug_tmp_frame,  \
        Printer_debug.printer_msg_len);  \
} while (0U)

#define PRINTF_DEBUG_ASSIGN(i_)  \
    (Printer_debug_tmp_frame[(i_)])

#define PRINTF_DEBUG_END()  \
    Printer_printf(&Printer_debug,  \
        Printer_debug_tmp_frame);  \
} while (0U);

#endif  /* printer_debug_h_ */
