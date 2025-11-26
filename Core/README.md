# libgpiod core API

This will probably replace `../C_Blink` from `GPIOD_V1.6` as it seems the contextless operations have been removed. Similar tasks will be performed with perhaps some additions due to the lower level nature of the API.

Current testing is with a Pi 3B+ running the 32 bit variant of RPiOS/Trixie.

## Requirements

```text
sudo apt install libgpiod-dev
```

## 2025-11-26 chip

`chip.c`, open and close the chip.

```text
gcc -Wall -o chip chip.c -l gpiod
```

```text
hbarta@drogo:~/Programming/GPIOD_Debian_Raspberry_Pi/Core $ ./chip
API version:2.2.1:
opened chip '/dev/gpiochip0'
hbarta@drogo:~/Programming/GPIOD_Debian_Raspberry_Pi/Core $ 
```

## 2025-11-26 chip info

Add to `chip.c`, read and print info.

```text
hbarta@drogo:~/Programming/GPIOD_Debian_Raspberry_Pi/Core $ ./chip
API version:2.2.1:
opened chip '/dev/gpiochip0'
  Info for chip:/dev/gpiochip0
           name:gpiochip0
          label:pinctrl-bcm2835
number of lines:54
hbarta@drogo:~/Programming/GPIOD_Debian_Raspberry_Pi/Core $ 
```
