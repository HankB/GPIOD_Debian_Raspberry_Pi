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
    for(int i=0; i< bulk.num_lines; i++)
    {
        unsigned int offset = gpiod_line_offset(bulk.lines[i]);
        printf("Offset of name %s is %d\n", names[i], offset);
        int state = gpiod_line_active_state(bulk.lines[i]);
        printf("Hi/lo state of name %s is %d\n", names[i], state);

        printf("\n");
    }

    gpiod_chip_close(chip);
}
