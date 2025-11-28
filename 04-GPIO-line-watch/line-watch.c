/*
Open the chip and read some info.

Build:
   gcc -Wall -o line-watch line-watch.c -l gpiod
*/

#include <stdio.h>
#include <gpiod.h>
#include <errno.h>

static const char *chip_name = "/dev/gpiochip0";

static void print_line_info(struct gpiod_line_info *p_info)
{
    printf("line info for:%s\n"
           "       direction:%d\n"
           "\n",
           gpiod_line_info_get_name(p_info),
           gpiod_line_info_get_direction(p_info));
}

int main(int argc, char **argv)
{
    printf("API version:%s:\n", gpiod_api_version());

    // open the chip
    struct gpiod_chip *chip = gpiod_chip_open(chip_name);
    if (NULL == chip)
    {
        perror("Can't open chip:");
        return (errno);
    }
    else
    {
        printf("opened chip '%s'\n\n", chip_name);
    }

    struct gpiod_info_event *info_event = gpiod_chip_read_info_event(chip);

    // release resources
    gpiod_chip_close(chip);

    return 0;
}
