#ifndef printer_h_
#define printer_h_

/**
******************************************************************************
                                Helper macros
******************************************************************************
**/
#define PRINTER_CEIL(x, y)  \
    (((x) - 1U) / (y) + 1U)
#define PRINTER_DIM(array_)  \
    (sizeof(array_) / sizeof((array_)[0]))

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
    Printer_idle,  // init state
    Printer_busy,

    PRINTER_MAX_STATE
};

/**
******************************************************************************
                            Public used
******************************************************************************
**/
void Printer_ctor(Printer *me,
    void *frame_pool_sto,
    uint8_t frame_pool_size,
    uint8_t frame_pool_unit_size,
    void *tran_queue_sto,
    uint8_t tran_queue_size,
    uint8_t printer_idx,
    uint8_t printer_msg_len);
void Printer_run(Printer *me);
void Printer_Fsm_Run(Printer *me);
/**
******************************************************************************
                            Handler in the ISR.
    called:
        // uart tx isr ...
        Printer_ISR_call(&Printer_debug);
        // uart tx isr ...
******************************************************************************
**/
void Printer_ISR_call(Printer *me);

/**
* Memory copy function with length limit...
*/
void Printer_memcpy(uint8_t *from,
    uint8_t *to,
    uint8_t length);

/**
* Fill the tran queue of specific Printer AO ...
*/
void Printer_printf(Printer *me,
    uint8_t *message);

/**
******************************************************************************
                            Printer tran_queue
    Choose a queue class to use.
******************************************************************************
**/
/**
******************************************************************************
    Trace of allocated frame: QMPool ==> fill content ==> CircularQueue
                                                ^
                                    user msg____|
******************************************************************************
**/
#define PRINTER_EN_QUEUE(me_,  \
    alloc_frame_)  \
do {  \
    CircularQueue_put(&((me_)->tran_queue),  \
        &(alloc_frame_));  \
} while (0)
/**
******************************************************************************
    Get frame: CircularQueue ==> (me_)->tran_frame
******************************************************************************
**/
#define PRINTER_DE_QUEUE(me_)  \
do {  \
    CircularQueue_get(&((me_)->tran_queue),  \
        &((me_)->tran_frame));  \
} while (0)

#define PRINTER_QUEUE_EMPTY(me_)  \
    CircularQueue_is_empty(&((me_)->tran_queue))

/**
******************************************************************************
                            Polymorphism here
    Implementation: bsp.c
******************************************************************************
**/
/**
* Load data to the buffer due to the me->printer_idx.
*
* Implementation depends on the chip peripheral.
*/
void Printer_data_upload(Printer *me);

#endif
