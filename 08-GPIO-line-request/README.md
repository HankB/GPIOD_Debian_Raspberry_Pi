# GPIO chip

* <https://libgpiod.readthedocs.io/en/latest/core_line_request.html>

Current testing is with a Pi 3B+ running the 32 bit variant of RPiOS/Trixie.

This API set starts to pull together some of the other APIs and in particular from "GPIO chip"

```text
 struct gpiod_line_request *gpiod_chip_request_lines(
       struct gpiod_chip *chip,                         // GPIO chip
       struct gpiod_request_config *req_cfg,            // GPIO line request
       struct gpiod_line_config *line_cfg)              // GPIO line configuration
 ```

Other APIs are used to configure the structs passed to `gpiod_chip_request_lines()`. This demonstration (at present) works through the steps (seemingly) required to drive a GPIO which lights the attached LED for 1s.

## Requirements

```text
sudo apt install libgpiod-dev
```

## 2025-11-27 explore GPIO Chip API

```text
gcc -Wall -o line-request line-request.c -l gpiod
```

```text
hbarta@drogo:~/Programming/GPIOD_Debian_Raspberry_Pi/08-GPIO-line-request $ ./line-request 
API version:2.2.1:
opened chip '/dev/gpiochip0'

Request configuration object acquired
Request consumer set to 'line request'
Default event buffer size 0
Line configuration object acquired
Settings object acquired
Settings object set to output
line set to output and set active
gpiod_line_settings added to gpiod_line_config
Event buffer size requested 10, realized 10
Line request object acquired

            chip name:gpiochip0
      requested lines:1
            offset at:8 is 0

gpiod_line_request_set_value():0

            chip name:gpiochip0
      requested lines:1
            offset at:8 is 1

gpiod_line_request_set_value():0

            chip name:gpiochip0
      requested lines:1
            offset at:8 is 0
hbarta@drogo:~/Programming/GPIOD_Debian_Raspberry_Pi/08-GPIO-line-request $ 
```

## 2025-12-02 add an input

Q: Some of these calls accept arrays of "lines". Can an array include both input and output? First step will be to configure the read GPIO separately and then once working, see what if any can be combined.

Input is now working but reduction is not yet tried.

```text
hbarta@drogo:~/Programming/GPIOD_Debian_Raspberry_Pi/08-GPIO-line-request $ ./line-request 
API version:2.2.1:
opened chip '/dev/gpiochip0'

Request configuration object acquired
Request consumer set to 'line request'
Default event buffer size 0
Line configuration object acquired
Settings object acquired
Settings object set to output
line set to output and set active
gpiod_line_settings added to gpiod_line_config
Event buffer size requested 10, realized 10
Line request object acquired

            chip name:gpiochip0
      requested lines:1
            offset at:8 is 0

gpiod_line_request_set_value():0

            chip name:gpiochip0
      requested lines:1
            offset at:8 is 1

gpiod_line_request_set_value():0

            chip name:gpiochip0
      requested lines:1
            offset at:8 is 0
Request input configuration object acquired
Request consumer set to 'line request'
Default event buffer size 0
Line configuration object acquired
Settings object acquired
Settings object set to input
line set to output and set active
gpiod_line_settings added to gpiod_line_config

            chip name:gpiochip0
      requested lines:1
            offset at:20 is 1

            chip name:gpiochip0
      requested lines:1
            offset at:20 is 1

            chip name:gpiochip0
      requested lines:1
            offset at:20 is 0

            chip name:gpiochip0
      requested lines:1
            offset at:20 is 0

            chip name:gpiochip0
      requested lines:1
            offset at:20 is 1

            chip name:gpiochip0
      requested lines:1
            offset at:20 is 1

            chip name:gpiochip0
      requested lines:1
            offset at:20 is 1

            chip name:gpiochip0
      requested lines:1
            offset at:20 is 1

            chip name:gpiochip0
      requested lines:1
            offset at:20 is 1

            chip name:gpiochip0
      requested lines:1
            offset at:20 is 1
hbarta@drogo:~/Programming/GPIOD_Debian_Raspberry_Pi/08-GPIO-line-request $ 
```

Last 10 repeats of line information reflect '`' when the switch is open and '0' when the switch is closed and the input pulled to ground.
