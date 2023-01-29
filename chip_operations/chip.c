/*
* Explore various GPIO chip related APIs

Build
    gcc -Wall -o chip chip.c -l gpiod
*/

#include <stdio.h>
#include <gpiod.h>

int main(int argc, char **argv)
{
    struct gpiod_chip *chip = gpiod_chip_open("/dev/gpiochip0");
    if (chip == NULL)
    {
        perror("gpiod_chip_open()");
        return 1;
    }
    // gpiod_chip is an opaque data type but it can be used to
    // identify other properties of the chip.

    printf("name:  \"%s\"\n", gpiod_chip_name(chip));
    printf("label: \"%s\"\n", gpiod_chip_label(chip));
    printf("lines: %d\n", gpiod_chip_num_lines(chip));

    // get lines - probably useful later

    struct gpiod_line *line = gpiod_chip_find_line(chip, "SPI_CE1_N");
    if (line == NULL)
    {
        perror("gpiod_chip_find_line(SPI_CE1_N)");
        gpiod_chip_close(chip);
        return 1;
    }

#if 0
    line = gpiod_chip_find_line(chip, "GPIO7");
    if (line == NULL)
    {
        perror("gpiod_chip_find_line(GPIO7)");
    }
#endif

    struct gpiod_line_bulk bulk;
    const char *names[] = {"GPIO16", "GPIO17", 0};
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

    gpiod_chip_close(chip);
}
