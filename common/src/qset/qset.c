/**
******************************************************************************
                                Includes
******************************************************************************
**/
// #include <stdint.h>
// #include <stdbool.h>
#include "cpu_port.h"
#include "qset.h"

/**
******************************************************************************
                                Implementations
******************************************************************************
**/
uint8_t QSet_log2(QSetBits const bitmask) {
    static uint8_t const log2LUT[16] = {
        0U,                             /* [0b0000] */
        1U,                             /* [0b0001] */
        2U, 2U,                         /* [0b001x] */
        3U, 3U, 3U, 3U,                 /* [0b01xx] */
        4U, 4U, 4U, 4U, 4U, 4U, 4U, 4U  /* [0b1xxx] */
    };
    uint8_t n;
    QSetBits tmp;
    QSetBits x_;

    n = 0U;
    x_ = bitmask;

#if (Q_SET_MAX > 16U)
    tmp = (x_ >> 16U);
    if (tmp != 0U) {
        n += 16U;
        x_ = tmp;
    }
#endif

#if (Q_SET_MAX > 8U)
    tmp = (x_ >> 8U);
    if (tmp != 0U) {
        n += 8U;
        x_ = tmp;
    }
#endif

    tmp = (x_ >> 4U);
    if (tmp != 0U) {
        n += 4U;
        x_ = tmp;
    }

    n += log2LUT[x_];

    return n;
}
/*..........................................................................*/
void QSet_set_empty(QSet * const me) {
    me->bits[0] = 0U;
#if Q_SET_MAX > 32U
    me->bits[1] = 0U;
#endif

    return;
}
/*..........................................................................*/
bool QSet_is_empty(QSet * const me) {
#if Q_SET_MAX <= 32U
    return (me->bits[0] == 0U);
#else
    return (me->bits[0] == 0U) ? (me->bits[1] == 0U) : false;
#endif
}
/*..........................................................................*/
bool QSet_is_not_empty(QSet * const me) {
#if Q_SET_MAX <= 32U
    return (me->bits[0] != 0U);
#else
    return (me->bits[0] != 0U) ? true: (me->bits[1] != 0U);
#endif
}
/*..........................................................................*/
bool QSet_has_element(QSet * const me,
    uint8_t const n)
{
#if Q_SET_MAX <= 32U
    return ((me->bits[0] & ((QSetBits)1U << (n - 1U))) != 0U);
#else
    return (n <= 32U)
        ? ((me->bits[0] & ((QSetBits)1U << (n - 1U))) != 0U)
        : ((me->bits[1] & ((QSetBits)1U << (n - 32U - 1U))) != 0U);
#endif
}
/*..........................................................................*/
void QSet_insert(QSet * const me,
    uint8_t const n)
{
#if Q_SET_MAX <= 32U
    me->bits[0] |= ((QSetBits)1U << (n - 1U));
#else
    if (n <= 32) {
        me->bits[0] |= ((QSetBits)1U << (n - 1U));
    }
    else {
        me->bits[1] |= ((QSetBits)1U << (n - 32U - 1U));
    }
#endif

    return;
}
/*..........................................................................*/
void QSet_remove(QSet * const me,
    uint8_t const n)
{
#if Q_SET_MAX <= 32U
    me->bits[0] &= ((QSetBits)(~((QSetBits)1U << (n - 1U))));
#else
    if (n <= 32U) {
        me->bits[0] &= ((QSetBits)(~((QSetBits)1U << (n - 1U))));
    }
    else {
        me->bits[1] &= ((QSetBits)(~((QSetBits)1U << (n - 32U - 1U))));
    }
#endif

    return;
}
/*..........................................................................*/
uint8_t QSet_find_max(QSet * const me) {
#if Q_SET_MAX <= 32U
    return QSet_log2(me->bits[0]);
#else
    return (me->bits[1] != 0U)
        ? (QSet_log2(me->bits[1]) + 32U)
        : (QSet_log2(me->bits[0]));
#endif
}
