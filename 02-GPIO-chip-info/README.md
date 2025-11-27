# GPIO chip info

* <https://libgpiod.readthedocs.io/en/latest/core_chips.html>

Current testing is with a Pi 3B+ running the 32 bit variant of RPiOS/Trixie.

## Requirements

```text
sudo apt install libgpiod-dev
```

## 2025-11-27 explore GPIO Chip info API

```text
gcc -Wall -o chip-info chip-info.c -l gpiod
```
