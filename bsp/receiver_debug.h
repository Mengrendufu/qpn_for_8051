#ifndef receiver_debug_h_
#define receiver_debug_h_

/**
******************************************************************************
                        Receiver_debug params
******************************************************************************
**/
#define RECEIVER_DEBUG_IDX  System_Idx_Uart_0

#define RECEIVER_DEBUG_MSG_LEN          8U
#define RECEIVER_DEBUG_FRAME_POOL_SIZE  3U
#define RECEIVER_DEBUG_RECV_QUEUE_SIZE  RECEIVER_DEBUG_FRAME_POOL_SIZE

/**
******************************************************************************
                            Receiver instance
    Receiver_xxx
******************************************************************************
**/
extern Receiver Receiver_debug;

/**
******************************************************************************
                    Frame memory pool unit block
******************************************************************************
**/
typedef struct Receiver_Debug_block {
    void *size_padding_unit[RECEIVER_CEIL(RECEIVER_DEBUG_MSG_LEN, Q_PTR_SIZE)];
} Receiver_Debug_block;

/**
******************************************************************************
                    Receiver_debug_ constructor
    call Receiver_ctor() inside
******************************************************************************
**/
void Receiver_debug_ctor(Receiver *me);

#endif  /* receiver_debug_h_ */
