#ifndef common_h_
#define common_h_

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
#include "stdint_.h"
#include "stdbool_.h"


/**
******************************************************************************
                        Ceilling width of pointer type
******************************************************************************
**/
// default, take it a pointer 32-bit width
#ifndef Q_PTR_SIZE_PADDING_TYPE
    #define Q_PTR_SIZE_PADDING_TYPE 4U
#endif

#if   (Q_PTR_SIZE_PADDING_TYPE == 8U)
    #define PTR_SIZE_PADDING_TYPE uint64_t
#elif (Q_PTR_SIZE_PADDING_TYPE == 7U)
    #define PTR_SIZE_PADDING_TYPE uint64_t
#elif (Q_PTR_SIZE_PADDING_TYPE == 6U)
    #define PTR_SIZE_PADDING_TYPE uint64_t
#elif (Q_PTR_SIZE_PADDING_TYPE == 5U)
    #define PTR_SIZE_PADDING_TYPE uint64_t
#elif (Q_PTR_SIZE_PADDING_TYPE == 4U)
    #define PTR_SIZE_PADDING_TYPE uint32_t
#elif (Q_PTR_SIZE_PADDING_TYPE == 3U)
    #define PTR_SIZE_PADDING_TYPE uint32_t
#elif (Q_PTR_SIZE_PADDING_TYPE == 2U)
    #define PTR_SIZE_PADDING_TYPE uint16_t
#elif (Q_PTR_SIZE_PADDING_TYPE == 1U)
    #define PTR_SIZE_PADDING_TYPE uint8_t
#else
    #error "Q_PTR_SIZE_PADDING_TYPE set error!"
#endif


/**
******************************************************************************
                            QP memory pool
    The system event pool of system use qmpool.
    User application may also use qmpool.
******************************************************************************
**/
#include "qmpool.h"


/**
******************************************************************************
                Helper macros for simplifying the application codes
******************************************************************************
**/
#include "helper_macros.h"


#endif
