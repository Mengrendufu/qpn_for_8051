/**
******************************************************************************
                                Include files
******************************************************************************
**/
#include "stdint_.h"
#include "qmpool.h"

/**
* \file
* \ingroup qf
* \brief QMPool_init() implementation.
*/

/*..........................................................................*/
void QMPool_init(QMPool * const me,
    void * const poolSto,
    uint32_t poolSize,
    QMPoolSize blockSize)
{
    QFreeBlock *fb;
    uint32_t nblocks;

    me->free_head = poolSto;

    /* round up the blockSize to fit an integer # free blocks, no division */
    me->blockSize = (QMPoolSize)sizeof(QFreeBlock); /* start with just one */
    nblocks = (uint32_t)1; /* # free blocks that fit in one memory block */
    while (me->blockSize < blockSize) {
        me->blockSize += (QMPoolSize)sizeof(QFreeBlock);
        ++nblocks;
    }
    blockSize = me->blockSize; /* use the rounded-up value from now on */

    /* chain all blocks together in a free-list... */
    poolSize -= (uint32_t)blockSize; /* don't count the last block */
    me->nTot  = (QMPoolCtr)1; /* the last block already in the pool */
    fb = (QFreeBlock *)me->free_head; /* start at the head of the free list */
    while (poolSize >= (uint32_t)blockSize) {
        fb->next = &QF_PTR_AT_(fb, nblocks);/*point next link to next block */
        fb = fb->next; /* advance to the next block */
        poolSize -= (uint32_t)blockSize; /* reduce the available pool size */
        ++me->nTot; /* increment the number of blocks so far */
    }

    fb->next  = (QFreeBlock *)0; /* the last link points to NULL */
    me->nFree = me->nTot; /* all blocks are free */
    me->nMin  = me->nTot; /* the minimum number of free blocks */
    me->start = poolSto; /* the original start this pool buffer */
    me->end   = fb; /* the last block in this pool */

    return;
}
