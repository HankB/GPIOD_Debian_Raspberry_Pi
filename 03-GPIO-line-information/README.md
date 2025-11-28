# GPIO lione information

* <https://libgpiod.readthedocs.io/en/latest/core_lines.html>

Current testing is with a Pi 3B+ running the 32 bit variant of RPiOS/Trixie.

## Requirements

```text
sudo apt install libgpiod-dev
```

## 2025-11-27 explore GPIO line information API

```text
gcc -Wall -o line-information line-information.c -l gpiod
```

```text
hbarta@drogo:~/Programming/GPIOD_Debian_Raspberry_Pi/03-GPIO-line-information $ ./line-information 
API version:2.2.1:
opened chip '/dev/gpiochip0'

Info for line offset 0
          Offset: 0
            Name: ID_SDA
         Is used: no
        Consumer: (null)
       Direction: input
  Edge Detection: none
            Bias: unknown bias
          Drive: push pull
   Is active low: no
    Is debounced: no
 Debounce period: 0
     Event clock: monotonic

Info for line offset 8
          Offset: 8
            Name: GPIO8
         Is used: no
        Consumer: (null)
       Direction: output
  Edge Detection: none
            Bias: unknown bias
          Drive: push pull
   Is active low: no
    Is debounced: no
 Debounce period: 0
     Event clock: monotonic

Info for line offset 20
          Offset: 20
            Name: GPIO20
         Is used: no
        Consumer: (null)
       Direction: input
  Edge Detection: none
            Bias: unknown bias
          Drive: push pull
   Is active low: no
    Is debounced: no
 Debounce period: 0
     Event clock: monotonic

Info for line offset 28
          Offset: 28
            Name: HDMI_HPD_N
         Is used: yes
        Consumer: hpd
       Direction: input
  Edge Detection: none
            Bias: unknown bias
          Drive: push pull
   Is active low: yes
    Is debounced: no
 Debounce period: 0
     Event clock: monotonic

Info for line offset 29
          Offset: 29
            Name: STATUS_LED_G
         Is used: yes
        Consumer: ACT
       Direction: output
  Edge Detection: none
            Bias: unknown bias
          Drive: push pull
   Is active low: no
    Is debounced: no
 Debounce period: 0
     Event clock: monotonic
hbarta@drogo:~/Programming/GPIOD_Debian_Raspberry_Pi/03-GPIO-line-information $
```
