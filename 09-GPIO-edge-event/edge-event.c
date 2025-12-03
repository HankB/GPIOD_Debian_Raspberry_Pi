/*
Open the chip and read some info.

Build:
   gcc -Wall -o edge-event edge-event.c -l gpiod
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gpiod.h>
#include <errno.h>

static const char *chip_name = "/dev/gpiochip0";
#define count_of(x) (sizeof(x) / sizeof(x[0]))

static void print_line_request_info(struct gpiod_line_request *request)
{
    static const size_t max_offsets = 5;
    unsigned int offsets[max_offsets];
    size_t offsets_found = gpiod_line_request_get_requested_offsets(request, offsets, max_offsets);
    for (int i = 0; i < offsets_found; i++)
    {
        enum gpiod_line_value value = gpiod_line_request_get_value(request, offsets[i]);
        printf("GPIO:%u is %d\n", offsets[i], value);
    }
}

int main(int argc, char **argv)
{
    printf("API version:%s:\n", gpiod_api_version());

    /////////////////////////////////////////////
    // open the chip
    /////////////////////////////////////////////
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

    /////////////////////////////////////////////
    // Start configuration of input line
    //
    // request configuration/input
    /////////////////////////////////////////////
    struct gpiod_request_config *config_request_in = gpiod_request_config_new();
    if (NULL == config_request_in)
    {
        int saved_error = errno;
        perror("gpiod_request_config_new()");
        gpiod_chip_close(chip);
        exit(saved_error);
    }

    gpiod_request_config_set_consumer(config_request_in, "line request");

    /////////////////////////////////////////////
    // line config/input
    /////////////////////////////////////////////
    struct gpiod_line_config *line_config_in = gpiod_line_config_new();
    if (NULL == line_config_in)
    {
        int saved_error = errno;
        perror("gpiod_line_config_new()");
        gpiod_request_config_free(config_request_in);
        gpiod_chip_close(chip);
        exit(saved_error);
    }
    /////////////////////////////////////////////
    // line settings, input GPIO20
    /////////////////////////////////////////////
    struct gpiod_line_settings *settings_in = gpiod_line_settings_new();
    if (NULL == settings_in)
    {
        int saved_error = errno;
        perror("gpiod_line_settings_new()");
        gpiod_request_config_free(config_request_in);
        gpiod_chip_close(chip);
        exit(saved_error);

        exit(errno);
    }
    if (-1 == gpiod_line_settings_set_direction(settings_in, GPIOD_LINE_DIRECTION_INPUT))
    {
        int saved_errno = errno;
        perror("gpiod_line_settings_set_direction()");
        gpiod_line_settings_free(settings_in);
        gpiod_chip_close(chip);
        return saved_errno;
    }

    enum gpiod_line_value values_in[] = {GPIOD_LINE_VALUE_INACTIVE};
    int rc = gpiod_line_config_set_output_values(line_config_in, values_in, count_of(values_in));

    unsigned int offsets_in[] = {20}; // NEED TO MATCH COUNT OF ELEMENTS IN values_in[]
    rc = gpiod_line_config_add_line_settings(line_config_in, offsets_in, count_of(offsets_in), settings_in);
    if (0 != rc)
    {
        int saved_errno = errno;
        perror("gpiod_line_config_add_line_settings()");
        gpiod_line_config_free(line_config_in);
        gpiod_line_settings_free(settings_in);
        return saved_errno;
    }

    /////////////////////////////////////////////
    // line request/input
    /////////////////////////////////////////////
    struct gpiod_line_request *line_request_in = gpiod_chip_request_lines(
        chip, config_request_in, line_config_in);
    if (NULL == line_request_in)
    {
        int saved_errno = errno;
        perror("gpiod_chip_request_lines()");
        gpiod_line_settings_free(settings_in);
        gpiod_request_config_free(config_request_in);
        gpiod_chip_close(chip);
        return saved_errno;
    }

    // loop a bit while the pushbutton is pressed or not
    int loops = 10;
    while (loops--)
    {
        print_line_request_info(line_request_in);
        sleep(1);
    }

    /////////////////////////////////////////////
    // release all resources
    /////////////////////////////////////////////
    gpiod_line_request_release(line_request_in);
    gpiod_line_settings_free(settings_in);
    gpiod_request_config_free(config_request_in);
    gpiod_chip_close(chip);

    return 0;
}
