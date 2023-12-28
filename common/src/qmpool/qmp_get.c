/**
******************************************************************************
                                Include files
******************************************************************************
**/
#include "cpu_port.h"
#include "qmpool.h"

/**
* \file
* \ingroup qf
* \brief QMPool_get() and QF_getPoolMargin() implementation.
*/

/*==========================================================================*/
void *QMPool_get(QMPool * const me) {
    QFreeBlock *fb;

    fb = (QFreeBlock *)0; /* assume that the allocation is failed */

    fb = (QFreeBlock *)me->free_head;           /* get a free block or NULL */
    if (fb != (QFreeBlock *)0) {                   /* free block available? */
        me->free_head = fb->next;/* adjust list head to the next free block */

        --me->nFree;                                 /* one less free block */
        if (me->nMin > me->nFree) {
            me->nMin = me->nFree;            /* remember the minimum so far */
        }
    }

    return fb;            /* return the block or NULL pointer to the caller */
}
