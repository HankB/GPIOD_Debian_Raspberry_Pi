# GPIO chip

* <https://libgpiod.readthedocs.io/en/latest/core_line_settings.html>

Current testing is with a Pi 3B+ running the 32 bit variant of RPiOS/Trixie.

## Requirements

```text
sudo apt install libgpiod-dev
```

## 2025-11-27 explore GPIO line settings API

> Functions for manipulating line settings objects.

These can be associated with actual GPIO lines in "GPIO line configuration." In this bit (at least for now) some settings will be manipulated. It seems pointless to deal with all.

```text
gcc -Wall -o line-settings line-settings.c -l gpiod
```

```text
hbarta@drogo:~/Programming/GPIOD_Debian_Raspberry_Pi/05-GPIO-line-settings $ ./line-settings 
API version:2.2.1:
Settings object acquired
   direction: 1
      change: 0
   direction: 2
      change: 0
   direction: 3
hbarta@drogo:~/Programming/GPIOD_Debian_Raspberry_Pi/05-GPIO-line-settings $ 
```
