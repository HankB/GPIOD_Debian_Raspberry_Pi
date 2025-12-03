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


* Github repo <https://github.com/brgl/libgpiod> with examples.
* Current kernel repo <https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git/about/>
* `libgpiod` `doxygen` docs <https://libgpiod.readthedocs.io/en/latest/index.html> (V2)

## GPIO access

This is not an issue with RPiOS and testing with Debian has not yet been done. Ownership tweaks are listed in the `GPIOD_V1.6` branch.

## Status 

* 2025-12-03 Portions relating to the online docs are complete.
* 2025-11-26 Revise this README.
* 2025-11-26 Update CLI operations.

The following section has been updated.

|name|Doc page|dir|status|
|---|---|---|---|
|CLI exploration|<https://libgpiod.readthedocs.io/en/latest/gpio_tools.html>|`00-CLI`|complete|
|GPIO chip|<https://libgpiod.readthedocs.io/en/latest/core_chips.html#>|`01-GPIO-chip`|complete|
|GPIO chip info|<https://libgpiod.readthedocs.io/en/latest/core_chip_info.html#>|`02-GPIO-chip-info`|complete|
|GPIO line information|https://libgpiod.readthedocs.io/en/latest/core_line_info.html#>|`03-GPIO-line-information`|complete|
|GPIO line watch|<https://libgpiod.readthedocs.io/en/latest/core_line_watch.html>|`04-GPIO-line-watch`|complete|
|GPIO line settings|https://libgpiod.readthedocs.io/en/latest/core_line_settings.html|`05-GPIO-line-settings`|complete|
|GPIO line configuration|https://libgpiod.readthedocs.io/en/latest/core_line_config.html#|`06-GPIO-line-configuration`|complete|
|GPIO request configuration|https://libgpiod.readthedocs.io/en/latest/core_request_config.html|`07-GPIO-request-configuration`|complete|
|GPIO line request|https://libgpiod.readthedocs.io/en/latest/core_line_request.html|`08-GPIO-line-request`|complete|
|GPIO edge event|https://libgpiod.readthedocs.io/en/latest/core_edge_event.html|`09-GPIO-edge-event`|complete|

[^1]: Most of my use of GPIO code is on Pi Zeroes so that is the environment chosen for this test. Using Debian on a Pi Zero with the 1-wire protocol has been a challenge.
