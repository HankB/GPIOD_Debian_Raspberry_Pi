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
    struct gpiod_request_config *config_request = gpiod_request_config_new();
    if (NULL == config_request)
    {
        perror("gpiod_request_config_new()");
        exit(errno);
    }
    printf("Request configuration object acquired\n");

    gpiod_request_config_set_consumer(config_request, "line request");
    printf("Request consumer set to '%s'\n", gpiod_request_config_get_consumer(config_request));
    printf("Default event buffer size %d\n", gpiod_request_config_get_event_buffer_size(config_request));

    /////////////////////////////////////////////
    // line config
    /////////////////////////////////////////////
    struct gpiod_line_config *line_config = gpiod_line_config_new();
    if (NULL == line_config)
    {
        perror("gpiod_line_config_new()");
        exit(errno);
    }
    printf("Line configuration object acquired\n");

    /////////////////////////////////////////////
    // line settings
    /////////////////////////////////////////////
    struct gpiod_line_settings *settings = gpiod_line_settings_new();
    if (NULL == settings)
    {
        perror("gpiod_line_settings_new()");
        exit(errno);
    }
    printf("Settings object acquired\n");
    if (-1 == gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT))
    {
        int saved_errno = errno;
        perror("gpiod_line_settings_set_direction()");
        gpiod_line_settings_free(settings);
        gpiod_chip_close(chip);
        return saved_errno;
    }
    printf("Settings object set to output\n");
    enum gpiod_line_value values[] = {GPIOD_LINE_VALUE_INACTIVE};
    int rc = gpiod_line_config_set_output_values(line_config, values, 1);
    printf("line set to output and set active\n");

    unsigned int offsets[1] = {8};
    rc = gpiod_line_config_add_line_settings(line_config, offsets, 1, settings);
    if (0 != rc)
    {
        int saved_errno = errno;
        perror("gpiod_line_config_add_line_settings()");
        gpiod_line_config_free(line_config);
        gpiod_line_settings_free(settings);
        return saved_errno;
    }
    printf("gpiod_line_settings added to gpiod_line_config\n");

    /////////////////////////////////////////////
    // Set event buffer (useless for an output?)
    /////////////////////////////////////////////
    size_t requested_size = 10;
    size_t realized_size;
    gpiod_request_config_set_event_buffer_size(config_request, requested_size);
    realized_size = gpiod_request_config_get_event_buffer_size(config_request);
    printf("Event buffer size requested %u, realized %u\n",
           requested_size, realized_size);

    /////////////////////////////////////////////
    // line request
    /////////////////////////////////////////////
    struct gpiod_line_request *line_request = gpiod_chip_request_lines(
        chip, config_request, line_config);
    if (NULL == line_request)
    {
        int saved_errno = errno;
        perror("gpiod_chip_request_lines()");
        gpiod_line_settings_free(settings);
        gpiod_request_config_free(config_request);
        gpiod_chip_close(chip);
        return saved_errno;
    }
    printf("Line request object acquired\n");
    print_line_request_info(line_request);
    rc = gpiod_line_request_set_value(line_request, offsets[0], GPIOD_LINE_VALUE_ACTIVE);
    printf("\ngpiod_line_request_set_value():%d\n", rc);
    print_line_request_info(line_request);
    sleep(1); // so we can see if GPIO8 was driven hign
    rc = gpiod_line_request_set_value(line_request, offsets[0], GPIOD_LINE_VALUE_INACTIVE);
    printf("\ngpiod_line_request_set_value():%d\n", rc);
    print_line_request_info(line_request);

    gpiod_line_request_release(line_request);
    gpiod_line_settings_free(settings);
    gpiod_request_config_free(config_request);
    gpiod_chip_close(chip);

    return 0;
}
