#ifndef QEPN_H
#define QEPN_H

/****************************************************************************/
/*! The current QP version as a decimal constant XXYZ, where XX is a 2-digit
* major version number, Y is a 1-digit minor version number, and Z is
* a 1-digit release number.
*/
#define QP_VERSION      690U

/*! The current QP version number string of the form XX.Y.Z, where XX is
* a 2-digit major version number, Y is a 1-digit minor version number,
* and Z is a 1-digit release number.
*/
#define QP_VERSION_STR  "6.9.0"

/*! Encrypted  current QP release (6.9.0) and date (2020-08-21) */
#define QP_RELEASE      0x884D22FDU


/****************************************************************************/
/* typedefs for basic numerical types; MISRA-C 2004 rule 6.3(req). */

/*! typedef for character strings. */
/**
* @description
* This typedef specifies character type for exclusive use in character
* strings. Use of this type, rather than plain 'char', is in compliance
* with the MISRA-C 2004 Rules 6.1(req), 6.3(adv).
*/
typedef char char_t;

/*! typedef for line numbers in assertions and return from QF_run() */
typedef int int_t;

/*! typedef for enumerations used for event signals */
typedef int enum_t;

/*! IEEE 754 32-bit floating point number, MISRA-C 2004 rule 6.3(req) */
/**
* @note QP-nano does not use floating-point types anywhere in the internal
* implementation.
*/
typedef float float32_t;

/*! IEEE 754 64-bit floating point number, MISRA-C 2004 rule 6.3(req) */
/**
* @note QP-nano does not use floating-point types anywhere in the internal
* implementation.
*/
// typedef double float64_t;

/*! Scalar type describing the signal of an event. */
// typedef uint8_t QSignal;
typedef uint16_t QSignal;

/****************************************************************************/
/*! Event structure. */
/**
* @description
* ::QEvt represents events, optionally with a single scalar parameter.
* @sa Q_PARAM_SIZE
* @sa ::QParam
*/
typedef struct {
    QSignal sig; /*!< signal of the event */
} QEvt;

/*
******************************************************************************
            used for replacing the QEvt *e param of state handler
******************************************************************************
*/
extern QEvt *p_global_evt_dispatch_;

extern QEvt *p_global_evt_post_;

/****************************************************************************/
/*! Macro to access the signal of the current event of a state machine */
/**
* @param[in,out] me_ pointer to a subclass of ::QHsm (see @ref oop)
*/
// #define Q_SIG(me_)  (((QHsm *)(me_))->evt.sig)
/*
******************************************************************************
                work with the dispatch of framework
******************************************************************************
*/
#define Q_SIG(me_)  (p_global_evt_dispatch_->sig)
#define Q_EVT(me_)  (p_global_evt_dispatch_)

/****************************************************************************/
/*! Type returned from  a state-handler function. */
typedef uint_fast8_t QState;

/*! the signature of a state handler function */
typedef QState (*QStateHandler)(void * const me);

/****************************************************************************/
/*! Hierarchical State Machine */
/**
* @description
* QHsm represents a Hierarchical Finite State Machine (HSM) with full
* support for hierarchical nesting of states, entry/exit actions,
* and initial transitions in any composite state.
*
* @note QHsm is not intended to be instantiated directly, but rather serves
* as the base structure for derivation of state machines in the application
* code.
*
* @usage
* The following example illustrates how to derive a state machine structure
* from QHsm. Please note that the QHsm member 'super' is defined as the
* _first_ member of the derived struct.
* @include qepn_qhsm.c
*
* @sa @ref oop
*/
typedef struct {
    QStateHandler state;  /*!< current active state (state-variable) */
    QStateHandler temp;   /*!< temporary: tran. chain, target state, etc. */
} QHsm;

/*! Polymorphically executes the top-most initial transition in a SM. */
/**
* @param[in,out] me_ pointer (see @ref oop)
*
* @note Must be called only ONCE after the SM "constructor".
*
* @usage
* The following example illustrates how to initialize a SM, and dispatch
* events to it:
* @include qepn_qhsm_use.c
*/
#define QHSM_INIT(me_)  \
    QHsm_init_((me_));

/*! Polymorphically dispatches an event to a HSM. */
/**
* @description
* Processes one event at a time in Run-to-Completion fashion.
*
* @param[in,out] me_ pointer (see @ref oop)
*
* @note Must be called after the "constructor" and after QHSM_INIT().
*/
#define QHSM_DISPATCH(me_)  \
    QHsm_dispatch_((me_))

