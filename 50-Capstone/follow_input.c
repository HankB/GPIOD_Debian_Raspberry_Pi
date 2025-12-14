/*
Drive output on GPIO 8 to match the input on GPIO 24

Logs of copy and paste from examples at 
https://github.com/brgl/libgpiod/tree/master/examples
*/

#include <errno.h>
#include <gpiod.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *chip_name = "/dev/gpiochip0";
static const char *consumer = "follower";
struct gpiod_edge_event_buffer *edge_event_buffer;