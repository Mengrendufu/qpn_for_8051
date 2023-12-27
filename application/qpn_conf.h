#ifndef QPN_CONF_H
#define QPN_CONF_H

/*
******************************************************************************
                                Abandoned
******************************************************************************
*/
// #define Q_PARAM_SIZE

/*
******************************************************************************
                                Applicable
******************************************************************************
*/
// Work with Q_LOG2()
#define MAX_ACTIVE  16U

// not used assert in 8051
#define Q_NASSERT

#define QF_TIMEEVT_CTR_SIZE  2U
#define QF_MAX_TICK_RATE     4U
#define QF_TIMEEVT_PERIODIC

#endif  /* QPN_CONF_H */
