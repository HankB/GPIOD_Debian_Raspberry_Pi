# Setting line configuration

Exerecised one API from <https://libgpiod.readthedocs.io/en/latest/group__line__config.html>.

## `line_cfg`

```text
gcc -Wall -o line_cfg line_cfg.c -l gpiod
```

For testing this an LED was connected to the output of GPIO 8 with a 470 ohm series resistor. 

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