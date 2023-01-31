/*
* Explore various GPIO line related APIs

Build
    gcc -Wall -o line_cfg line_cfg.c -l gpiod
*/

#include <stdio.h>
#include <gpiod.h>
#include <unistd.h>

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
}

int main(int argc, char **argv)
{
    // need to open the chip first
    struct gpiod_chip *chip = gpiod_chip_open("/dev/gpiochip0");
    if (chip == NULL)
    {
        perror("gpiod_chip_open()");
        return 1;
    }

    // acquire & configure GPIO 8

    struct gpiod_line *gpio_8;

    gpio_8 = gpiod_chip_find_line(chip, "SPI_CE0_N");
    report_line_attributes(gpio_8, "SPI_CE0_N");

    if (gpio_8 == NULL)
    {
        perror("gpiod_chip_find_line(SPI_CE0_N)");
        gpiod_chip_close(chip);
        return 1;
    }

    const struct gpiod_line_request_config write_config =
        {"line_cfg",
         GPIOD_LINE_REQUEST_DIRECTION_OUTPUT,
         0};

    int rc = gpiod_line_request(gpio_8,
                                &write_config,
                                1);
    if (rc < 0)
    {
        perror("               gpiod_line_request(gpio_8)");
        gpiod_line_release(gpio_8);
        gpiod_chip_close(chip);
    }

    printf("=====\n");
    report_line_attributes(gpio_8, "SPI_CE0_N");
    printf("============\n\n");

    sleep(1); // GPIO 8 should be driven high here.

    rc = gpiod_line_set_config(gpio_8,
                               GPIOD_LINE_REQUEST_DIRECTION_OUTPUT,
                               0,
                               0);
    if (rc < 0)
    {
        perror("               gpiod_line_set_config(gpio_8)");
        gpiod_line_release(gpio_8);
        gpiod_chip_close(chip);
    }

    printf("=====\n");
    report_line_attributes(gpio_8, "SPI_CE0_N");
    printf("============\n\n");

    sleep(1); // GPIO 8 should be driven low here.

    // cleanup
    gpiod_line_release(gpio_8);
    gpiod_chip_close(chip);
    // GPIO 8 should return to default here (input w/ pullup)
}
