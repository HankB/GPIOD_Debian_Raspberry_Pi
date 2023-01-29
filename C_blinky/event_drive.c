/*
* Monitor an event for GPIO 7 When it goes high, turn on the LED for 1/2
* second.

Build:
   gcc -Wall -o event_drive event_drive.c -l gpiod
*/

#include <stdio.h>
#include <unistd.h>
#include <gpiod.h>

const unsigned int gpio_read = 7;
const unsigned int gpio_drive = 8;
const struct timespec timeout = {10, 0}; // 10s

// callback to keep output on for 1/2 s
void drive_cb(void *unused)
{
    usleep(500000);
}

// callback to drive the output on rising edge
int event_cb(int i, unsigned int j, const struct timespec *ts, void *unused)
{
    printf("i: %d, j:%d\n", i, j);
    if (j == 0)
        return 1;
    int rc = gpiod_ctxless_set_value("/dev/gpiochip0",
                                     gpio_drive, 1, false,
                                     "consumer", drive_cb, 0);
    if (rc < 0)
    {
        perror("gpiod_ctxless_get_value");
        return 1;
    }

    return 0;
}

int main(int argc, char **argv)
{
    int rc = gpiod_ctxless_event_monitor("/dev/gpiochip0",
                                         GPIOD_CTXLESS_EVENT_RISING_EDGE,
                                         gpio_read, false, "consumer",
                                         &timeout, 0, event_cb, 0);
    if (rc < 0)
        perror("gpiod_ctxless_get_value");
    printf("Hello World, rc is %d\n", rc);
}
