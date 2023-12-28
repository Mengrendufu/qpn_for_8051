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
* \brief QMPool_put() implementation.
*/

/*..........................................................................*/
void QMPool_put(QMPool * const me,
    void *b)
{
    ((QFreeBlock *)b)->next = (QFreeBlock *)me->free_head;/* link into list */
    me->free_head = b; /* set as new head of the free list */
    ++me->nFree; /* one more free block in this pool */

    return;
}
