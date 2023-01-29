# GPIOD_Debian_Raspberry_Pi

Explore GPIOD on Raspberry Pi running Debian Linux

## Motivation

Recent explorations demonstrate that Debian (not Debian based Raspberry Pi OS - R-Pi OS) runs well on the Pi Zero and Pi 3B+. One question regarding this is the usability of GPIO which is one of the useful capabilities of the Raspberry Pi.

The library previously used was [WiringPi](http://wiringpi.com/) and which seems to be languishing. It's been set aside by its original developer and what seems like the [most popular fork](https://github.com/WiringPi/WiringPi) has not received an update in nearly a year. Along with looking at GPIO on the Pi, this project will use `gpiod` to explore GPIO including both shell commands and C/C++ programs.

## GPIO access

On Debian the GPIO devices are owned by `root` and commands that access them thus require root. I have temporarily fixed that by adding the `gpio` group to `/etc/group` and changing the ownership and permissions for the device nodes. (The `/etc/group` entry is copied from Raspbian.)

```text
sudo vim /etc/group
sudo chown root.gpio /dev/gpiochip0
sudo chmod g+rw /dev/gpiochip0
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

Work in progress

CLI examples provided. Work started on C examples using the "high level APIs."
