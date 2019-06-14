# DotMatrix full demo

This demo is a full-featured demo providing a common base for your hacking needs. It is based around
an RTOS (ChibiOS) and lets the user choose between multiple applications. It provides some
convinience utilities, such as physical topology detection and messaging protocol. If you do
not need these aminities, you may want to look at the simpler GameOfLife demo.

To see how you can create an applet, take a look at `applets/hello.c`. Do not forget to add
your applet to the Makefile. You do not have to register it explicitely, it will be done
by the `applet_add` macro. The applet's `run` function will be run when the applet is selected.
It is recommanded to contain a `while (!applet_should_end)` loop. You might want to set
your applet as `autostart` during debugging.

## ChibiOS basics

You can find documentation for ChibiOS at http://chibios.sourceforge.net/docs/19.1/full_rm/index.html.

## Functions suffixes

`I` suffix signifies function can be called from ISR locked state.
`S` suffix signifies function can be called from locked state.
`X` suffix signifies function can be called from any.

### Quick reference

- Getting current time: `chVTGetSystemTimeX`
- Sleep: `chThdSleep`
- Specifying time-outs: use `TIME_MS2I`, `TIME_S2I` or `TIME_INFINITE`
- Non-blocking calls: pass `TIME_IMMEDIATE` as a time-out paremeter
- Wait for multiple events: use `chEvtRegister` -- see `chooser_run` in `chooser_applet.c`
- Absolute time-outs: see `util/timeout.h`
- Panic: `chSysHalt`
- Memory allocation: `<chmemheaps.h>` `chHeapAlloc`
- Create a task: use `chThdCreateStatic` with working area allocatd by `THD_WORKING_AREA`

### Debugging

OpenOCD has support for ChibiOS threads. Use `info threads` and `thread X` to show and switch
threads respectivelly.

Also check `util/stack_usage.c` which you can use to get the biggest stack usage so far. Paste
these lines into GDB:
```
p stat_threads()
set print pretty on
p stat_thrinfo
```

## Messaging

There is a simple framing protocol which adds message IDs, checksum and routing information to
the messages. To send those messages, use either `i2c_send`, `i2c_broadcast` or `usart_send`.

To receive messages, the concept of message queues are used (`util/message_queue.h`). To get a
next message from the queue use `msg_rx_queue_get` and `msg_rx_queue_ack`. You will get a message
header and a "handle" to read the actual data from the queue buffer.

By default, the messages are automatically placed in the `default_queue` (both from i2c and uart).
If you want to change the queue, you can use the `dispatch_msg` callback of your applet.
This is used for example in the `GameOfLife`, where the time synchronization message (the tick)
is placed in a different queue than other messages.

## Hardware

Apart from UART and I2C (see previous paragraph), there are APIs for:
 - Buttons: `hw/buttons.h`
 - USB serial console: `console.h`
 - Unique Cell ID: `cell_id.h` (most of the time, you will want to use the short id from topology)
 - LED Matrix display: `hw/disp.h`

## Topology

When the full demo starts, it automatically enumerates all connected boards and elects a master
board. You can use this topology information to do proper display rotation (all boards can
show images rotated the same way), address the boards
over I2C or decide what will be shown on what board. Check the `util/topo.h` for more information.

## Other utilities

We have already touched on the topic of message queues. There is an alternative for streams without
any framing (`util/byte_queue.h`).

There are also utilities for basic operations with 2d integer vectors (used mostly for cell positions).

