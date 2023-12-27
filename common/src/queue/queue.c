/**
******************************************************************************
                                Include files
    Type dependencies.
******************************************************************************
**/
#include "stdint_.h"
#include "queue.h"

/**
******************************************************************************
                                Implementations
******************************************************************************
**/
void CircularQueue_init(CircularQueue *me,
    void *q_sto,
    uint16_t q_len)
{
    me->ring   = q_sto;
    me->head   = 0U;
    me->tail   = 0U;
    me->q_len  = q_len;
    me->n_used = 0U;

    return;
}
/*..........................................................................*/
void CircularQueue_put(CircularQueue *me,
    void *elem_put)
{
    /* Element input -------------------------------------------------------*/
    *(CQ_CAST(void **, me->ring) + me->head)
        = *CQ_CAST(void **, elem_put);

    /* count adjust */
    if (me->head == 0U) {
        me->head = me->q_len;
    }
    --me->head;

    /* not full */
    if (me->n_used < me->q_len) {
        ++me->n_used;
    }
    /* overflow */
    else {
        if (me->tail == 0U) {
            me->tail = me->q_len;
        }
        --me->tail;
    }

    return;
}
/*..........................................................................*/
void CircularQueue_get(CircularQueue *me,
    void *elem_get)
{
    /* Element output ------------------------------------------------------*/
    *CQ_CAST(void **, elem_get)
        = *(CQ_CAST(void **, me->ring) + me->tail);

    if (me->tail == 0U) {
        me->tail = me->q_len;
    }
    --me->tail;
    --me->n_used;

    return;
}
