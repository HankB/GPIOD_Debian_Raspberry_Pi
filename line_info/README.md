# Line info

Explore interesting APIs described in <https://libgpiod.readthedocs.io/en/latest/group__line__info.html> 

## `line`

```text
gcc -Wall -o line line.c -l gpiod
```

```text
hbarta@bullpi3b:~/Programming/GPIOD_Debian_Raspberry_Pi/line_info$ ./line
found 3 lines
Offset of      GPIO16 is 16
Hi/lo state of GPIO16 is 1
Bias of        GPIO16 is 1
Consumer of    GPIO16 is (null)
Direction of   GPIO16 is 1
Is             GPIO16 used no

Offset of      GPIO17 is 17
Hi/lo state of GPIO17 is 1
Bias of        GPIO17 is 1
Consumer of    GPIO17 is (null)
Direction of   GPIO17 is 1
Is             GPIO17 used no

Offset of      STATUS_LED_G is 29
Hi/lo state of STATUS_LED_G is 1
Bias of        STATUS_LED_G is 1
Consumer of    STATUS_LED_G is ACT
Direction of   STATUS_LED_G is 2
Is             STATUS_LED_G used yes

hbarta@bullpi3b:~/Programming/GPIOD_Debian_Raspberry_Pi/line_info$ 
```
