#ifndef QVN_H
#define QVN_H

/****************************************************************************/
/*! Ready set of QV-nano. */
// extern uint_fast8_t volatile QV_readySet_;

/*! QV idle callback (customized in BSPs for QK)
*
* QV_onIdle() is called continuously by the QV-nano scheduler. This callback
* gives the application an opportunity to enter a power-saving CPU mode,
* or perform some other idle processing.
*
* @note QV_onIdle() is invoked with interrupts disabled, but must return
* with interrupts enabled. This is in contrast to the callback QK_onIdle(),
* which is used by the preemptive QK-nano scheduler.
*/
void QV_onIdle(void);

#endif /* QVN_H */
