#ifndef qset_h_
#define qset_h_

/**
******************************************************************************
                                QSet helper macros
******************************************************************************
**/
#define QSET_CEIL(x, y)  (((x) - 1U) / (y) + 1U)

/**
******************************************************************************
                                Class struct
    QSet contains a 64-bit-max bitmask. The configuration outside is
compile-time.
******************************************************************************
**/
#ifndef Q_SET_MAX
    #define Q_SET_MAX  64U  /* maximum default */
#endif  /* Q_SET_MAX */

#if (Q_SET_MAX <= 8U)
    typedef uint8_t QSetBits;
#endif

#if ((8U < Q_SET_MAX) && (Q_SET_MAX <= 16U))
    typedef uint16_t QSetBits;
#endif

#if (16U < Q_SET_MAX)
    typedef uint32_t QSetBits;
#endif

#if (Q_SET_MAX > 64U)
    #error Q_SET_MAX exceeds the maximum of 64U;
#endif

/**
******************************************************************************
    Q_SET_MAX               QSet                  [n]
    1  --> 8                uint8_t                1
    9  --> 16               uint16_t               1
    17 --> 32               uint32_t               1
    33 --> 64               uint32_t uint32_t      2

Q_SET_MAX ------ QSet ------ MAX_BIT
    1         QSetBits[1]       8
    2         QSetBits[1]       8
    3         QSetBits[1]       8
    4         QSetBits[1]       8
    5         QSetBits[1]       8
    6         QSetBits[1]       8
    7         QSetBits[1]       8
    8         QSetBits[1]       8
    9         QSetBits[1]      16
    10        QSetBits[1]      16
    11        QSetBits[1]      16
    12        QSetBits[1]      16
    13        QSetBits[1]      16
    14        QSetBits[1]      16
    15        QSetBits[1]      16
    16        QSetBits[1]      16
    17        QSetBits[1]      32
    18        QSetBits[1]      32
    19        QSetBits[1]      32
    20        QSetBits[1]      32
    21        QSetBits[1]      32
    22        QSetBits[1]      32
    23        QSetBits[1]      32
    24        QSetBits[1]      32
    25        QSetBits[1]      32
    26        QSetBits[1]      32
    27        QSetBits[1]      32
    28        QSetBits[1]      32
    29        QSetBits[1]      32
    30        QSetBits[1]      32
    31        QSetBits[1]      32
    32        QSetBits[1]      32
    33        QSetBits[2]      64
    34        QSetBits[2]      64
    35        QSetBits[2]      64
    36        QSetBits[2]      64
    37        QSetBits[2]      64
    38        QSetBits[2]      64
    39        QSetBits[2]      64
    40        QSetBits[2]      64
    41        QSetBits[2]      64
    42        QSetBits[2]      64
    43        QSetBits[2]      64
    44        QSetBits[2]      64
    45        QSetBits[2]      64
    46        QSetBits[2]      64
    47        QSetBits[2]      64
    48        QSetBits[2]      64
    49        QSetBits[2]      64
    50        QSetBits[2]      64
    51        QSetBits[2]      64
    52        QSetBits[2]      64
    53        QSetBits[2]      64
    54        QSetBits[2]      64
    55        QSetBits[2]      64
    56        QSetBits[2]      64
    57        QSetBits[2]      64
    58        QSetBits[2]      64
    59        QSetBits[2]      64
    60        QSetBits[2]      64
    61        QSetBits[2]      64
    62        QSetBits[2]      64
    63        QSetBits[2]      64
    64        QSetBits[2]      64
******************************************************************************
**/
typedef struct QSet {
    QSetBits bits[QSET_CEIL(Q_SET_MAX,
        8U * sizeof(QSetBits))];
} QSet;

/**
******************************************************************************
    The input type QSetBits could be uint8_t, uint16_t and uint32_t, depending
on the configuration.
******************************************************************************
**/
uint8_t QSet_log2(QSetBits const bitmask);

/**
******************************************************************************
    Clear all to 0U
******************************************************************************
**/
void QSet_set_empty(QSet * const me);

/**
******************************************************************************
    Check == 0U
******************************************************************************
**/
bool QSet_is_empty(QSet * const me);

/**
******************************************************************************
    Check != 0U
******************************************************************************
**/
bool QSet_is_not_empty(QSet * const me);

/**
******************************************************************************
    Check the bitmask via QSet_log2()

    @param n 1 ~ 64
******************************************************************************
**/
// & bitmask
bool QSet_has_element(QSet * const me,
    uint8_t const n);

// &= ~bitmask
void QSet_insert(QSet * const me,
    uint8_t const n);

// |= bitmask
void QSet_remove(QSet * const me,
    uint8_t const n);

/**
******************************************************************************
    clz
    log2
******************************************************************************
**/
uint8_t QSet_find_max(QSet * const me);

#endif  /* qset_h_ */
