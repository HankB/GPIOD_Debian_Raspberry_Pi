/*
* Explore various GPIO line event related APIs

Build
    gcc -Wall -o line_events line_events.c -l gpiod
*/

#include <stdio.h>
#include <gpiod.h>
#include <unistd.h>

/*
void report_line_attributes(struct gpiod_line *line, const char *name)
{
    bool is_free = gpiod_line_is_free(line);
    printf("Is free        %s is %s\n", name, is_free ? "yes" : "no");
    bool is_requested = gpiod_line_is_requested(line);
    printf("Is requested   %s is %s\n", name, is_requested ? "yes" : "no");

    int bias = gpiod_line_bias(line);
    printf("Bias of        %s is %d\n", name, bias);

    const char *consumer = gpiod_line_consumer(line);
    printf("Consumer of    %s is %s\n", name, consumer);

    int direction = gpiod_line_direction(line);
    printf("Direction of   %s is %d\n", name, direction);

    bool used = gpiod_line_is_used(line);
    printf("Is             %s used %s\n", name, used ? "yes" : "no");

    int value = gpiod_line_get_value(line);
    printf("Value          %s is %d\n", name, value );
}
*/

int main(int argc, char **argv)
{
    // need to open the chip first
    struct gpiod_chip *chip = gpiod_chip_open("/dev/gpiochip0");
    if (chip == NULL)
    {
        perror("gpiod_chip_open()");
        return 1;
    }

    // acquire & configure GPIO 8 (output) GPIO 11 (input)

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
        {"line_cfg",
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
        perror("               gpiod_line_event_wait(gpio_11)");
        return -1;
    }
    rc = gpiod_line_request_falling_edge_events(gpio_11, "line_cfg");
    if (rc < 0)
    {
        perror("               gpiod_line_event_wait(gpio_11)");
        return -1;
    }

    const struct timespec timeout = {0, 1000000}; // 0ns, 5s
    do
    {
        rc = gpiod_line_event_wait(gpio_11, &timeout);

    } while (rc <= 0);
    
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
