#ifndef printer_h_
#define printer_h_

/**
******************************************************************************
                                    ms
******************************************************************************
**/
#define PRINTER_TICKS_PER_SEC  (10U)

/**
******************************************************************************
                                Printer class
******************************************************************************
**/
typedef struct Printer {
    M_Fsm super;

    /* the static memory pool ... */
    QMPool frame_pool;
    /* the print queue, which is circular */
    CircularQueue tran_queue;
    /* the frame pointer used in the FSM and the ISR */
    uint8_t *tran_frame;
    /* the count that indicates the current remaining tran byte */
    uint8_t tran_frame_cnt;
    /* the event flag that ISR comm with the application layer FSM */
    uint8_t ISR_frame_tran_is_done;

    /* polymorphism attrs --------------------------------------------------*/
    /* the uart idx that the AO uses, to achieve param polymorphism */
    uint8_t printer_idx;
    /* static frame length of printer */
    uint8_t printer_msg_len;
} Printer;

/**
******************************************************************************
                            Printer states
******************************************************************************
**/
enum Printer_State {
    Printer_idle,
    Printer_busy,

    PRINTER_MAX_STATE
};

/**
******************************************************************************
                            Public used
******************************************************************************
**/
void Printer_ctor(Printer *me,
    uint8_t *frame_pool_sto,
    uint8_t frame_pool_size,
    uint8_t *tran_queue_sto,
    uint8_t tran_queue_size,
    uint8_t printer_idx,
    uint8_t printer_msg_len);
void Printer_run(Printer *me);
void Printer_Fsm_Run(Printer *me);
/**
* Handler in the ISR.
*/
void Printer_ISR_call(Printer *me);

/**
* Memory copy function with length limit...
*/
void Printer_memcpy(uint8_t *from,
    uint8_t *to,
    uint8_t length);

/**
******************************************************************************
                            Printer tran_queue
******************************************************************************
**/
#define PRINTER_EN_QUEUE(me_, enQueueElmt_) do { \
    CIRQUEUE_ENQ_U8P(&(me_)->tranQueue, \
        (uint8_t *)(enQueueElmt_)); \
} while (0)

#define PRINTER_DE_QUEUE(me_) do { \
    CIRQUEUE_DEQ_U8P(&(me_)->tranQueue, \
        &(me_)->tranFrame); \
} while (0)

#define PRINTER_QUEUE_EMPTY(me_)  \
    CIRQUEUE_EMPTY(&(me_)->tranQueue)

/**
******************************************************************************
                            Polymorphism here
    Implementation: bsp.c
******************************************************************************
**/
/**
* Load data to the buffer due to the uartNum.
*
* Implementation depends on the chip peripheral.
*
* param uint8_t frameSizePreSet as a setting value.
*/
void Printer_load_data2uart(Printer *me);

/**
* Fill the tran queue of specific Printer AO ...
*/
void Printer_printf(Printer *me,
    uint8_t *message);

#endif
