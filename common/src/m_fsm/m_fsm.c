/**
******************************************************************************
                                Includes
******************************************************************************
**/
#include "stdint_.h"
#include "queue.h"
#include "m_fsm.h"

/**
******************************************************************************
                                Handlers
******************************************************************************
**/
void M_Fsm_ctor(M_Fsm * const me,
    M_Ticks run_ticks,
    M_State initial_state)
{
    me->run_ticks = run_ticks;
    me->state = initial_state;

    me->entry_is_ready = 0xFF;
    me->exit_is_ready  = 0x00;

    me->ticks = 0;
    me->run_is_ready = 0x00;

    return;
}
/*..........................................................................*/
bool M_Fsm_post(M_Evt *evt,
    M_Signal signal)
{
    bool can_post;
    /* assume */

    can_post = false;

    /* do not cover */
    if (!M_Evt_is_updated(evt)) {
        can_post = true;

        M_Evt_sig(evt) = signal;
        M_Evt_post(evt);
    }

    return can_post;
}
