# GPIOD_Debian_Raspberry_Pi

Explore GPIOD on Raspberry Pi running Debian Linux

## Motivation

Recent explorations demonstrate that Debian (not Debian based Raspberry Pi OS - R-Pi OS) runs well on the Pi Zero and Pi 3B+. One question regarding this is the usability of GPIO which is one of the useful capabilities of the Raspberry Pi.

## GPIO libraries

Here is a brief survey of the current situation WRT GPIO libraries (and specifically for programs written in C) as of 2024-06-16.

* `WiringPi` [WiringPi](http://wiringpi.com/) was the most popular but appears to have been abandoned by the original developer. There is a popular fork that appears to be under active development by volunteers. They claim [We're not dead yet!](https://github.com/WiringPi/WiringPi/discussions/202) but a year ago it looked like it was languishing.
* `gpiod` and `lg` also looked good, but the dev (Joan) got tired of trying to keep up with kernel changes and these seem to be languishing. (<https://github.com/joan2937/pigpio>, <https://github.com/joan2937/lg>)
* `libgpiod` seems to be the heir apparent, available for both Debian and RpiOS. But the fly in the ointment is that there is a V2 avaiable, but not for us. At present V2 is only available on Debian in the experimental repo. There has been mention in the RpiOS forums that the entineers would like to backport V2 to RpiOS but that has not yet happened.

## Plan

Configure a Pi CM4 on a board with GPIO pins (Bicool Mini-base) and connect an LED that can be driven and a pushbutton that can be sampled to support the tests.

The circuit is configured to use GPIO 20 as an input and GPIO 8 as an output.

```text
   3V3
    |
    R = 6.66K ohm
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
    R = 220 ohm
    |
    |
    D LED
    |
 Ground
```

## Requirements

* Install the following packages `gpiod libgpiod-dev libgpiod-doc libgpiod2`. (`libgpiod2` is confusingly named as it is for version 1.6.2 of the library.) The document links in the READMEs all reference the on-disk documentation.

## References

This exploration proceeds with the version of `libgpiod` presently available for Debian/RpiOS. The online documentation seems to reference the V2 library whereas V 1.6 is found in the repos.

* Github repo <https://github.com/brgl/libgpiod> (V2)
* Current kernel repo <https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git/>
* `libgpiod` `doxygen` docs <https://libgpiod.readthedocs.io/en/latest/index.html> (V2)
* `libgpiod` `doxygen` docs <file:///usr/share/doc/libgpiod-dev/html/index.html> V1, on local disk. Unfortunately browser security causes browsers to not want to open local files so the user will need to work to open these (e.g. copy the `file:///` link and open in another window.)

## GPIO access

On Debian the GPIO devices are owned by `root` and commands that access them thus require root. I have temporarily fixed that by adding the `gpio` group to `/etc/group` and changing the ownership and permissions for the device nodes. (The `/etc/group` entry is copied from Raspbian.)

```text
sudo vim /etc/group
sudo chown root.gpio /dev/gpiochip?
sudo chmod g+rw /dev/gpiochip?
```

with the result

```text
hbarta@bullpi3b:~$ grep gpio /etc/group
gpio:x:997:hbarta
hbarta@bullpi3b:~$ ls -l /dev/gpiochip0
crw-rw---- 1 root gpio 254, 0 Aug  7 08:25 /dev/gpiochip0
hbarta@bullpi3b:~$ 
```

`gpioinfo` et al now report results w/out the need for root access.

## Status 

Once again as work in progress (as of 2024-06-15.) At this point I have reviewed and repeated `CLI` and `C_blinky` and revised my notes along with some code modifications. I have identified APIs to try for my other project (see `C_blinky/event.c`) and will return my attention to that. 

Complete for now.

Complete unless the need for further exploration is encountered. That need came up when looking for a better way to [implement an HC-SR04 sonar distance sensor](https://github.com/HankB/interrupt-driven-HC-SR04). At present I am fixing the broken documentation links and repeating the operations on both Debian and RpiOS to see if they still work. Of course (rolls eyes) as I proceed I can see that a lot of the instructions are deficient and will strive to fill the gaps.

Work in progress working through first CLI commands then various protions of the [libgpiod](https://libgpiod.readthedocs.io/en/latest/index.html) (`doxygen`) documentation.

Note: The first entry `CLI` links to information about the V2 library. The best V1 information I found was to execute any of the commands using the `-h` option to provide information on the V1 variant. I did not compare the differences between V1 and V2 CLI commands because they don't particularly interest me, though some are useful.

|name|Doc page|dir|status|
|---|---|---|---|
|CLI exploration|<https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git/about/>|`CLI`|done|
|High-level API|<file:///usr/share/doc/libgpiod-dev/html/group__high__level.html>|`C_blinky`|done|
|GPIO chip operations|<file:///usr/share/doc/libgpiod-dev/html/group__chips.html>|`chip_operations`|done|
|Line info|<file:///usr/share/doc/libgpiod-dev/html/group__line__info.html>|`line_info`|done|
|Line requests|<file:///usr/share/doc/libgpiod-dev/html/group__line__request.html>|`line_request`|done|
|Setting line configuration|<file:///usr/share/doc/libgpiod-dev/html/group__line__config.html>|`line_config`|done|
|Reading & setting line values|<file:///usr/share/doc/libgpiod-dev/html/group__line__value.html>|`line_IO`|done|
|Line events handling|<file:///usr/share/doc/libgpiod-dev/html/group__line__event.html>|`line_events`|done|
