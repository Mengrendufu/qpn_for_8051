#ifndef receiver_h_
#define receiver_h_

/**
******************************************************************************
                                Helper macros
******************************************************************************
**/
#define RECEIVER_CEIL(x, y)  \
    (((x) - 1U) / (y) + 1U)
#define RECEIVER_DIM(array_)  \
    (sizeof(array_) / sizeof((array_)[0]))

/**
******************************************************************************
                                    ms
******************************************************************************
**/
#define RECEIVER_RUN_TICKS_PER_SEC  10U

/**
******************************************************************************
                                    ms
    Used to judge if the comm of receiver is lost.
******************************************************************************
**/
#define RECEIVER_COMM_LOST_TIME_TICKS  1000U

/**
******************************************************************************
                                Receiver class
******************************************************************************
**/
typedef struct Receiver {
    M_Fsm super;

    /**
    **************************************************************************
                                        dynamic
    **************************************************************************
    **/
    QMPool *frame_pool;
    CircularQueue *recv_queue;

    /**
    **************************************************************************
                                    static
    **************************************************************************
    **/
    uint8_t *recv_frame;
    uint8_t recv_frame_cnt;

    /**
    **************************************************************************
                                    ISR call
    **************************************************************************
    **/
    uint8_t ISR_call_state;
    uint8_t ISR_state_entry_is_ready;
    uint8_t ISR_recv_data;
    uint32_t yield_checksum;
    uint32_t recv_checksum;

    /**
    **************************************************************************
                                recv msg process
    **************************************************************************
    **/
    uint8_t *process_frame;
    uint8_t receiver_data_is_updated;
    uint8_t receiver_comm_is_lost;

    /**
    **************************************************************************
                                polymorphism section
    **************************************************************************
    **/
    uint8_t receiver_idx;
    uint8_t receiver_msg_len;
} Receiver;

/**
******************************************************************************
                Receiver states, application level and ISR level
******************************************************************************
**/

// application level
enum Receiver_State {
    /**
    **************************************************************************
            The system is idle, the communication is not turn on
    **************************************************************************
    **/
    Receiver_idle,

    /**
    **************************************************************************
                    The system is doing communication
    **************************************************************************
    **/
    Receiver_monitoring,

    /**
    ******************************************************************************
        (1) Long time no bytes come
        (2) other types communication error
    ******************************************************************************
    **/
    Receiver_monitor_capture_error,

    RECEIVER_MAX_STATE
};

// ISR level
enum Receiver_ISR_State {
    /**
    **************************************************************************
            Allocate recv_frame and check the code head receive.
    **************************************************************************
    **/
    Receiver_ISR_idle,

    /**
    **************************************************************************
                        Receive and check checksum
    **************************************************************************
    **/
    Receiver_ISR_receiving,

    /**
    **************************************************************************
        Filtering the data received until the code head appear again.
    **************************************************************************
    **/
    Receiver_ISR_byte_missing,

    /**
    *************************************************************************
        After recv code head in state Receiver_ISR_byte_missing, tran to
    this state, go on receiving the rest of code head.
    *************************************************************************
    **/
    Receiver_ISR_go_on_receiving,

    RECEIVER_ISR_MAX_STATE
};

/**
******************************************************************************
                                    Public
******************************************************************************
**/
void Receiver_ctor(Receiver *me,
    void *frame_pool,
    void *frame_pool_sto,
    uint16_t frame_pool_sto_size,
    uint8_t frame_pool_sto_block_size,
    void *recv_queue,
    void *recv_queue_sto,
    uint8_t recv_queue_sto_size,
    uint8_t receiver_idx,
    uint8_t receiver_msg_len);
void Receiver_ISR_call(Receiver *me);
void Receiver_run(Receiver *me);
void Receiver_Fsm_run(Receiver *me);

/**
******************************************************************************
    Check if the recv_queue is empty.
******************************************************************************
**/
#define RECEIVER_RECV_QUEUE_IS_EMPTY(me_)  \
    CircularQueue_is_empty((me_)->recv_queue)

#define RECEIVER_RECV_QUEUE_IS_FULL(me_)  \
    CircularQueue_is_full((me_)->recv_queue)

/**
******************************************************************************
    Put the recv_frame into recv_queue
******************************************************************************
**/
#define RECEIVER_EN_QUEUE(me_)  \
do {  \
    CircularQueue_put((me_)->recv_queue,  \
        &(me_)->recv_frame);  \
} while (false)

/**
******************************************************************************
    Get a frame from recv_queue to process_frame
******************************************************************************
**/
#define RECEIVER_DE_QUEUE(me_)  \
do {  \
    CircularQueue_get((me_)->recv_queue,  \
        &(me_)->process_frame);  \
} while (false)

/**
******************************************************************************
    Get a frame from frame_pool to recv_frame
******************************************************************************
**/
#define RECEIVER_FRAME_GET(me_)  \
do {  \
    (me_)->recv_frame = (uint8_t *)QMPool_get((me_)->frame_pool);  \
} while (false)

#define RECEIVER_FRAME_GC(me_)  \
do {  \
    QMPool_put((me_)->frame_pool,  \
        (me_)->recv_frame);  \
} while (false)

/**
******************************************************************************
                            polymorphism handlers
******************************************************************************
**/


/**
******************************************************************************
                                application
******************************************************************************
**/

void Receiver_received_frame_process(Receiver *me);

/**
******************************************************************************
                                    isr
******************************************************************************
**/

void Receiver_data_download(Receiver *me);

void Receiver_recv_code_head_process(Receiver *me);
uint8_t Receiver_recv_is_code_head(Receiver *me);

void Receiver_init_checksum(Receiver *me);
void Receiver_yiled_checksum(Receiver *me);
uint8_t Receiver_checksum_is_same(Receiver *me);

#endif  /* receiver_h_ */
