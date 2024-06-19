# C Blinky

Produce the embedded version of "Hello World" using C and he `gpiod` library to deal with GPIO pins as inputs and outputs. The `gpiod` library is documented at <file:///usr/share/doc/libgpiod-dev/html/index.html>

Platform is Raspberry Pi 3B+ running off an SSD on Debian Bullseye (Not Raspberry Pi OS.) 

## Requirements

* `apt install libgpiod-dev`

## `read`

Read the state of GPIO 20 using APIs listed at <file:///usr/share/doc/libgpiod-dev/html/group__high__level.html>. Build with:

```text
gcc -Wall -o read read.c -l gpiod
```

## `drive`

Drive GPIO 8, same APIs as `read`. 

### Previous behavior

It shares similar behavior with `gpioset` in that returns the GPIO to an input configuration on return. By putting a 1s delay in the callback function, the result of the output can be viewed. There doesn't seem to be any way around this so this API seems not useful for a process that needs to set an output and continue execution.

### Current behavior

The output is now sticky. When the program exits, the output remains set to whatever was set to. `gpioinfo` confirms that it is set to output. `gpioget` reconfigures the pin as an input. Mixing CLI and simple C APIs is a little fraught.

```text
gcc -Wall -o drive drive.c -l gpiod
```

## `event`

Monitor an event for GPIO 20 (button press.) Things I learned.

1. Event callback, first arg is event - 2 => rising edge, 3 => falling edge
1. Event callback, second arg is GPIO #. Zero means a timeout.
1. Event callback, returns 0, polling continues. Return 1, `gpiod_ctxless_event_monitor()` returns.
1. The event mechanism does not consume a processor core. IOW, interrupt driven and not continuous polling.

Build:

```text
gcc -Wall -o event event.c -l gpiod
```

## `event_drive`

### first version

Drive a GPIO pin when a pushbutton is pressed. This is done in the event callback and the call to drive the output itself has a callback to provide timing. It's callbacks all the way down! In particular I wonder if calling another API within the callback is legal. It seems to work w/out difficulty. Judging by the behavior, a noisy pushbutton input may be calling the event callback when a previous callback has not yet returned. Or maybe they are just firing in sequence.

### Present version

Drive a GPIO pin to follow an inpout on another pin. Outputs (and repoorting) is in the event callback. The timing callback to hold the output has been removed.

```text
gcc -Wall -o event_drive event_drive.c -l gpiod
```

## Status

This wraps up the portions of the [High-level API](https://libgpiod.readthedocs.io/en/latest/group__high__level.html) that interest.
