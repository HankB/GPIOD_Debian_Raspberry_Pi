# GPIO chip

* <https://libgpiod.readthedocs.io/en/latest/core_line_settings.html>

Current testing is with a Pi 3B+ running the 32 bit variant of RPiOS/Trixie.

## Requirements

```text
sudo apt install libgpiod-dev
```

## 2025-11-27 explore GPIO line configuration API

> Functions for manipulating line configuration objects.

Line configuration ties `struct gpiod_line_settings` to actual lines, maybe. It seems not to actually work with the lines.

```text
gcc -Wall -o line-configuration line-configuration.c -l gpiod
```

```text
hbarta@drogo:~/Programming/GPIOD_Debian_Raspberry_Pi/06-GPIO-line-configuration $ ./line-configuration 
API version:2.2.1:
Line configuration object acquired
Line settings object acquired
      change: 0
line set to output and set active
gpiod_line_settings added to gpiod_line_config
hbarta@drogo:~/Programming/GPIOD_Debian_Raspberry_Pi/06-GPIO-line-configuration $ 
```
