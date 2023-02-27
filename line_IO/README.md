# Reading & setting line values

Finally get to play with I/O using APIs described at <https://libgpiod.readthedocs.io/en/latest/group__line__value.html>.

The same LED setup was used as with [line config](../line_config/README.md)

# `read_write`

```text
gcc -Wall -o read_write read_write.c -l gpiod
```


Clone of `../line_config/line_cfg.c` with the following changes.

* `gpiod_line_get_value()` is added to `report_line_attributes()`
* `gpiod_line_set_value()` is used to drive the line low.

```text
hbarta@bullpi3b:~/Programming/GPIOD_Debian_Raspberry_Pi/line_IO$ ./read_write 
===== line found, unconfigured
Is free        SPI_CE0_N is yes
Is requested   SPI_CE0_N is no
Bias of        SPI_CE0_N is 1
Consumer of    SPI_CE0_N is (null)
Direction of   SPI_CE0_N is 2
Is             SPI_CE0_N used no
Value          SPI_CE0_N is -1
===== line configured for write, default high
Is free        SPI_CE0_N is no
Is requested   SPI_CE0_N is yes
Bias of        SPI_CE0_N is 1
Consumer of    SPI_CE0_N is line_cfg
Direction of   SPI_CE0_N is 2
Is             SPI_CE0_N used yes
Value          SPI_CE0_N is 1
============

===== driven low
Is free        SPI_CE0_N is no
Is requested   SPI_CE0_N is yes
Bias of        SPI_CE0_N is 1
Consumer of    SPI_CE0_N is line_cfg
Direction of   SPI_CE0_N is 2
Is             SPI_CE0_N used yes
Value          SPI_CE0_N is 0
============

hbarta@bullpi3b:~/Programming/GPIOD_Debian_Raspberry_Pi/line_IO$ 
```
