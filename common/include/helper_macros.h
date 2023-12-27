#ifndef helper_macros_h_
#define helper_macros_h_

/*
******************************************************************************
                        Some useful helper macros here.
******************************************************************************
*/

/**
******************************************************************************
                                Casting
******************************************************************************
**/
#define Q_DOWN_CAST(type_, unit_)  ((type_)(unit_))
#define Q_UP_CAST(type_, unit_)    ((type_)(unit_))

/**
******************************************************************************
                                Math algorithm.
******************************************************************************
**/
#define Q_CEIL(x, y)  (((x) - 1U) / (y) + 1U)

/*
******************************************************************************
                                    Numeric
******************************************************************************
*/
#define MAX(x, y)               ((x) > (y) ? (x) : (y))
#define MIN(x, y)               ((x) < (y) ? (x) : (y))

#define EQUAL(x, y)             ((x) == (y))
#define NOT_EQUAL(x, y)         ((x) != (y))
#define GREATER_THAN(x, y)      ((x) > (y))
#define LESS_THAN(x, y)         ((x) < (y))

#define IN_RANGE(x, min, max)   ((min) <= (x) && (x) <= (max))

/*
******************************************************************************
                            also called dimension
******************************************************************************
*/
#define ARRAY_SIZE(arr)   (sizeof(arr) / sizeof((arr)[0]))

/*
******************************************************************************
                            bit operation
******************************************************************************
*/
#define SET_BIT(var, bit)     ((var) |= (1U << (bit)))
#define CLEAR_BIT(var, bit)   ((var) &= (~(1U << (bit))))
#define TOGGLE_BIT(var, bit)  ((var) ^= (1U << (bit)))
#define CHECK_BIT(var, bit)   ((var) &  (1U << (bit)))

/*
******************************************************************************
                        special macro usage: # and ##
                    Take them carefully!
******************************************************************************
*/
#define STR(x) #x

/* mind that we don't use any parentheses here, ## can not be used nestly. */
#define CONCAT(x, y) x ## y

/* some expand usages from the basic ones (This is not good) ---------------*/
#define PREFIXED_NAME(prefix, name)   prefix ## _ ## name
#define NAME_SUFFIXED(name, suffix)   name ## _ ## suffix

/*
******************************************************************************
                            pointer <==> uint
******************************************************************************
*/
#define UINT2PTR(ptype_, uint_) \
    ((ptype_)((uint_)))

// Q_PTR_SIZE_PADDING_TYPE is pointer's length equal uint type,
// platform specific
#define PTR2UINT(p_) \
    ((Q_PTR_SIZE_PADDING_TYPE)((p_)))

/*
******************************************************************************
                    Count the number of variable arguments

    Not be allowed to be used in C51.
******************************************************************************
*/
/* max num of args: 16 here */
// #define COUNT_ARGS_SEQ() 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
// #define COUNT_ARGS_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, N, ...) N
// #define COUNT_ARGS_IMPL(...)  COUNT_ARGS_N(__VA_ARGS__)
// #define COUNT_ARGS(...)  COUNT_ARGS_IMPL(__VA_ARGS__, COUNT_ARGS_SEQ())

#endif
