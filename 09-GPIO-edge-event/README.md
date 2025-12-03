# GPIO edge event

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

## fiddle with output

Reduce unnecessary commentary and print out all information associated with the events. The application will continue to report events until it times out at 5s with no activity at which point it exits.

```text
hbarta@drogo:~/Programming/GPIOD_Debian_Raspberry_Pi/09-GPIO-edge-event $ ./edge-event 
API version:2.2.1:
events to read:1
ix:0 GPIO:20 type:falling, global_seqno:1 line_seqno:1 timestamp:669464154188345 delta:669464154188345
events to read:1
ix:0 GPIO:20 type: rising, global_seqno:2 line_seqno:2 timestamp:669464415182821 delta:260994476
[...]
events to read:1
ix:0 GPIO:20 type:falling, global_seqno:72 line_seqno:72 timestamp:669478538805154 delta:237593603
events to read:2
ix:0 GPIO:20 type:falling, global_seqno:73 line_seqno:73 timestamp:669478653752288 delta:114947134
ix:1 GPIO:20 type: rising, global_seqno:74 line_seqno:74 timestamp:669478653810048 delta:57760
[...]
events to read:1
ix:0 GPIO:20 type:falling, global_seqno:99 line_seqno:99 timestamp:669483309277494 delta:101510325
events to read:2
ix:0 GPIO:20 type: rising, global_seqno:100 line_seqno:100 timestamp:669483358590164 delta:49312670
ix:1 GPIO:20 type: rising, global_seqno:101 line_seqno:101 timestamp:669483358639539 delta:49375
events to read:1
ix:0 GPIO:20 type:falling, global_seqno:102 line_seqno:102 timestamp:669483601488377 delta:242848838
hbarta@drogo:~/Programming/GPIOD_Debian_Raspberry_Pi/09-GPIO-edge-event $```
```
