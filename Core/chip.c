/*
Open the chip and read some info.

Build:
   gcc -Wall -o read read.c -l gpiod
*/

#include <stdio.h>
#include <gpiod.h>
#include <errno.h>

static const char *chip_name = "/dev/gpiochip0";

int main(int argc, char **argv)
{
    printf("API version:%s:\n", gpiod_api_version());

    // open the chip
    struct gpiod_chip * p_chip = gpiod_chip_open(chip_name);
    if (NULL == p_chip) {
        perror("Can't open chip:");
        return(errno);
    }
    else {
        printf("opened chip '%s'\n", chip_name);
    }

    // fetch chip info
    struct gpiod_chip_info * p_info = gpiod_chip_get_info(p_chip);
    if(NULL == p_info) {
        perror("Can't get chip info:");
        int save_errno = errno;
        gpiod_chip_close(p_chip);
        return(save_errno);
    }
    else {
        printf( "  Info for chip:%s\n"
                "           name:%s\n"
                "          label:%s\n"
                "number of lines:%d\n",
            chip_name,
            gpiod_chip_info_get_name(p_info),
            gpiod_chip_info_get_label(p_info),
            gpiod_chip_info_get_num_lines(p_info)
        );
        gpiod_chip_info_free(p_info);
    }

    // close the chip
    gpiod_chip_close(p_chip);

    return 0;
}
