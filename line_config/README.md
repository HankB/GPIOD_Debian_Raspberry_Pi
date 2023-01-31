# Setting line configuration

Exerecised one API from <https://libgpiod.readthedocs.io/en/latest/group__line__config.html>.

## `line_cfg`

```text
gcc -Wall -o line_cfg line_cfg.c -l gpiod
```

For testing this an LED was connected to the output of GPIO 8 with a 330 ohm series resistor. With GPIO 8 in default configuration (input with pullup) Th3e LED glows dimly. When set to output it is either bright or off, depending on the output state. Delays are inserted in the code to help make this obvious.

```text
hbarta@bullpi3b:~/Programming/GPIOD_Debian_Raspberry_Pi/line_cfg$ ./line_cfg 
Is free        SPI_CE0_N is yes
Is requested   SPI_CE0_N is no
Bias of        SPI_CE0_N is 1
Consumer of    SPI_CE0_N is (null)
Direction of   SPI_CE0_N is 2
Is             SPI_CE0_N used no
=====
Is free        SPI_CE0_N is no
Is requested   SPI_CE0_N is yes
Bias of        SPI_CE0_N is 1
Consumer of    SPI_CE0_N is line_cfg
Direction of   SPI_CE0_N is 2
Is             SPI_CE0_N used yes
============

=====
Is free        SPI_CE0_N is no
Is requested   SPI_CE0_N is yes
Bias of        SPI_CE0_N is 1
Consumer of    SPI_CE0_N is line_cfg
Direction of   SPI_CE0_N is 2
Is             SPI_CE0_N used yes
============

hbarta@bullpi3b:~/Programming/GPIOD_Debian_Raspberry_Pi/line_cfg$ 
```