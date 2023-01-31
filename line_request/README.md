# Line requests

Interesting APIs from <https://libgpiod.readthedocs.io/en/latest/group__line__request.html>

## `line_rqst`

```text
gcc -Wall -o line_rqst line_rqst.c -l gpiod
```

```text
hbarta@bullpi3b:~/Programming/GPIOD_Debian_Raspberry_Pi/line_rqst$ ./line_rqst 
found 3 lines

untouched =====
Is free        SPI_CE1_N is yes
Is requested   SPI_CE1_N is no
Bias of        SPI_CE1_N is 1
Consumer of    SPI_CE1_N is (null)
Direction of   SPI_CE1_N is 1
Is             SPI_CE1_N used no
line_request() SPI_CE1_N is 0
requested =====
Is free        SPI_CE1_N is no
Is requested   SPI_CE1_N is yes
Bias of        SPI_CE1_N is 1
Consumer of    SPI_CE1_N is line_rqst
Direction of   SPI_CE1_N is 1
Is             SPI_CE1_N used yes
released ======
Is free        SPI_CE1_N is yes
Is requested   SPI_CE1_N is no
Bias of        SPI_CE1_N is 1
Consumer of    SPI_CE1_N is line_rqst
Direction of   SPI_CE1_N is 1
Is             SPI_CE1_N used yes
===============

untouched =====
Is free        SPI_CE0_N is yes
Is requested   SPI_CE0_N is no
Bias of        SPI_CE0_N is 1
Consumer of    SPI_CE0_N is (null)
Direction of   SPI_CE0_N is 1
Is             SPI_CE0_N used no
line_request() SPI_CE0_N is 0
requested =====
Is free        SPI_CE0_N is no
Is requested   SPI_CE0_N is yes
Bias of        SPI_CE0_N is 1
Consumer of    SPI_CE0_N is line_rqst
Direction of   SPI_CE0_N is 1
Is             SPI_CE0_N used yes
released ======
Is free        SPI_CE0_N is yes
Is requested   SPI_CE0_N is no
Bias of        SPI_CE0_N is 1
Consumer of    SPI_CE0_N is line_rqst
Direction of   SPI_CE0_N is 1
Is             SPI_CE0_N used yes
===============

untouched =====
Is free        STATUS_LED_G is yes
Is requested   STATUS_LED_G is no
Bias of        STATUS_LED_G is 1
Consumer of    STATUS_LED_G is ACT
Direction of   STATUS_LED_G is 2
Is             STATUS_LED_G used yes
line_request() STATUS_LED_G is -1
requested =====
               gpiod_line_request(): Device or resource busy
Is free        STATUS_LED_G is yes
Is requested   STATUS_LED_G is no
Bias of        STATUS_LED_G is 1
Consumer of    STATUS_LED_G is ACT
Direction of   STATUS_LED_G is 2
Is             STATUS_LED_G used yes
released ======
Is free        STATUS_LED_G is yes
Is requested   STATUS_LED_G is no
Bias of        STATUS_LED_G is 1
Consumer of    STATUS_LED_G is ACT
Direction of   STATUS_LED_G is 2
Is             STATUS_LED_G used yes
===============

hbarta@bullpi3b:~/Programming/GPIOD_Debian_Raspberry_Pi/line_rqst$ 
```
