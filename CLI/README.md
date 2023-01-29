# GPIO access via CLI

The `gpiod` package includes the following commands as listed by `dpkg -L gpiod`.

```text
/usr/bin/gpiodetect
/usr/bin/gpiofind
/usr/bin/gpioget
/usr/bin/gpioinfo
/usr/bin/gpiomon
/usr/bin/gpioset
```

The `man` pages for these commands are pretty terse. Some information and examples are found at <https://lloydrochester.com/post/hardware/libgpiod-intro-rpi/>

## `gpiodetect`

```text
hbarta@bullpi3b:~$ gpiodetect
gpiochip0 [pinctrl-bcm2835] (54 lines)
gpiochip1 [raspberrypi-exp-gpio] (8 lines)
hbarta@bullpi3b:~$ 
```

## `gpiofind` and `gpioinfo`

These report information about the system.

```text
hbarta@bullpi3b:~$ gpioinfo 
gpiochip0 - 54 lines:
        line   0:     "ID_SDA"       unused   input  active-high 
        line   1:     "ID_SCL"       unused   input  active-high 
        line   2:       "SDA1"       unused   input  active-high 
        line   3:       "SCL1"       unused   input  active-high 
        line   4:  "GPIO_GCLK"       unused   input  active-high 
        line   5:      "GPIO5"       unused   input  active-high 
...
gpiochip1 - 8 lines:
        line   0:      "BT_ON"   "shutdown"  output  active-high [used]
        line   1:      "WL_ON"      "reset"  output   active-low [used]
        line   2:  "PWR_LED_R"        "PWR"  output   active-low [used]
        line   3:    "LAN_RUN"       unused  output  active-high 
...
hbarta@bullpi3b:~$ gpiofind GPIO5
gpiochip0 5
hbarta@bullpi3b:~$ 
```

## `gpioget`

The GPIO chosen is #7 (pin 26) since it is convenient to use as a shutdown/reboot switch. That has an alternate function `SPI_CE1_N` and is identified that way by `gpioinfo`.

```text
hbarta@bullpi3b:~$ gpioinfo | grep "SPI_CE1_N"
        line   7:  "SPI_CE1_N"       unused   input  active-high 
hbarta@bullpi3b:~$ gpioget -l /dev/gpiochip0 7
0
hbarta@bullpi3b:~$ gpioget -l /dev/gpiochip0 7
1
hbarta@bullpi3b:~$ 
```

Checking with a DVM, the pin is at 3.28V. The description from `gpioinfo` lists it as active high, but it could be inverted by circuitry external to the chip. Next check is to connect to ground to pull it low. That results in no change to the reading. Voltae is confirmed at 0V using the DVM.

## `gpioset`

Drive an LED with a 330 ohm series resistor from GPIO8 (`SPI_CE1_N`). When connected it glows dimly when the GPIO is configured as an input, likely driven by the pullup. The following command turns the LED on momentarily and the next turns on until the the user hits `<enter>`

```text
hbarta@bullpi3b:~$ gpioset -m time -u 500 /dev/gpiochip0 8=1
hbarta@bullpi3b:~$ gpioset -m wait /dev/gpiochip0 8=1
```

There does not appear to be a way to have `gpioset` leave the GPIO configured on exit. It appears to put the GPIO back to input on exit.

## `gpimon`

Conneting a momentary pushbutton between GPIO 7 and ground shows just how bouncy the switch is. For example one press close (short) and one release (open) produces the following events. And all events are not captured.

```text
hbarta@bullpi3b:~$ gpiomon  /dev/gpiochip0 7
event: FALLING EDGE offset: 7 timestamp: [   69913.990484974] # press
event: FALLING EDGE offset: 7 timestamp: [   69915.972319494] # release
event: FALLING EDGE offset: 7 timestamp: [   69915.972457982]
event: FALLING EDGE offset: 7 timestamp: [   69915.972673396]
event:  RISING EDGE offset: 7 timestamp: [   69915.972804383]
event: FALLING EDGE offset: 7 timestamp: [   69915.972841310]
event: FALLING EDGE offset: 7 timestamp: [   69915.972947454]
event: FALLING EDGE offset: 7 timestamp: [   69915.972985683]
event: FALLING EDGE offset: 7 timestamp: [   69915.973027349]
event:  RISING EDGE offset: 7 timestamp: [   69915.973107556]
^Chbarta@bullpi3b:~$ 
```
