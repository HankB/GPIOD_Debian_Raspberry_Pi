# Reading & setting line values

Finally get to play with I/O using APIs described at <https://libgpiod.readthedocs.io/en/latest/group__line__value.html>.

The same LED setup was used as with [line config](../line_config/README.md) with the addition of a momentary (normally open) pushbutton switch connecting GPIO 11 and ground. However the pullup was nos strong enough to trigger a state change on a GPIO so the cirduit was changed to be:

```text
                --- GPIO 11
                |
3V3 - 1K ohm R --- Gnd
                |
            pushbutton
                |
                --- GND
```

In other words, a 1K external pullup on the input that could be brought to ground using the pushbutton.

## `line_events`

```text
gcc -Wall -o line_events line_events.c -l gpiod
```

```text
hbarta@bullpi3b:~/Programming/GPIOD_Debian_Raspberry_Pi/line_events$ ./line_events 
event 2 at 261136 s, 336607648 ns
hbarta@bullpi3b:~/Programming/GPIOD_Debian_Raspberry_Pi/line_events$ 
```
