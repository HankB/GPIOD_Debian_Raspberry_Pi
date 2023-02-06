# GPIOD_Debian_Raspberry_Pi

Explore GPIOD on Raspberry Pi running Debian Linux

## Motivation

Recent explorations demonstrate that Debian (not Debian based Raspberry Pi OS - R-Pi OS) runs well on the Pi Zero and Pi 3B+. One question regarding this is the usability of GPIO which is one of the useful capabilities of the Raspberry Pi.

The library previously used was [WiringPi](http://wiringpi.com/) and which seems to be languishing. It's been set aside by its original developer and what seems like the [most popular fork](https://github.com/WiringPi/WiringPi) has not received an update in nearly a year. Along with looking at GPIO on the Pi, this project will use `gpiod` to explore GPIO including both shell commands and C/C++ programs.

## References

Archived Github repo <https://github.com/brgl/libgpiod>  
Current kernel repo <https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git/>  
`libgpiod` `doxygen` docs <https://libgpiod.readthedocs.io/en/latest/index.html>  

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

Work in progress working through first CLI commands then various protions of the [libgpiod](https://libgpiod.readthedocs.io/en/latest/index.html) (`doxygen`) documentation.

|name|Doc page|dir|status|
|---|---|---|---|
|CLI exploration|<https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git/about/>|`CLI`|done|
|High-level API|<https://libgpiod.readthedocs.io/en/latest/group__high__level.html>|`C_blinky`|done|
|GPIO chip operations|<https://libgpiod.readthedocs.io/en/latest/group__chips.html>|`chip_operations`|done|
|Line info|<https://libgpiod.readthedocs.io/en/latest/group__line__info.html>|`line_info`|done|
|Line requests|<https://libgpiod.readthedocs.io/en/latest/group__line__request.html>|`line_request`|done|
|Setting line configuration|<https://libgpiod.readthedocs.io/en/latest/group__line__config.html>|`line_config`|done|
|Reading & setting line values|<https://libgpiod.readthedocs.io/en/latest/group__line__value.html>|`line_IO`|next|
