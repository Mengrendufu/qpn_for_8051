## qpn for 8051

qpn framework for 8051.

Modified based on ```qpn v6.9.0```

***

## Rebuilding the qpn specific for the 8051

### File structure

```c
~/qpn_blinky
|-- application
|   |-- application.h
|   |-- blinky.c
|   |-- blinky.h
|   |-- main.c
|   |-- qpn_conf.h
|-- bsp
|   |-- bsp.c
|   |-- bsp.h
|   |-- printer_debug.c
|   |-- printer_debug.h
|   |-- printer_polymorphism.c
|   |-- receiver_debug.c
|   |-- receiver_debug.h
|   |-- receiver_polymorphism.c
|-- common
|   |-- include
|   |   |-- helper_macros.h
|   |   |-- m_crc16.h
|   |   |-- m_fsm.h
|   |   |-- printer.h
|   |   |-- qmpool.h
|   |   |-- qset.h
|   |   |-- queue.h
|   |   |-- receiver.h
|   |-- src
|   |   |-- crc
|   |   |   |-- m_crc16.c
|   |   |-- m_fsm
|   |   |   |-- m_fsm.c
|   |   |-- printer
|   |   |   |-- printer.c
|   |   |-- qmpool
|   |   |   |-- qmp_get.c
|   |   |   |-- qmp_init.c
|   |   |   |-- qmp_put.c
|   |   |-- qset
|   |   |   |-- qset.c
|   |   |-- queue
|   |   |   |-- queue.c
|   |   |-- receiver
|   |   |   |-- receiver.c
|   |-- common.h
|-- include
|   |-- qassert.h
|   |-- qepn.h
|   |-- qfn.h
|   |-- qpn.h
|   |-- qvn.h
|-- ports
|   |-- cpu_port.h
|   |-- m_fsm_port.h
|   |-- qfn_port.c
|   |-- qfn_port.h
|-- src
|   |-- qfn
|   |   |-- qepn.c
|   |   |-- qfn.c
|   |-- qvn
|   |   |-- qvn.c
|-- README.md
```

### Framework layers

```c
            Application::AOs
-----------------------------------------
                        QF-nano
                        QEP-nano
  BSP       M_Fsm       QV-nano
-----------------------------------------
  ^  ^       ^  ^        ^  ^
  |  |       |  |        |  |
 common   m_fsm_port    qfn_port
-----------------------------------------
                ^ ^
                | |
                cpu_port
```

## Framework porting

Every single file of 3 dirs followed to be modify:

**~/*application***
**~/bsp**
**~/ports**

## How me post and process events

```Q_DOWN_CAST()``` matters and is very dangerous.

### Posting

```c
void Blinky_Evt_post(Blinky *me,
    uint8_t sig,
    uint8_t blink_on_off)
{
    p_global_evt_post_ = (QEvt *)0;
    p_global_evt_post_ = Q_NEW_EVT(&g_sys_evt_pool);
    if (p_global_evt_post_) {
        p_global_evt_post_->sig = sig;
        Q_DOWN_CAST(Blinky_Evt *, p_global_evt_post_)->blink_on_off =
            blink_on_off;
    }
    QACTIVE_POST(&me->super,
        p_global_evt_post_);

    return;
}
```

### Processing

```c
    // other codes ...
    case BLINKY_SIG: {
        if (Q_DOWN_CAST(Blinky_Evt *, Q_EVT(me))->blink_on_off) {
            //            0123456789012----
            PRINTF_DEBUG("LedOn        \r\n");
        }
        else {
            //            0123456789012----
            PRINTF_DEBUG("LedOff       \r\n");
        }

        break;
    }
    // other codes ...
```

### The Printer_debug

How to log out fucking aggressively.

```c
QF_INT_DISABLE();
//------------0123456789012----
PRINTF_DEBUG("Hello, World!\r\n");
QF_INT_ENABLE();

QF_INT_DISABLE();
PRINTF_DEBUG_START()
//-------------0123456789012----
PRINTF_DEBUG_("Hello, World!\r\n");
PRINTF_DEBUG_ASSIGN(12) = 0x88;
PRINTF_DEBUG_END()
QF_INT_ENABLE();
```

### The Receiver_debug

In the ```Receiver_received_frame_process```:

```c
uint32_t debug_recv_code;
debug_recv_code =
    (((uint32_t)(me->process_frame[2])) << 24) |
        (((uint32_t)(me->process_frame[3])) << 16) |
            (((uint32_t)(me->process_frame[4])) << 8) |
                (((uint32_t)(me->process_frame[5])) << 0);

switch (debug_recv_code) {
    case 0x0001: {
        // do something
        break;
    }
    case 0x0002: {
        break;
    }
    case 0x0003: {
        break;
    }
    default: {
        // printf the recv data
        PRINTF_DEBUG_START()
        //-------------01234567890123--
        PRINTF_DEBUG_("Unexpct:      \n");
        PRINTF_DEBUG_ASSIGN(10) = me->process_frame[2];
        PRINTF_DEBUG_ASSIGN(11) = me->process_frame[3];
        PRINTF_DEBUG_ASSIGN(12) = me->process_frame[4];
        PRINTF_DEBUG_ASSIGN(13) = me->process_frame[5];
        PRINTF_DEBUG_END()

        break;
    }
}
```
