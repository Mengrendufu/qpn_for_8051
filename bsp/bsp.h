#ifndef BSP_H
#define BSP_H

/**
******************************************************************************
    The frequency of the QF_tick, unit is Hz. Used this to init the system
timer to post timeout events.
******************************************************************************
**/
#define BSP_TICKS_PER_SEC  1000U

void BSP_init(void);
void BSP_ledOn(void);
void BSP_ledOff(void);

/**
******************************************************************************
                            M_Fsms or others
******************************************************************************
**/
#include "printer_debug.h"

#endif /* BSP_H */
