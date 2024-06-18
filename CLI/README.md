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

The GPIO chosen is #20 (pin 26) since it is otherwiose unused. It is named `GPIO20` and is identified that way by `gpioinfo`.

```text
hbarta@higgs:~$ gpioinfo | grep GPIO20
        line  20:     "GPIO20"       unused   input  active-high 
hbarta@higgs:~$ 
hbarta@higgs:~$ gpioget -l /dev/gpiochip0 20    # pushbutton open
0
hbarta@higgs:~$ gpioget -l /dev/gpiochip0 20    # pushbutton pressed
1
hbarta@higgs:~$ 
```

Checking with a DVM, the pin is at 3.28V. The description from `gpioinfo` lists it as active high, but it could be inverted by circuitry external to the chip. Next check is to connect to ground to pull it low. That results in no change to the reading. Voltage is confirmed at 0V using the DVM.

## `gpioset`

Drive an LED with a 330 ohm series resistor from GPIO8 (`SPI_CE1_N`). When connected it glows dimly when the GPIO is configured as an input, likely driven by the pullup. The following command turns the LED on and the next turns off.

```text
hbarta@higgs:~$ gpioset /dev/gpiochip0 8=1
hbarta@higgs:~$ gpioset /dev/gpiochip0 8=0
```

~~There does not appear to be a way to have `gpioset` leave the GPIO configured on exit. It appears to put the GPIO back to input on exit.~~ This was previous behavior. The commands now leave the bit set/cleared when the command exits.

## `gpimon`

~~Conneting a momentary pushbutton between GPIO 20 and ground shows just how bouncy the switch is. For example one press close (short) and one release (open) produces the following events. And all events are not captured.~~

The default settings seem to include some debounce (or a different switch is producing differeit results.) The following results from pressing the pushbutton three times while the monitor is running. It appears that the events are timestamped with nanosecond resolution.

```text
hbarta@higgs:~$ gpiomon /dev/gpiochip0 20
event: FALLING EDGE offset: 20 timestamp: [  263978.994698254]
event:  RISING EDGE offset: 20 timestamp: [  263979.207460594]
event: FALLING EDGE offset: 20 timestamp: [  263979.704718954]
event:  RISING EDGE offset: 20 timestamp: [  263979.950956902]
event: FALLING EDGE offset: 20 timestamp: [  263980.404080620]
event:  RISING EDGE offset: 20 timestamp: [  263980.694575096]
^Chbarta@higgs:~$ 
```
