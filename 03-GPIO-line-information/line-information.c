/*
Open the chip and read some info.

Build:
   gcc -Wall -o line-information line-information.c -l gpiod
*/

#include <stdio.h>
#include <gpiod.h>
#include <errno.h>

static const char *chip_name = "/dev/gpiochip0";

static void print_line_information( struct gpiod_chip * chip, 
                                    unsigned int offset) {
    struct gpiod_line_info * line_info = 
        gpiod_chip_get_line_info(chip, offset);
    if(NULL == line_info) {
        perror("gpiod_chip_get_line_info()");
        return;
    }
    
    printf( "\nInfo for line offset %d\n", offset);
    printf( "          Offset: %d\n", gpiod_line_info_get_offset(line_info));
    printf( "            Name: %s\n", gpiod_line_info_get_name(line_info));
    printf( "         Is used: %s\n", gpiod_line_info_is_used(line_info)?"yes":"no");
    printf( "        Consumer: %s\n", gpiod_line_info_get_consumer(line_info));
    printf( "       Direction: ");
    switch(gpiod_line_info_get_direction(line_info)) {
        case GPIOD_LINE_DIRECTION_AS_IS:
            printf("as is\n");
            break;
        case GPIOD_LINE_DIRECTION_INPUT:
            printf("input\n");
            break;
        case GPIOD_LINE_DIRECTION_OUTPUT:
            printf("output\n");
            break;
        default:
            printf("unknown\n");
            break;
    }

    printf( "  Edge Detection: ");
    switch(gpiod_line_info_get_edge_detection(line_info)) {
        case GPIOD_LINE_EDGE_NONE:
            printf("none\n");
            break;
        case GPIOD_LINE_EDGE_RISING:
            printf("rising\n");
            break;
        case GPIOD_LINE_EDGE_FALLING:
            printf("falling\n");
            break;
        case GPIOD_LINE_EDGE_BOTH:
            printf("both\n");
            break;
        default:
            printf("unknown\n");
            break;
    }

    printf( "            Bias: ");
    switch(gpiod_line_info_get_bias(line_info)) {
        case GPIOD_LINE_BIAS_AS_IS:
            printf("as is\n");
            break;
        case GPIOD_LINE_BIAS_UNKNOWN:
            printf("unknown bias\n");
            break;
        case GPIOD_LINE_BIAS_DISABLED:
            printf("disabled\n");
            break;
        case GPIOD_LINE_BIAS_PULL_UP:
            printf("pull up\n");
            break;
        case GPIOD_LINE_BIAS_PULL_DOWN:
            printf("pull down\n");
            break;
        default:
            printf("unknown retval\n");
            break;
    }

    printf( "          Drive: ");
    switch(gpiod_line_info_get_drive(line_info)) {
        case GPIOD_LINE_DRIVE_PUSH_PULL:
            printf("push pull\n");
            break;
        case GPIOD_LINE_DRIVE_OPEN_DRAIN:
            printf("open drain\n");
            break;
        case GPIOD_LINE_DRIVE_OPEN_SOURCE:
            printf("open source\n");
            break;
        default:
            printf("unknown\n");
            break;
    }

    printf( "   Is active low: %s\n", gpiod_line_info_is_active_low(line_info)?"yes":"no");
    printf( "    Is debounced: %s\n", gpiod_line_info_is_debounced(line_info)?"yes":"no");
    printf( " Debounce period: %ld\n", gpiod_line_info_get_debounce_period_us(line_info));

    printf( "     Event clock: ");
    switch(gpiod_line_info_get_event_clock(line_info)) {
        case GPIOD_LINE_CLOCK_MONOTONIC:
            printf("monotonic\n");
            break;
        case GPIOD_LINE_CLOCK_REALTIME:
            printf("real time\n");
            break;
        case GPIOD_LINE_CLOCK_HTE:
            printf("HTE\n");
            break;
        default:
            printf("unknown\n");
            break;
    }

    gpiod_line_info_free(line_info);
}

int main(int argc, char **argv)
{
    printf("API version:%s:\n", gpiod_api_version());

    // open the chip
    struct gpiod_chip * chip = gpiod_chip_open(chip_name);
    if (NULL == chip) {
        perror("Can't open chip:");
        return(errno);
    }
    else {
        printf("opened chip '%s'\n", chip_name);
    }

    print_line_information(chip, 0);
    print_line_information(chip, 8);
    print_line_information(chip, 20);
    print_line_information(chip, 28);
    print_line_information(chip, 29);

    gpiod_chip_close(chip);

    return 0;
}
