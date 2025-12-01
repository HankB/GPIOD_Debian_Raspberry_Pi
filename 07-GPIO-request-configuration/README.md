# GPIO chip

* <https://libgpiod.readthedocs.io/en/latest/core_line_settings.html>

Current testing is with a Pi 3B+ running the 32 bit variant of RPiOS/Trixie.

## Requirements

```text
sudo apt install libgpiod-dev
```

## 2025-11-27 explore GPIO line configuration API

> Functions for manipulating request configuration objects.

Namely "consumer" and event buffer size. Event buffer size seems constrained only by the allowable values for `size_t` (on a 32 bit host.)

```text
gcc -Wall -o request-configuration request-configuration.c -l gpiod
```

```text
hbarta@drogo:~/Programming/GPIOD_Debian_Raspberry_Pi/07-GPIO-request-configuration $ ./request-configuration API version:2.2.1:
Request configuration object acquired
Request configurationconsumer set to 'request_config'
Default event buffer size 0
Event buffer size requested 2, realized 2
Event buffer size requested 4, realized 4
Event buffer size requested 8, realized 8
Event buffer size requested 16, realized 16
Event buffer size requested 32, realized 32
Event buffer size requested 64, realized 64
Event buffer size requested 128, realized 128
Event buffer size requested 256, realized 256
Event buffer size requested 512, realized 512
Event buffer size requested 1024, realized 1024
Event buffer size requested 2048, realized 2048
Event buffer size requested 4096, realized 4096
Event buffer size requested 8192, realized 8192
Event buffer size requested 16384, realized 16384
Event buffer size requested 32768, realized 32768
Event buffer size requested 65536, realized 65536
Event buffer size requested 131072, realized 131072
Event buffer size requested 262144, realized 262144
Event buffer size requested 524288, realized 524288
Event buffer size requested 1048576, realized 1048576
Event buffer size requested 2097152, realized 2097152
Event buffer size requested 4194304, realized 4194304
Event buffer size requested 8388608, realized 8388608
Event buffer size requested 16777216, realized 16777216
Event buffer size requested 33554432, realized 33554432
Event buffer size requested 67108864, realized 67108864
Event buffer size requested 134217728, realized 134217728
Event buffer size requested 268435456, realized 268435456
Event buffer size requested 536870912, realized 536870912
Event buffer size requested 1073741824, realized 1073741824
Event buffer size requested 2147483648, realized 2147483648
Event buffer size requested 0, realized 0
hbarta@drogo:~/Programming/GPIOD_Debian_Raspberry_Pi/07-GPIO-request-configuration $
```
