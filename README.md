# GPIOD_Debian_Raspberry_Pi

## 2025-11-27 Plan update

I'm struggling to map the sub-projects (beyone CLI) to the V2.2 APIs. For thgis reason I plan to set aside the old sub-projects and create new sub-projects based on heradings under the Copre API (https://libgpiod.readthedocs.io/en/latest/core_api.html) I'm also struggling to work out how the new APIs are to be assembled for what I need. (What I need is the capability to monitor events from a GPIO pin to accurately measure time using an HC-SR04 ulktrasonic sensor. I also need to drive another GPIO to trigger sensing.) As an aside, I'll prepend the directory names with numbers so they sort in order of implementation. (e.g. `CLI` becomes `00-CLI`.)

## Project

Explore GPIOD on Raspberry Pi running Debian Linux.

This is the second generation of the project which supports GPIOD V2.x which comes with Debian/RpiOS Trixie and is not API compatible. If you need to explore GPIOD on an older host OS, checkout the branch `GPIOD_V1.6`

## Motivation

Recent explorations demonstrate that Debian (not Debian based Raspberry Pi OS AKA RPiOS) runs well on the Pi Zero and Pi 3B+. One question regarding this is the usability of GPIO which is one of the useful capabilities of the Raspberry Pi.

## GPIO libraries

Here is a brief survey of the current situation WRT GPIO libraries (and specifically for programs written in C) as of 2025-11-26.

* `WiringPi` [WiringPi](http://wiringpi.com/) seems to have been the most popular but has been abandoned by the original developer. There is a popular fork that appears to be under development by volunteers. The most recent commit was 2025-06-06.
* `pigpio` and `lg` also looked good, but the dev (Joan) got tired of trying to keep up with kernel changes and these seem to be languishing. (<https://github.com/joan2937/pigpio>, <https://github.com/joan2937/lg>) (Four years and one year since last commit, respectively.)
* `libgpiod` seems to be the heir apparent and V2.2 is now in the Trixie repos for both Debian And RPiOS. The most recent commit was 9 days ago (<https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git/>.)

## Plan

Configure a Pi 3B+ with the 32 bit variant of RPiOS [^1] and connect an LED that can be driven and a pushbutton that can be sampled to support the tests. 

The circuit is configured to use GPIO 20 as an input and GPIO 8 as an output.

```text
   3V3
    |
    R = 13K ohm
    |
    |_____ GPIO 20 (pin 38)
    |
     / Momentary pushbutton
    |
    |
 Ground
```

```text
     _____ GPIO 8 (pin 24)
    |
    R = 470 ohm
    |
    |
    D LED
    |
 Ground
```

## Requirements

* Install the following packages `gpiod libgpiod-dev libgpiod-doc libgpiod2`. `libgpiod-doc` is no optional as the online documentation is current with this release (<https://libgpiod.readthedocs.io/en/latest/>.)

```text
sudo apt install gpiod libgpiod3 libgpiod-dev
```

## References


* Github repo <https://github.com/brgl/libgpiod>
* Current kernel repo <https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git/about/>
* `libgpiod` `doxygen` docs <https://libgpiod.readthedocs.io/en/latest/index.html> (V2)

## GPIO access

This is not an issue with RPiOS and testiung with Debian has not yet been done. Ownership tweaks are listed in the `GPIOD_V1.6` branch.

## Status 

* 2025-11-26 Revise this README.
* 2025-11-26 Update CLI operations.

The following section has been updated.

|name|Doc page|dir|status|
|---|---|---|---|
|CLI exploration|<https://libgpiod.readthedocs.io/en/latest/gpio_tools.html>|`CLI`|complete|
|GPIO chip|<https://libgpiod.readthedocs.io/en/latest/core_chips.html#>|01-GPIO-Chip|complete|

False V2.2 start

|name|Doc page|dir|status|
|---|---|---|---|
|libgpiod core API|https://libgpiod.readthedocs.io/en/latest/core_api.html|`Core`|chip, line info (complete)|
|Setting line configuration|<https://libgpiod.readthedocs.io/en/latest/core_line_config.html>|`line_config`|WIP|
|Reading & setting line values|<file:///usr/share/doc/libgpiod-dev/html/group__line__value.html>|`line_IO`|not yet|

The following sections are from the `GPIOD_V1.6` branch.

|name|Doc page|dir|status|
|---|---|---|---|
|High-level API|<file:///usr/share/doc/libgpiod-dev/html/group__high__level.html>|`C_blinky`|done|
|GPIO chip operations|<file:///usr/share/doc/libgpiod-dev/html/group__chips.html>|`chip_operations`|done|
|Line info|<file:///usr/share/doc/libgpiod-dev/html/group__line__info.html>|`line_info`|done|
|Line requests|<file:///usr/share/doc/libgpiod-dev/html/group__line__request.html>|`line_request`|done|
|Line events handling|<file:///usr/share/doc/libgpiod-dev/html/group__line__event.html>|`line_events`|done|

[^1]: Most of my use of GPIO code is on Pi Zeroes so that is the environment chosen for this test. Using Debian on a Pi Zero with the 1-wire protocol has been a challenge.
