/**
******************************************************************************
                                Includes
    Need:
    stdint.h
    stdbool.h
******************************************************************************
**/
#include "m_fsm_port.h"
#include "m_fsm.h"
#include "qmpool.h"
#include "queue.h"
#include "printer.h"

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
    uint8_t printer_msg_len)
{
    // init super
    M_Fsm_ctor(&me->super,
        PRINTER_TICKS_PER_SEC,
        Printer_idle);

    // init static attrs
    me->tran_frame = (uint8_t *)0;
    me->tran_frame_cnt = 0;
    me->ISR_frame_tran_is_done = 0x00;

    // init dynamic attrs
    QMPool_init(&me->frame_pool,
        frame_pool_sto,
        frame_pool_size,
        frame_pool_unit_size);
    CircularQueue_init(&me->tran_queue,
        tran_queue_sto,
        tran_queue_size);

    // init polymorphism
    me->printer_idx = printer_idx;
    me->printer_msg_len = printer_msg_len;

    return;
}
/*..........................................................................*/
void Printer_run(Printer *me) {
    if (M_Fsm_run_is_ready(&me->super)) {
        M_Fsm_run_clear(&me->super);
        Printer_Fsm_Run(me);
    }

    return;
}
/*..........................................................................*/
// The state machine.
void Printer_Fsm_Run(Printer *me) {
    switch (me->super.state) {
        case Printer_idle: {
            entry_exec_start_(&me->super)
                me->super.ticks = 0;
            entry_exec_end_(&me->super)

            state_process_begin_(me)
            M_ticks_run_period(me->super.ticks,
                M_TICKS_PERIOD);

            M_INT_DISABLE();
            if (!PRINTER_QUEUE_EMPTY(me)) {
                /**
                * The length is public ...
                * Or maybe customized...
                */
                me->tran_frame_cnt = me->printer_msg_len;

                // fill me->tran_frame
                PRINTER_DE_QUEUE(me);

                /**
                * Load the data to the UART that current Printer AO uses ...
                */
                Printer_data_upload(me);

                --me->tran_frame_cnt;

                state_tran(Printer_busy);
            }
            M_INT_ENABLE();
            state_process_end_(me)

            exit_exec_start_(&me->super)
            exit_exec_end_(&me->super)

            break;
        }
        case Printer_busy: {
            entry_exec_start_(&me->super)
            entry_exec_end_(&me->super)

            state_process_begin_(me)
            /* Take the flag as an event here */
            if (me->ISR_frame_tran_is_done) {
                me->ISR_frame_tran_is_done = 0x00;
                /* recv the event and tran the state */
                state_tran(Printer_idle);
            }
            state_process_end_(me)

            exit_exec_start_(&me->super)
            exit_exec_end_(&me->super)

            break;
        }
        default: {
            break;
        }
    }

    return;
}
/*..........................................................................*/
void Printer_ISR_call(Printer *me) {
    /* more than one data, can enter the ISR again. */
    if (me->printer_msg_len > 1U) {
        if (me->tran_frame_cnt != 0) {
            /**
            * Load the data to the UART that current Printer AO uses ...
            */
            Printer_data_upload(me);

            /* minus the count of tran */
            --me->tran_frame_cnt;
            if (me->tran_frame_cnt == 0U) {
                /* execute the gc procedure */
                QMPool_put(&me->frame_pool,
                    (void *)me->tran_frame);

                /**
                * Set the flag to tell the AO to tran the state...
                */
                me->ISR_frame_tran_is_done = 0xFF;
            }
        }
    }
    // frame length is 1
    else {
        /* execute the gc procedure */
        QMPool_put(&me->frame_pool,
            (void *)me->tran_frame);

        /**
        * Set the flag to tell the AO to tran the state...
        */
        me->ISR_frame_tran_is_done = 0xFF;
    }

    return;
}
/*..........................................................................*/
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
    uint8_t *message)
{
    uint8_t *frame;

    /**
    * Get the frame from pool.
    */
    frame = (uint8_t *)QMPool_get(&me->frame_pool);

    // if valid
    if (frame) {
        // copy printf msg to frame asigned
        Printer_memcpy(message, /* from */
            frame, /* to */
            me->printer_msg_len); /* limit length */

        /**! Fill the frame into the tranqueue. */
        PRINTER_EN_QUEUE(me,
            frame);
    }

    return;
}
