/*
* Explore various GPIO line event related APIs

Build
    gcc -Wall -o line_events line_events.c -l gpiod
*/

#include <stdio.h>
#include <gpiod.h>
#include <unistd.h>

static const char* consumer = "line_events";

int main(int argc, char **argv)
{
    // need to open the chip first
    struct gpiod_chip *chip = gpiod_chip_open("/dev/gpiochip0");
    if (chip == NULL)
    {
        perror("gpiod_chip_open()");
        return 1;
    }

    // acquire & configure GPIO 8 (output) GPIO 11 (events)

    struct gpiod_line *gpio_8, *gpio_11;

    gpio_8 = gpiod_chip_find_line(chip, "SPI_CE0_N");
    if (gpio_8 == NULL)
    {
        perror("gpiod_chip_find_line(SPI_CE0_N)");
        gpiod_chip_close(chip);
        return 1;
    }

    // configure GPIO_8 for output
    struct gpiod_line_request_config config =
        {consumer,
         GPIOD_LINE_REQUEST_DIRECTION_OUTPUT,
         0};

    int rc = gpiod_line_request(gpio_8,
                                &config,
                                1);
    if (rc < 0)
    {
        perror("               gpiod_line_request(gpio_8)");
        gpiod_line_release(gpio_8);
        gpiod_chip_close(chip);
    }

    // prepare to wait on event for GPIO 11
    gpio_11 = gpiod_chip_get_line(chip, 11);
    if (rc < 0)
    {
        perror("               gpiod_chip_get_line(gpio_11)");
        return -1;
    }

    rc = gpiod_line_request_both_edges_events(gpio_11, consumer);
    if (rc < 0)
    {
        perror("               gpiod_line_request_both_edges_events(gpio_11)");
        return -1;
    }

    const struct timespec timeout = {0, 1000000}; // 0ns, 5s
    rc = gpiod_line_event_wait(gpio_11, &timeout);
    if (rc < 0)
    {
        perror("               gpiod_line_event_wait(gpio_11)");
        return -1;
    }
    
    {
        struct gpiod_line_event event;
        rc = gpiod_line_event_read(gpio_11, &event);
        if (rc < 0)
        {
            printf("event %d at %ld s, %ld ns\n", event.event_type, event.ts.tv_sec, event.ts.tv_nsec);
            perror("               gpiod_line_event_read(gpio_11, &event)");
        }
        else
        {
            printf("event %d at %ld s, %ld ns\n", event.event_type, event.ts.tv_sec, event.ts.tv_nsec);
        }
    }

    // cleanup
    gpiod_line_release(gpio_8);
    gpiod_line_release(gpio_11);
    gpiod_chip_close(chip);
    // GPIO 8 should return to default here (input w/ pullup)
}
