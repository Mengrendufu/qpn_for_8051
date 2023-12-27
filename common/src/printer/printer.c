/**
******************************************************************************
                                Includes
******************************************************************************
**/
#include "stdint_.h"
#include "queue.h"
#include "qmpool.h"
#include "m_fsm.h"
#include "printer.h"

/*==========================================================================*/
/**
* The ugly memory function of Mengrendufu ...
*/
void Printer_memcpy(uint8_t *from,
    uint8_t *to,
    uint8_t length)
{
    uint8_t i;
    for (i = 0; i < length; ++i) {
        *(to + i) = *(from + i);
    }

    return;
}
/*..........................................................................*/
void Printer_printf(Printer *me,
    uint8_t *message,
    uint8_t length)
{
    uint8_t *frame;

    /**
    * Get the frame from pool.
    */
    frame = (uint8_t *)QMPool_get(&me->framePool);

    /**
    * Fill the content.
    */
    if (frame) {
        Printer_memcpy(message, frame, length);
        /**! Fill the frame into the tranqueue. */
        PRINTER_EN_QUEUE(me, frame);
    }

    return;
}
/*..........................................................................*/
/**
* Just a shell.
*/
void Printer_run(Printer *me,
    uint8_t frameSizePreSet)
{
    if (M_Fsm_runIsReady(&me->super)) {
        M_Fsm_runClear(&me->super);
        Printer_Fsm_Run(me, frameSizePreSet);
    }

    return;
}
/*..........................................................................*/
/**
* The state machine.
*/
void Printer_Fsm_Run(Printer *me,
    uint8_t frameSizePreSet)
{
    switch (me->super.state) {
        case Printer_idle: {
            entryExecStart_(&me->super)
                me->super.ticks = 0;
            entryExecEnd_

            stateProcessBegin_
            M_TicksRunPeriod(me->super.ticks, MTICKS_PERIOD);

            M_INT_DISABLE();
            if (!PRINTER_QUEUE_EMPTY(me)) {
                /**
                * The length is public ...
                * Or maybe customized...
                */
                me->tranFrameCnt = frameSizePreSet;

                PRINTER_DE_QUEUE(me);

                /**
                * Load the data to the UART that current Printer AO uses ...
                */
                loadData2UartBuffer(me, frameSizePreSet);

                --me->tranFrameCnt;

                stateTran(Printer_busy);
            }
            M_INT_ENABLE();
            stateProcessEnd_

            exitExecStart_(&me->super)
            exitExecEnd_

            break;
        }
        case Printer_busy: {
            entryExecStart_(&me->super)
            entryExecEnd_

            stateProcessBegin_
            /* Take the flag as an event here */
            if (me->ISRFrameTranDone) {
                me->ISRFrameTranDone = 0x00;
                /* recv the event and tran the state */
                stateTran(Printer_idle);
            }
            stateProcessEnd_

            exitExecStart_(&me->super)
            exitExecEnd_

            break;
        }
        default: {
            break;
        }
    }

    return;
}
/*..........................................................................*/
/**
* Make it happen at the foreground.
*/
void Printer_ISR_call(Printer *me,
    uint8_t frameSizePreSet)
{
    /* more than one data, can enter the ISR again. */
    if (frameSizePreSet > 1U) {
        if (me->tranFrameCnt != 0) {
            /**
            * Load the data to the UART that current Printer AO uses ...
            */
            loadData2UartBuffer(me, frameSizePreSet);

            /* minus the count of tran */
            --me->tranFrameCnt;
            if (me->tranFrameCnt == 0U) {
                /* execute the gc procedure */
                QMPool_put(&me->framePool, (void *)me->tranFrame);

                /**
                * Set the flag to tell the AO to tran the state...
                */
                me->ISRFrameTranDone = 0xFF;
            }
        }
    }
    /* only one data to send, in and out */
    else {
        /* execute the gc procedure */
        QMPool_put(&me->framePool, (void *)me->tranFrame);

        /**
        * Set the flag to tell the AO to tran the state...
        */
        me->ISRFrameTranDone = 0xFF;
    }

    return;
}
