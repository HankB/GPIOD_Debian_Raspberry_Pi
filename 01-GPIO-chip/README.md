# GPIO Chip

* <https://libgpiod.readthedocs.io/en/latest/core_chips.html>

Current testing is with a Pi 3B+ running the 32 bit variant of RPiOS/Trixie.

## Requirements

```text
sudo apt install libgpiod-dev
```

## 2025-11-27 explore GPIO Chip API

```text
gcc -Wall -o chip chip.c -l gpiod
```

```text
hbarta@drogo:~/Programming/GPIOD_Debian_Raspberry_Pi/01-GPIO-Chip $ ./chip
API version:2.2.1:
opened chip '/dev/gpiochip0'

  Info for chip:/dev/gpiochip0
           name:gpiochip0
          label:pinctrl-bcm2835
number of lines:54
line info for:GPIO8
       direction:3

line info for:GPIO20
       direction:2

line info for:GPIO10
       direction:2

hbarta@drogo:~/Programming/GPIOD_Debian_Raspberry_Pi/01-GPIO-Chip $ 
```

NB `print_line_info()` includes APIs not part of 'GPIO Chip' page.
