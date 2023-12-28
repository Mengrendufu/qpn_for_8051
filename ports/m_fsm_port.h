#ifndef m_fsm_port_h_
#define m_fsm_port_h_

/**
******************************************************************************
                                m_fsm_port
******************************************************************************
**/

#include "cpu_port.h"

#define M_INT_DISABLE()  \
do {  \
    CPU_INT_DISABLE();  \
} while (false)

#define M_INT_ENABLE()  \
do {  \
    CPU_INT_ENABLE();  \
} while (false)

#endif
