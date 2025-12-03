/*
Open the chip and read some info.

Build:
   gcc -Wall -o line-request line-request.c -l gpiod
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gpiod.h>
#include <errno.h>

static const char *chip_name = "/dev/gpiochip0";
#define count_of(x) (sizeof(x)/sizeof(x[0]))

static void print_line_request_info(struct gpiod_line_request *request)
{
    printf("\n");
    printf("            chip name:%s\n", gpiod_line_request_get_chip_name(request));
    printf("      requested lines:%u\n", gpiod_line_request_get_num_requested_lines(request));
    static const size_t max_offsets = 5;
    unsigned int offsets[max_offsets];
    size_t offsets_found = gpiod_line_request_get_requested_offsets(request, offsets, max_offsets);
    for (int i = 0; i < offsets_found; i++)
    {
        enum gpiod_line_value value = gpiod_line_request_get_value(request, offsets[i]);
        printf("            offset at:%u is %d\n", offsets[i], value);
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
    // request configuration
    /////////////////////////////////////////////
    struct gpiod_request_config *config_request_out = gpiod_request_config_new();
    if (NULL == config_request_out)
    {
        perror("gpiod_request_config_new()");
        exit(errno);
    }
    printf("Request configuration object acquired\n");

    gpiod_request_config_set_consumer(config_request_out, "line request");
    printf("Request consumer set to '%s'\n", gpiod_request_config_get_consumer(config_request_out));
    printf("Default event buffer size %d\n", gpiod_request_config_get_event_buffer_size(config_request_out));

    /////////////////////////////////////////////
    // line config
    /////////////////////////////////////////////
    struct gpiod_line_config *line_config_out = gpiod_line_config_new();
    if (NULL == line_config_out)
    {
        perror("gpiod_line_config_new()");
        exit(errno);
    }
    printf("Line configuration object acquired\n");

    /////////////////////////////////////////////
    // line settings, output GPIO8
    /////////////////////////////////////////////
    struct gpiod_line_settings *settings_out = gpiod_line_settings_new();
    if (NULL == settings_out)
    {
        perror("gpiod_line_settings_new()");
        exit(errno);
    }
    printf("Settings object acquired\n");
    if (-1 == gpiod_line_settings_set_direction(settings_out, GPIOD_LINE_DIRECTION_OUTPUT))
    {
        int saved_errno = errno;
        perror("gpiod_line_settings_set_direction()");
        gpiod_line_settings_free(settings_out);
        gpiod_chip_close(chip);
        return saved_errno;
    }
    printf("Settings object set to output\n");
    enum gpiod_line_value values_out[] = {GPIOD_LINE_VALUE_INACTIVE};
    int rc = gpiod_line_config_set_output_values(line_config_out, values_out, count_of(values_out));
    printf("line set to output and set active\n");

    unsigned int offsets_out[] = {8}; // NEED TO MATCH COUNT OF ELEMENTS IN values_out[]
    rc = gpiod_line_config_add_line_settings(line_config_out, offsets_out, count_of(offsets_out) , settings_out);
    if (0 != rc)
    {
        int saved_errno = errno;
        perror("gpiod_line_config_add_line_settings()");
        gpiod_line_config_free(line_config_out);
        gpiod_line_settings_free(settings_out);
        return saved_errno;
    }
    printf("gpiod_line_settings added to gpiod_line_config\n");

    /////////////////////////////////////////////
    // Set event buffer (useless for an output?)
    /////////////////////////////////////////////
    size_t requested_size = 10;
    size_t realized_size;
    gpiod_request_config_set_event_buffer_size(config_request_out, requested_size);
    realized_size = gpiod_request_config_get_event_buffer_size(config_request_out);
    printf("Event buffer size requested %u, realized %u\n",
           requested_size, realized_size);

    /////////////////////////////////////////////
    // line request
    /////////////////////////////////////////////
    struct gpiod_line_request *line_request_out = gpiod_chip_request_lines(
        chip, config_request_out, line_config_out);
    if (NULL == line_request_out)
    {
        int saved_errno = errno;
        perror("gpiod_chip_request_lines()");
        gpiod_line_settings_free(settings_out);
        gpiod_request_config_free(config_request_out);
        gpiod_chip_close(chip);
        return saved_errno;
    }
    printf("Line request object acquired\n");
    print_line_request_info(line_request_out);
    rc = gpiod_line_request_set_value(line_request_out, offsets_out[0], GPIOD_LINE_VALUE_ACTIVE);
    printf("\ngpiod_line_request_set_value():%d\n", rc);
    print_line_request_info(line_request_out);

    sleep(1); // so we can see if GPIO8 was driven hign
    rc = gpiod_line_request_set_value(line_request_out, offsets_out[0], GPIOD_LINE_VALUE_INACTIVE);
    printf("\ngpiod_line_request_set_value():%d\n", rc);
    print_line_request_info(line_request_out);

    /////////////////////////////////////////////
    // Start configuration of input line
    // 
    // request configuration/input
    /////////////////////////////////////////////
    struct gpiod_request_config *config_request_in = gpiod_request_config_new();
    if (NULL == config_request_in)
    {
        perror("gpiod_request_config_new()");
        exit(errno);
    }
    printf("Request input configuration object acquired\n");

    gpiod_request_config_set_consumer(config_request_in, "line request");
    printf("Request consumer set to '%s'\n", gpiod_request_config_get_consumer(config_request_in));
    printf("Default event buffer size %d\n", gpiod_request_config_get_event_buffer_size(config_request_in));

    /////////////////////////////////////////////
    // line config/input
    /////////////////////////////////////////////
    struct gpiod_line_config *line_config_in = gpiod_line_config_new();
    if (NULL == line_config_in)
    {
        perror("gpiod_line_config_new()");
        exit(errno);
    }
    printf("Line configuration object acquired\n");

    /////////////////////////////////////////////
    // line settings, input GPIO20
    /////////////////////////////////////////////
    struct gpiod_line_settings *settings_in = gpiod_line_settings_new();
    if (NULL == settings_in)
    {
        perror("gpiod_line_settings_new()");
        exit(errno);
    }
    printf("Settings object acquired\n");
    if (-1 == gpiod_line_settings_set_direction(settings_in, GPIOD_LINE_DIRECTION_INPUT))
    {
        int saved_errno = errno;
        perror("gpiod_line_settings_set_direction()");
        gpiod_line_settings_free(settings_in);
        gpiod_chip_close(chip);
        return saved_errno;
    }
    printf("Settings object set to input\n");
    enum gpiod_line_value values_in[] = {GPIOD_LINE_VALUE_INACTIVE};
    rc = gpiod_line_config_set_output_values(line_config_in, values_in, count_of(values_in));
    printf("line set to output and set active\n");

    unsigned int offsets_in[] = {20}; // NEED TO MATCH COUNT OF ELEMENTS IN values_in[]
    rc = gpiod_line_config_add_line_settings(line_config_in, offsets_in, count_of(offsets_in) , settings_in);
    if (0 != rc)
    {
        int saved_errno = errno;
        perror("gpiod_line_config_add_line_settings()");
        gpiod_line_config_free(line_config_in);
        gpiod_line_settings_free(settings_in);
        return saved_errno;
    }
    printf("gpiod_line_settings added to gpiod_line_config\n");

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
    while(loops--)
    {
    print_line_request_info(line_request_in);
    sleep(1);
    }

    /////////////////////////////////////////////
    // release all resources
    /////////////////////////////////////////////
    gpiod_line_request_release(line_request_in);
    //gpiod_line_request_release(line_request_out);
    gpiod_line_settings_free(settings_in);
    //gpiod_line_settings_free(settings_out);
    gpiod_request_config_free(config_request_in);
    //gpiod_request_config_free(config_request_out);
    gpiod_chip_close(chip);

    return 0;
}
