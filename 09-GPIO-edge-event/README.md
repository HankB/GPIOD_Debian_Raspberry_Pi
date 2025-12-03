# GPIO cedge eventip

* <https://libgpiod.readthedocs.io/en/latest/core_edge_event.html>

Current testing is with a Pi 3B+ running the 32 bit variant of RPiOS/Trixie.

This API can be used to respond to events coming from inputs. (Perhaps it can also respond to edge events on outputs but that will not be explored at this time.)

The code required to configure an input is in `../08-GPIO-line-request/line-request.c` so that will be used as tha starting point, removing all output related configuration and reducing the chattiness if the existing code.

## Requirements

```text
sudo apt install libgpiod-dev
```

## 2025-12-02 remove output related configuration

```text
gcc -Wall -o edge-event edge-event.c -l gpiod
```

```text
```
