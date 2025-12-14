# Capstone

Reflect the state of a GPIO input to a GPIO output. The H/W configuration will be a pushbutton triggering GPIO 20 and an output driving an LED via GPIO 8.

## 2025-12-14 Motivation

* Having found the [libgpiod examples](https://github.com/brgl/libgpiod/tree/master/examples) when working on my last example, I wanted to employ some of the coding techniques displayed in the `libgoiod` examples, namely:
    * release some resources when no longer needed.
    * Use `goto`s to release the resources already acquired when an error is encountered.
* Demonstrate responding to an input by changing an output. (I have another project that uses this pattern.)
* Provide a simplified project that involves both GPIO input and output, albeit waiting for the input change vs. reading (polling) the input.

## 2025-12-14 Plan

### Functions

* `struct gpiod_line_request *init_input_gpio(unsigned int GPIO_number)` patterned after `request_input_line()` in `watch_line_value.c`.
* `struct gpiod_edge_event_buffer *configure_edge_events()` (because init_input_gpio can conveniently return only one value.)
* `struct gpiod_line_request *init_output_gpio(unsigned int GPIO_number)` patterned after `request_output_line()` in `toggle_line_value.c`.
* `bool wait_for_gpio_change(struct gpiod_line_request *)` returns the new state of the chip.
* `void set_output_state(struct gpiod_line_request *, bool stat4e)` Sets output to desired state.

Consumer and GPIO Chip path are predefined and (path) relative to the first chip on a Raspberry Pi.

## 2025-12-14 Build

```text
gcc -o follow_input -Wall follow_input.c -lgpiod
```

## Requirements

For RPiOS

```text
sudo apt install build-essential libgpiod-dev
```

For Debian Bookworm first uncomment backports in `/etc/apt/sources.list` and run `sudo apt update`.

```text
sudo apt install build-essential
sudo apt install -t bookworm-backports libgpiod-dev
```
