#ifndef queue_h_
#define queue_h_

/**
******************************************************************************
                                Ringbuffer
    Here, we always store the data in the void *sto which might cause some
wasted.
    If we want to store a byte in the the *ring, then the size of
sizeof(void *) - sizeof(unsigned char) is wasted.
******************************************************************************
**/

/**
******************************************************************************
                                Helper macro
******************************************************************************
**/
#define CQ_CAST(type_, unit_) ((type_)(unit_))

/**
******************************************************************************
                            Ringbuffer struct
******************************************************************************
**/
typedef struct CircularQueue {
    void *ring;  // point to qsto
    uint16_t head;  // insert here
    uint16_t tail;  // delete here
    uint16_t q_len;  // buffer length
    uint16_t n_used;  // real time content
} CircularQueue;

/**
******************************************************************************
                            Check macros
******************************************************************************
**/
#define CircularQueue_is_empty(me_) \
    (((me_)->n_used == 0U)  \
        ? (1U)  \
        : (0U))
#define CircularQueue_is_full(me_) \
    (((me_)->n_used >= (me_)->q_len)  \
        ? (1U)  \
        : (0U))


/**
******************************************************************************
                            Setters and getters
******************************************************************************
**/


/**
******************************************************************************
                                Static init
    void *ringbuffer_sto[RINGBUFFER_SIZE];
    CircularQueue_init(p_ringbuffer,
        (void *)ringbuffer_sto,
        RINGBUFFER_SIZE);
******************************************************************************
**/
void CircularQueue_init(CircularQueue *me,
    void *q_sto,
    uint16_t q_len);

/**
******************************************************************************
    There's no need to check full in terms of machanism
    Putting an uint16_t digit into the ringbuffer:
        uint16_t tmp = 23333U;
        CircularQueue_put(p_ringbuffer,
            &tmp);
******************************************************************************
**/
void CircularQueue_put(CircularQueue *me,
    void *elem_put);

/**
******************************************************************************
                        No empty protection inside
    visiting is safe at anytime in the view of memory accessing.
    To get the right content of the ringbuffer, must ensure the queue is not
empty before accessing the content.
    Getting an uint16_t digit from the ringbuffer:
        uint16_t tmp;
        CircularQueue_get(p_ringbuffer,
            &tmp);
******************************************************************************
**/
void CircularQueue_get(CircularQueue *me,
    void *elem_get);

#endif  /* queue_h_ */
