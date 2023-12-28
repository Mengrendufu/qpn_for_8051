#ifndef cpu_port_h_
#define cpu_port_h_

/**
******************************************************************************
                            Keil C51
                            BYD7615CMxx
******************************************************************************
**/


/**
******************************************************************************
                    Exact-width types and Boolean type.
    The framework use xint_fastx_t. In C51, the stdint.h do not provide
such types.

* @note If C99 or later standard is available, do this directly:
    #include "stdint.h"
    #include "stdbool.h"
******************************************************************************
**/
typedef unsigned char bool;
enum STAND_BOOL_ {
    false = (bool)(0U),
    true  = (bool)(1U)
};

// fast, fast, fast
typedef signed char int8_t;
typedef signed char int_fast8_t;
typedef unsigned char uint8_t;
typedef unsigned char uint_fast8_t;

typedef signed int int16_t;
typedef signed int int_fast16_t;
typedef unsigned int uint16_t;
typedef unsigned int uint_fast16_t;

typedef signed long int int32_t;
typedef signed long int int_fast32_t;
typedef unsigned long int uint32_t;
typedef unsigned long int uint_fast32_t;

/**
******************************************************************************
                                sizeof(void *)
******************************************************************************
**/
#define Q_PTR_SIZE 3U

/**
******************************************************************************
                        Ceilling width of pointer type
******************************************************************************
**/
#if   (Q_PTR_SIZE == 8U)
    #define PTR_SIZE_PADDING_TYPE uint64_t
#elif (Q_PTR_SIZE == 7U)
    #define PTR_SIZE_PADDING_TYPE uint64_t
#elif (Q_PTR_SIZE == 6U)
    #define PTR_SIZE_PADDING_TYPE uint64_t
#elif (Q_PTR_SIZE == 5U)
    #define PTR_SIZE_PADDING_TYPE uint64_t
#elif (Q_PTR_SIZE == 4U)
    #define PTR_SIZE_PADDING_TYPE uint32_t
#elif (Q_PTR_SIZE == 3U)
    #define PTR_SIZE_PADDING_TYPE uint32_t
#elif (Q_PTR_SIZE == 2U)
    #define PTR_SIZE_PADDING_TYPE uint16_t
#elif (Q_PTR_SIZE == 1U)
    #define PTR_SIZE_PADDING_TYPE uint8_t
#else
    #error "Q_PTR_SIZE set error!"
#endif

/**
******************************************************************************
                            Dangerous operations
******************************************************************************
**/
#define Q_UINT2PTR(p_type_, uint_)  ((p_type_)(uint_))
#define Q_PTR2UINT(ptr_)          ((PTR_SIZE_PADDING_TYPE)(ptr_))

/*
******************************************************************************
                                system
******************************************************************************
*/
#include "Config.h"

/**
******************************************************************************
                        interrupt disable/enable
******************************************************************************
**/
#define CPU_INT_DISABLE()  \
do {  \
    EA = 0;  \
} while (false)

#define CPU_INT_ENABLE()  \
do {  \
    EA = 1;  \
} while (false)

/**
******************************************************************************
    How many uarts the system have?
    Enum them and use in the Printer_ctor, Printer_data_upload
******************************************************************************
**/
enum System_Idx_Uart {
    System_Idx_Uart_0 = (uint8_t)0U,
    System_Idx_Uart_1,
    System_Idx_Uart_2,

    MAX_PRINTER_IDX_UART
};

#endif
