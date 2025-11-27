# GPIO access via CLI

The `gpiod` package includes the following commands as listed by `dpkg -L gpiod`.

```text
/usr/bin/gpiodetect
/usr/bin/gpioget
/usr/bin/gpioinfo
/usr/bin/gpiomon
/usr/bin/gpionotify
/usr/bin/gpioset
```

The `man` pages for these commands have been expanded considerably and likewise the `--help` output. The V1.6 version of this document also referenced <https://lloydrochester.com/post/hardware/libgpiod-intro-rpi/> and I have not verified if that is still relevant.

## `gpiodetect`

```text
hbarta@drogo:~ $ gpiodetect
gpiochip0 [pinctrl-bcm2835] (54 lines)
gpiochip1 [raspberrypi-exp-gpio] (8 lines)
hbarta@drogo:~ $ 
```

## `gpioinfo`

```text
hbarta@drogo:~ $ gpioinfo
gpiochip0 - 54 lines:
        line   0:       "ID_SDA"                input
        line   1:       "ID_SCL"                input
        line   2:       "GPIO2"                 input
        line   3:       "GPIO3"                 input
        line   4:       "GPIO4"                 input
        line   5:       "GPIO5"                 input
        line   6:       "GPIO6"                 input
        line   7:       "GPIO7"                 input active-low consumer="shutdown"
        line   8:       "GPIO8"                 input
        line   9:       "GPIO9"                 input
        line  10:       "GPIO10"                input
        line  11:       "GPIO11"                input
        line  12:       "GPIO12"                input
        line  13:       "GPIO13"                input
        line  14:       "GPIO14"                input
        line  15:       "GPIO15"                input
        line  16:       "GPIO16"                input
        line  17:       "GPIO17"                input
        line  18:       "GPIO18"                input
        line  19:       "GPIO19"                input
        line  20:       "GPIO20"                input
        line  21:       "GPIO21"                input
        line  22:       "GPIO22"                input
        line  23:       "GPIO23"                input
        line  24:       "GPIO24"                input
        line  25:       "GPIO25"                input
        line  26:       "GPIO26"                input
        line  27:       "GPIO27"                input
        line  28:       "HDMI_HPD_N"            input active-low consumer="hpd"
        line  29:       "STATUS_LED_G"          output consumer="ACT"
        line  30:       "CTS0"                  input
        line  31:       "RTS0"                  input
        line  32:       "TXD0"                  input
        line  33:       "RXD0"                  input
        line  34:       "SD1_CLK"               input
        line  35:       "SD1_CMD"               input
        line  36:       "SD1_DATA0"             input
        line  37:       "SD1_DATA1"             input
        line  38:       "SD1_DATA2"             input
        line  39:       "SD1_DATA3"             input
        line  40:       "PWM0_OUT"              input
        line  41:       "PWM1_OUT"              input
        line  42:       "ETH_CLK"               input
        line  43:       "WIFI_CLK"              input
        line  44:       "SDA0"                  input
        line  45:       "SCL0"                  input
        line  46:       "SMPS_SCL"              input
        line  47:       "SMPS_SDA"              output
        line  48:       "SD_CLK_R"              input
        line  49:       "SD_CMD_R"              input
        line  50:       "SD_DATA0_R"            input
        line  51:       "SD_DATA1_R"            input
        line  52:       "SD_DATA2_R"            input
        line  53:       "SD_DATA3_R"            input
gpiochip1 - 8 lines:
        line   0:       "BT_ON"                 output consumer="shutdown"
        line   1:       "WL_ON"                 output
        line   2:       "PWR_LED_R"             output active-low consumer="PWR"
        line   3:       "LAN_RUN"               output
        line   4:       "NC"                    input
        line   5:       "CAM_GPIO0"             output consumer="cam1_regulator"
        line   6:       "CAM_GPIO1"             output
        line   7:       "NC"                    input
hbarta@drogo:~ $ 
```

## `gpioget`

The GPIO chosen is #20 (pin 38) for no goiod reason. Its name is`GPIO20` and it is identified as such by `gpioinfo`.

```text
hbarta@drogo:~ $ gpioinfo | grep GPIO20
        line  20:       "GPIO20"                input
hbarta@drogo:~ $ gpioget GPIO20 # SW open, 2.6V on input
"GPIO20"=active
hbarta@drogo:~ $ gpioget GPIO20 # SW closed, 0V on input
"GPIO20"=inactive
hbarta@drogo:~ $ 
```

## `gpioset`

Drive an LED with a 470 ohm series resistor from `GPIO8`. The following command turns the LED on and the next turns off.

```text
hbarta@drogo:~ $ gpioset GPIO8=1 # turns LED on and exits using <ctrl>C
^C
hbarta@drogo:~ $ gpioset GPIO8=0 # turns LED off and exits using <ctrl>C
^C
hbarta@drogo:~ $ 
```

By default `gpioset` does not exit but can be set to exit using the `-t` option. The following sequence turns the LED on and exits, leaving the LED on. The second command turns the LED off and similarly exits.

```text
hbarta@drogo:~ $ gpioset -t 1s -t 1 -t 0 GPIO8=1
hbarta@drogo:~ $ gpioset -t 1s -t 1 -t 0 GPIO8=0
hbarta@drogo:~ $ 
```

## `gpimon`

The following sequence captures the output when the (momentary) switch is pressed three times, demonstrating switch bounce on the last release.

```text
hbarta@drogo:~ $ gpiomon GPIO20
42016.628976252 falling "GPIO20"
42016.950150625 rising  "GPIO20"
42017.476182171 falling "GPIO20"
42017.840469645 rising  "GPIO20"
42018.266603689 falling "GPIO20"
42018.605324244 rising  "GPIO20"
42018.605495642 falling "GPIO20"
42018.605883177 falling "GPIO20"
42018.606015827 falling "GPIO20"
42018.606093532 falling "GPIO20"
42018.606243474 falling "GPIO20"
42018.606380082 rising  "GPIO20"
^C
hbarta@drogo:~ $
```

## `gpionotify`

Report changes in line info. For example it produces the following when the command `gpioset GPIO8=1` is executed in a different terminal window:

```text
hbarta@drogo:~ $ gpionotify GPIO8
42487.298348511 requested       "GPIO8"
42492.355611284 released        "GPIO8"
^C
hbarta@drogo:~ $
```
