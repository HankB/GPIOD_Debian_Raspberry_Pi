# C Blinky

Produce the embedded version of "Hello World" using C and he `gpiod` library to deal with GPIO pins as inputs and outputs. The `gpiod` library is documented at <file:///usr/share/doc/libgpiod-dev/html/index.html>

Platform is Raspberry Pi 3B+ running off an SSD on Debian Bullseye (Not Raspberry Pi OS.) 

## `read`

Read the state of GPIO 7 using APIs listed at <file:///usr/share/doc/libgpiod-dev/html/group__high__level.html>. Build with:

```text
gcc -Wall -o read read.c -l gpiod
```

## `drive`

Drive GPIO 8, same APIs as `read`. It shares similar behavior with `gpioset` in that returns the GPIO to an input configuration on return. By putting a 1s delay in the callback function, the result of the output can be viewed. There doesn't seem to be any way around this so this API seems not useful for a process that needs to set an output and continue execution.

```text
gcc -Wall -o drive drive.c -l gpiod
```

## `event`

Monitor an event for GPIO 7 (button press.) Things I learned.

1. Event callback, first arg is event - 2 => rising edge, 3 => falling edge
1. Event callback, second arg is GPIO #. Zero means a timeout.
1. Event callback, returns 0, polling continues. Return 1, `gpiod_ctxless_event_monitor()` returns.
1. The event mechanism does not consume a processor core. IOW, interrupt driven and not continuous polling.

Build:

```text
gcc -Wall -o event event.c -l gpiod
```