/* public methods */
/*! "constructor" of a HSM.
* @protected @memberof QHsm
*/
void QHsm_ctor(QHsm * const me,
    QStateHandler initial);

/*! Obtain the current active state from a HSM (read only). */
/**
* @param[in] me_ pointer (see @ref oop)
*
* @returns the current active state of a HSM
*/
#define QHsm_state(me_) (Q_STATE_CAST(Q_HSM_UPCAST(me_)->state))

/*! Implementation of the top-most initial transition in QHsm.
* @private @memberof QHsm
*/
void QHsm_init_(QHsm * const me);

/*! Implementation of dispatching events to QHsm.
* @private @memberof QHsm
*/
void QHsm_dispatch_(QHsm * const me);

/*! the top-state.
* @protected @memberof QHsm
*/
QState QHsm_top(void const * const me);


/****************************************************************************/
/* All possible values returned from state/action handlers */

/* unhandled and need to "bubble up"... */

/*! event passed to superstate to handle */
#define Q_RET_SUPER      ((QState)0)

/*! event passed to submachine superstate */
#define Q_RET_SUPER_SUB  ((QState)1)

/*! event unhandled due to a guard */
#define Q_RET_UNHANDLED  ((QState)2)

/* handled and do not need to "bubble up"... */
/*! event handled (internal transition) */
#define Q_RET_HANDLED    ((QState)3)

/*! event silently ignored (bubbled up to top) */
#define Q_RET_IGNORED    ((QState)4)

/* entry/exit... */
/*! state entry action executed */
#define Q_RET_ENTRY      ((QState)5)

/*! state exit  action executed */
#define Q_RET_EXIT       ((QState)6)

/* no side effects */
/*! return value without any effect */
#define Q_RET_NULL       ((QState)7)

/* transitions need to execute transition-action table in QHsm... */
/*! event handled (regular transition) */
#define Q_RET_TRAN       ((QState)8)

/*! initial transition in a state or submachine */
#define Q_RET_TRAN_INIT  ((QState)9)

/*! event handled (transition to history) */
#define Q_RET_TRAN_HIST  ((QState)10)


/*! Perform upcast from a subclass of ::QHsm to the base class ::QHsm */
/**
* @description
* Upcasting from a subclass to superclass is a very frequent and __safe__
* operation in object-oriented programming and object-oriented languages
* (such as C++) perform such upcasting automatically. However, OOP is
* implemented in C just as a set of coding conventions (see @ref oop),
* and the C compiler does not "know" that certain types are related by
* inheritance. Therefore for C, the upcast must be performed explicitly.
* Unfortunately, pointer casting violates the advisory MISRA-C 2004 rule 11.4
* "cast pointer to pointer". This macro encapsulates this deviation and
* provides a descriptive name for the reason of this cast.
*/
#define Q_HSM_UPCAST(ptr_) ((QHsm *)(ptr_))

/*! Perform cast to ::QStateHandler. */
/**
* @description
* This macro encapsulates the cast of a specific state handler function
* pointer to QStateHandler, which violates MISRA-C 2004 rule 11.4(advisory).
* This macro helps to localize this deviation.
*
* @usage
* @include qepn_qhsm_ctor.c
*/
#define Q_STATE_CAST(handler_)  ((QStateHandler)(handler_))

/*! Macro to call in a state-handler when it executes a regular
* or and initial transition. Applicable to both HSMs and FSMs.
* @include qepn_qtran.c
*/
#define Q_TRAN(target_)  \
    ((Q_HSM_UPCAST(me))->temp = Q_STATE_CAST(target_), (QState)Q_RET_TRAN)

/*! Macro to call in a state-handler when it executes a transition
* to history. Applicable only to HSMs.
*
* @usage
* @include qepn_qhist.c
*/
#define Q_TRAN_HIST(hist_)  \
    ((Q_HSM_UPCAST(me))->temp = (hist_), (QState)Q_RET_TRAN_HIST)

/*! Macro to call in a state-handler when it designates the
* superstate of a given state. Applicable only to HSMs.
*
* @usage
* @include qepn_qtran.c
*/
#define Q_SUPER(super_)  \
    ((Q_HSM_UPCAST(me))->temp = Q_STATE_CAST(super_), Q_RET_SUPER)

