#ifndef QPN_H
#define QPN_H

/**
* @description
* This header file must be included directly or indirectly
* in all application modules (*.c files) that use QP-nano.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "qpn_conf.h" /* QP-nano configuration file (from the application) */
#include "qfn_port.h" /* QF-nano port from the port directory */
#include "qassert.h"  /* embedded systems-friendly assertions */


/****************************************************************************/
/* QP API compatibility layer */
#ifndef QP_API_VERSION

/*! Macro that specifies the backwards compatibility with the
* QP-nano API version.
*/
/**
* @description
* For example, QP_API_VERSION=450 will cause generating the compatibility
* layer with QP-nano version 4.5.0 and newer, but not older than 4.5.0.
* QP_API_VERSION=0 causes generation of the compatibility layer "from the
* begining of time", which is the maximum backwards compatibilty. This is
* the default.@n
* @n
* Conversely, QP_API_VERSION=9999 means that no compatibility layer should
* be generated. This setting is useful for checking if an application
* complies with the latest QP-nano API.
*/
#define QP_API_VERSION 0

#endif  /* QP_API_VERSION */

/****************************************************************************/
#if (QP_API_VERSION < 580U)

/*! @deprecated QMActive Control Block; instead use: ::QActiveCB. */
typedef QActiveCB QMActiveCB;

/*! @deprecated QMActive; instead use: ::QActive. */
typedef QActive QMActive;

/*! @deprecated QMsm state machine; instead use: ::QHsm. */
typedef QHsm QMsm;

/*! @deprecated QMActive constructor; instead use: QActive_ctor() */
#define QMActive_ctor QActive_ctor

/*! @deprecated QMsm state machine constructor; instead use: QHsm_ctor() */
#define QMsm_ctor     QHsm_ctor

#endif /* QP_API_VERSION < 580U */
/****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif  /* QPN_H */
