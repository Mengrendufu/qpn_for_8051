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
            some more application modules in addition to AOs
******************************************************************************
**/
// #include "debug_printer.h"

#endif /* BSP_H */
