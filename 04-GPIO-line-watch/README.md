# GPIO chip

* <https://libgpiod.readthedocs.io/en/latest/core_line_watch.html>

Current testing is with a Pi 3B+ running the 32 bit variant of RPiOS/Trixie.

## Requirements

```text
sudo apt install libgpiod-dev
```

## 2025-11-27 explore GPIO line watch API

This is truly an exploration as it is not obvious to me how this works. The `struct gpiod_info_event *` object is returned by the `gpiod_chip_read_info_event()` which can be read following a call to `gpiod_chip_wait_info_event()` (both of which are described under "GPIO chip".)

First cut, employ `gpiod_chip_read_info_event()` and try using the CLI set to see if that causes this call to return. neither `gpioset` nor concurrent `gpionotify`/`gpioset` causes `gpiod_chip_read_info_event()` to return. This will be set aside and returned to when other APIs seen like they would relate to this API.

```text
gcc -Wall -o line-watch line-watch.c -l gpiod
```
