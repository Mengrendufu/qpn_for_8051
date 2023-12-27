## qpn for 8051

qpn framework for 8051.

Modified based on ```qpn v6.9.0```

***

## Rebuilding the qpn specific for the 8051

```c
~/qpn
|-- application
|	|-- application.h
|	|-- blinky.c
|	|-- blinky.h
|	|-- main.c
|	|-- qpn_conf.h
|-- bsp
|	|-- bsp.c
|	|-- bsp.h
|-- common
|	|-- include
|	|	|-- helper_macros.h
|	|	|-- qmpool.h
|	|	|-- queue.h
|	|-- src
|	|   |-- qmpool
|	|	|  |-- qmp_get.c
|	|	|  |-- qmp_init.c
|	|	|  |-- qmp_put.c
|	|	|-- queue
|	|	|  |-- queue.c
|	|-- common.h
|-- include
|	|-- qassert.h
|	|-- qepn.h
|	|-- qfn.h
|	|-- qpn.h
|	|-- qvn.h
|-- ports
|	|-- qfn_port.c
| 	|-- qfn_port.h
| 	|-- stdbool_.h
| 	|-- stdint_.h
|-- src
|	|-- qfn
|	|	|-- qepn.c
|	|	|-- qfn.c
|	|-- qvn
|	|	|-- qvn.c
```

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
            DEBUG_PRINTF("LedOn\n");
        }
        else {
            DEBUG_PRINTF("LedOff\n");
        }

        break;
    }
    // other codes ...
```
