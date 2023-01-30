/*
* Explore various GPIO line related APIs

Build
    gcc -Wall -o line line.c -l gpiod
*/

#include <stdio.h>
#include <gpiod.h>

int main(int argc, char **argv)
{
    // need to open the chip first
    struct gpiod_chip *chip = gpiod_chip_open("/dev/gpiochip0");
    if (chip == NULL)
    {
        perror("gpiod_chip_open()");
        return 1;
    }

    // next find a couple lines
    struct gpiod_line_bulk bulk;
    const char *names[] = {"GPIO16", "GPIO17", "STATUS_LED_G", 0};
    int rc = gpiod_chip_find_lines(chip,
                                   names,
                                   &bulk);
    if (rc != 0)
    {
        printf("rc = %d ", rc);
        perror("gpiod_chip_find_lines()");
        gpiod_chip_close(chip);
        return 1;
    }
    else
    {
        printf("found %d lines\n", bulk.num_lines);
    }

    // find the offset of the lines
    for (int i = 0; i < bulk.num_lines; i++)
    {
        unsigned int offset = gpiod_line_offset(bulk.lines[i]);
        printf("Offset of      %s is %d\n", names[i], offset);

        int state = gpiod_line_active_state(bulk.lines[i]);
        printf("Hi/lo state of %s is %d\n", names[i], state);

        int bias = gpiod_line_bias(bulk.lines[i]);
        printf("Bias of        %s is %d\n", names[i], bias);

        const char *consumer = gpiod_line_consumer(bulk.lines[i]);
        printf("Consumer of    %s is %s\n", names[i], consumer);

        int direction = gpiod_line_direction(bulk.lines[i]);
        printf("Direction of   %s is %d\n", names[i], direction);

        bool used = gpiod_line_is_used(bulk.lines[i]);
        printf("Is             %s used %s\n", names[i], used ? "yes" : "no");

        printf("\n");
    }

    gpiod_chip_close(chip);
}
