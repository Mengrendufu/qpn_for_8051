/**
******************************************************************************
                                    Includes
******************************************************************************
**/
#include "m_fsm_port.h"
#include "qmpool.h"
#include "queue.h"
#include "receiver.h"

/**
******************************************************************************
                                Implementations
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
    uint8_t receiver_msg_len)
{
    // super class
    M_Fsm_ctor(&me->super,
        RECEIVER_RUN_TICKS_PER_SEC,
        Receiver_idle);

    // solid
    QMPool_init(frame_pool,
        frame_pool_sto,
        frame_pool_sto_size,
        frame_pool_sto_block_size);
    me->frame_pool = frame_pool;

    CircularQueue_init(recv_queue,
        recv_queue_sto,
        recv_queue_sto_size);
    me->recv_queue = recv_queue;

    // static attrs::basic
    me->recv_frame = (uint8_t *)0;
    me->recv_frame_cnt = 0;

    // static attrs::isr_call
    me->ISR_call_state = Receiver_ISR_idle;
    me->ISR_state_entry_is_ready = 0xFF;
    me->ISR_recv_data = 0x00;
    me->yield_checksum = 0x00000000;
    me->recv_checksum = 0x00000000;

    // static attrs::recv_msg_process
    me->process_frame = (uint8_t *)0;
    me->receiver_data_is_updated = 0x00;
    me->receiver_comm_is_lost = 0x00;

    // polymorphism
    me->receiver_idx = receiver_idx;
    me->receiver_msg_len = receiver_msg_len;

    return;
}
/*..........................................................................*/
void Receiver_ISR_call(Receiver *me) {
    /**
    **************************************************************************
                                    update flag
    **************************************************************************
    **/
    me->receiver_data_is_updated = 0xFF;

    // store data input
    Receiver_data_download(me);

    switch (me->ISR_call_state) {
        case Receiver_ISR_idle: {
            // state entry
            if (me->ISR_state_entry_is_ready) {
                // one shot
                me->ISR_state_entry_is_ready = 0x00;
                // checksum init
                Receiver_init_checksum(me);
                // allocate frame
                RECEIVER_FRAME_GET(me);
                // recv count reset
                me->recv_frame_cnt = me->receiver_msg_len;
            }

            /**
            ******************************************************************
                If the frame allocation is not good, me->ISR_call_state
            will never leave Receiver_ISR_idle state.
                So ONLY check the me->recv_frame in the Receiver_ISR_idle
            state.
            ******************************************************************
            **/
            if (me->recv_frame) {
                // download data to recv frame
                me->recv_frame[me->receiver_msg_len - me->recv_frame_cnt] =
                    me->ISR_recv_data;
                // download ==> adjust recv cnt left.
                --me->recv_frame_cnt;
                // yield checksum
                Receiver_yiled_checksum(me);
                // code head check
                Receiver_recv_code_head_process(me);
            }
            else {
                // back to idle state, until frame allocation successful.
                me->ISR_state_entry_is_ready = 0xFF;
                me->ISR_call_state = Receiver_ISR_idle;
            }

            break;
        }
        case Receiver_ISR_receiving: {
            // download data to recv frame
            me->recv_frame[me->receiver_msg_len - me->recv_frame_cnt] =
                me->ISR_recv_data;
            // download ==> adjust recv cnt left.
            --me->recv_frame_cnt;
            // yield the checksum, put the condition inside.
            Receiver_yiled_checksum(me);

            // when recv a frame done.
            if (me->recv_frame_cnt == 0U) {
                // when two checksums compare good
                if (Receiver_checksum_is_same(me)) {
                    /**
                    **********************************************************
                        How to deal with the recv frame at this point
                    **********************************************************
                    **/
                    // when recv_queue is not full
                    if (!RECEIVER_RECV_QUEUE_IS_FULL(me)) {
                        // put recv_frame into recv_queue
                        RECEIVER_EN_QUEUE(me);
                    }
                    // when recv_queue is full
                    else {
                        // abandon the recv_frame, gc it.
                        RECEIVER_FRAME_GC(me);
                    }

                    // back to idle
                    me->ISR_state_entry_is_ready = 0xFF;
                    me->ISR_call_state = Receiver_ISR_idle;
                }
                // when checksum is not good.
                else {
                    // abandon the recv_frame, gc it.
                    RECEIVER_FRAME_GC(me);

                    // back to idle
                    me->ISR_state_entry_is_ready = 0xFF;
                    me->ISR_call_state = Receiver_ISR_idle;
                }
            }

            break;
        }
        case Receiver_ISR_byte_missing: {
            // when current byte is the code head again.
            if (Receiver_recv_is_code_head(me)) {
                // checksum init
                Receiver_init_checksum(me);
                // recv count reset
                me->recv_frame_cnt = me->receiver_msg_len;

                // download data to recv frame
                me->recv_frame[me->receiver_msg_len - me->recv_frame_cnt] =
                    me->ISR_recv_data;
                // download ==> adjust recv cnt left.
                --me->recv_frame_cnt;
                // yield checksum
                Receiver_yiled_checksum(me);

                // go on recv the reset of code head
                me->ISR_state_entry_is_ready = 0xFF;
                me->ISR_call_state = Receiver_ISR_go_on_receiving;
            }

            break;
        }
        case Receiver_ISR_go_on_receiving: {
            // download data to recv frame
            me->recv_frame[me->receiver_msg_len - me->recv_frame_cnt] =
                me->ISR_recv_data;
            // download ==> adjust recv cnt left.
            --me->recv_frame_cnt;
            // yield checksum
            Receiver_yiled_checksum(me);
            // code head check
            Receiver_recv_code_head_process(me);

            break;
        }
        default: {
            break;
        }
    }

    return;
}
/*..........................................................................*/
void Receiver_run(Receiver *me) {
    if (M_Fsm_run_is_ready(&me->super)) {
        M_Fsm_run_clear(&me->super);
        Receiver_Fsm_run(me);
    }

    return;
}
/*..........................................................................*/
void Receiver_Fsm_run(Receiver *me) {
    /**
    **************************************************************************
                            Received frame process
    **************************************************************************
    **/
    M_INT_DISABLE();  // enter critical section
    if (!RECEIVER_RECV_QUEUE_IS_EMPTY(me)) {  // msg updated
        RECEIVER_DE_QUEUE(me);  // update processs_frame

        Receiver_received_frame_process(me);  // handle msg

        // frame processed, gc
        QMPool_put(me->frame_pool,
            me->process_frame);
    }
    M_INT_ENABLE();  // quit critical section

    switch (me->super.state) {
        case Receiver_idle: {
            M_FSM_ENTRY_EXEC_START_(&me->super)
                me->super.ticks = 0U;
                me->receiver_data_is_updated = 0x00;
                me->receiver_comm_is_lost = 0x00;
            M_FSM_ENTRY_EXEC_END_(&me->super)

            M_FSM_STATE_PROCESS_BEGIN_(&me->super)
            // state ticks
            M_ticks_run_period(me->super.ticks,
                M_TICKS_PERIOD);

            // When to trigger receiver monitor
            M_INT_DISABLE();
            if (me->receiver_data_is_updated) {  // has recv data
                me->receiver_data_is_updated = 0x00;  // clear
                M_Fsm_state_tran(Receiver_monitoring);  // trigger monitor
            }
            M_INT_ENABLE();
            M_FSM_STATE_PROCESS_END_(&me->super)

            M_FSM_EXIT_EXEC_START_(&me->super)
            M_FSM_EXIT_EXEC_END_(&me->super)
            break;
        }
        case Receiver_monitoring: {
            M_FSM_ENTRY_EXEC_START_(&me->super)
                me->super.ticks = 0U;
                me->receiver_data_is_updated = 0x00;
                me->receiver_comm_is_lost = 0x00;
            M_FSM_ENTRY_EXEC_END_(&me->super)

            M_FSM_STATE_PROCESS_BEGIN_(&me->super)
            // state ticks
            M_ticks_run_period(me->super.ticks,
                M_TICKS_PERIOD);
            // when we has data download
            M_INT_DISABLE();
            if (me->receiver_data_is_updated) {  // has recv data
                me->receiver_data_is_updated = 0x00;  // clear
                me->super.ticks = 0U;  // clear monitor ticks
            }
            M_INT_ENABLE();

            // when long long time no data download
            if (me->super.ticks >=
                    RECEIVER_COMM_LOST_TIME_TICKS
                        / RECEIVER_RUN_TICKS_PER_SEC)
            {
                me->receiver_comm_is_lost = 0xFF; // set the flag true

                // what to do about alerting the comm lost error
                // ...

                M_Fsm_state_tran(Receiver_monitor_capture_error);
            }
            M_FSM_STATE_PROCESS_END_(&me->super)

            M_FSM_EXIT_EXEC_START_(&me->super)
            M_FSM_EXIT_EXEC_END_(&me->super)
            break;
        }
        case Receiver_monitor_capture_error: {
            M_FSM_ENTRY_EXEC_START_(&me->super)
            M_FSM_ENTRY_EXEC_END_(&me->super)

            M_FSM_STATE_PROCESS_BEGIN_(&me->super)
            // when to quit the error state
            M_INT_DISABLE();
            if (me->receiver_data_is_updated) {
                me->receiver_data_is_updated = 0x00;
                me->receiver_comm_is_lost = 0x00;

                M_Fsm_state_tran(Receiver_monitoring);
            }
            M_INT_ENABLE();
            M_FSM_STATE_PROCESS_END_(&me->super)

            M_FSM_EXIT_EXEC_START_(&me->super)
            M_FSM_EXIT_EXEC_END_(&me->super)
            break;
        }
        default: {
            break;
        }
    }

    return;
}
