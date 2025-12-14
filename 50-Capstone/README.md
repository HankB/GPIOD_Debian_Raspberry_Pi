# Capstone

Reflect the state of a GPIO input to a GPIO output. The H/W configuration will be a pushbutton triggering GPIO 20 and an output driving an LED via GPIO 8.

## Motivation

* Having found the [libgpiod examples](https://github.com/brgl/libgpiod/tree/master/examples) when working on my last example, I wanted to employ some of the coding techniques displayed in the `libgoiod` examples, namely:
    * release some resources when no longer needed.
    * Use `goto`s to release the resources already acquired when an error is encountered.
* Demonstrate responding to an input by changing an output. (I have another project that uses this pattern.)
* Provide a simplified project that involves both GPIO input and output, albeit waiting for the input change vs. reading (polling) the input.