/*! Macro to call in a state-handler when it handles an event.
*  Applicable to both HSMs and FSMs.
*/
#define Q_HANDLED()      Q_RET_HANDLED

/*! Macro to call in a state-handler when it attempts to handle
* an event but a guard condition evaluates to 'false' and there is no other
* explicit way of handling the event. Applicable only to HSMs.
*/
#define Q_UNHANDLED()    Q_RET_UNHANDLED


/*! QP reserved signals */

/*! signal for coding entry actions */
#define Q_ENTRY_SIG     ((QSignal)1)

/*! signal for coding exit actions */
#define Q_EXIT_SIG      ((QSignal)2)

/*! signal for coding nested initial transitions */
#define Q_INIT_SIG      ((QSignal)3)

/*! timeout signal at the default tick rate 0 */
#define Q_TIMEOUT_SIG   ((QSignal)4)

/*! timeout signal at tick rate 1 */
#define Q_TIMEOUT1_SIG  ((QSignal)5)

/*! timeout signal at tick rate 2 */
#define Q_TIMEOUT2_SIG  ((QSignal)6)

/*! timeout signal at tick rate 3 */
#define Q_TIMEOUT3_SIG  ((QSignal)7)

/*!< first signal for the user applications */
#define Q_USER_SIG      ((QSignal)8)

/*! Perform cast from unsigned integer to a pointer of type @a type_ */
/**
* @description
* This macro encapsulates the cast to (type_ *), which QP ports or
* application might use to access embedded hardware registers.
* Such uses can trigger PC-Lint "Note 923: cast from int to pointer" and
* this macro helps to encapsulate this deviation.
*/
#define Q_UINT2PTR_CAST(type_, uintptr_)  ((type_ *)(uintptr_))

/****************************************************************************/
/* macros for accessing data in ROM */
#ifndef Q_ROM /* if NOT defined, provide the default definition */

    /*! Macro to specify compiler-specific directive for placing a
    * constant object in ROM. */
    /**
    * @description
    * Many compilers for 8-bit Harvard-architecture MCUs provide non-standard
    * extensions to support placement of objects in different memories.
    * In order to conserve the precious RAM, QP-nano uses the Q_ROM macro for
    * all constant objects that can be allocated in ROM.
    *
    * @note
    * To override the following empty definition, you need to define the
    * Q_ROM macro in the qpn_port.h header file. Some examples of valid
    * Q_ROM macro definitions are: __code (IAR 8051 compiler), code (Keil
    * 8051 compiler), PROGMEM (gcc for AVR), __flash (IAR for AVR).
    */
    #define Q_ROM
#endif

#ifndef Q_ROM_BYTE
    /*! Macro to access a byte allocated in ROM */
    /**
    * Some compilers for Harvard-architecture MCUs, such as gcc for AVR, do
    * not generate correct code for accessing data allocated in the program
    * space (ROM). The workaround for such compilers is to explicitly add
    * assembly code to access each data element allocated in the program
    * space. The macro Q_ROM_BYTE() retrieves a byte from the given ROM
    * address.
    *
    * @note
    * The Q_ROM_BYTE() macro should be defined in the qpn_port.h header file
    * for each compiler that cannot handle correctly data allocated in ROM
    * (such as the gcc). If the macro is left undefined, the default
    * definition simply returns the parameter and lets the compiler
    * synthesize the correct code.
    */
    #define Q_ROM_BYTE(rom_var_)   (rom_var_)
#endif

#ifndef Q_ROM_PTR
    /*! Macro to access a pointer allocated in ROM */
    /**
    * Some compilers for Harvard-architecture MCUs, such as gcc for AVR, do
    * not generate correct code for accessing data allocated in the program
    * space (ROM). The workaround for such compilers is to explicitly add
    * assembly code to access each data element allocated in the program
    * space. The macro Q_ROM_PTR() retrieves an object-pointer from the given
    * ROM address. Please note that the pointer can be pointing to the object
    * in RAM or ROM.
    *
    * @note
    * The Q_ROM_PTR() macro should be defined in the qpn_port.h header file
    * for each compiler that cannot handle correctly data allocated in ROM
    * (such as the gcc). If the macro is left undefined, the default
    * definition simply returns the parameter and lets the compiler
    * synthesize the correct code.
    */
    #define Q_ROM_PTR(rom_var_)    (rom_var_)
#endif

#endif /* QEPN_H */
