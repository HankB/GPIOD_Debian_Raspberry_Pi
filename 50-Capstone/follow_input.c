/*
Drive output on GPIO 8 to match the input on GPIO 24

Lots of copy and paste from examples at
https://github.com/brgl/libgpiod/tree/master/examples
(as well as my examples.)

Build:
gcc -o follow_input -Wall follow_input.c -lgpiod
*/

#include <errno.h>
#include <gpiod.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define count_of(x) (sizeof(x) / sizeof(x[0]))

static const char *chip_name = "/dev/gpiochip0";
static const char *consumer = "follower";

// used to report input changes
static const char *event_type_to_str(enum gpiod_edge_event_type type)
{
    switch (type)
    {
    case GPIOD_EDGE_EVENT_RISING_EDGE:
        return " rising";
        break;
    case GPIOD_EDGE_EVENT_FALLING_EDGE:
        return "falling";
        break;
    default:
        return "unknown";
        break;
    }
}

// configuire input
static struct gpiod_line_request *init_input_gpio(unsigned int GPIO_number)
{
    /////////////////////////////////////////////
    // open the chip
    /////////////////////////////////////////////
    struct gpiod_chip *chip = gpiod_chip_open(chip_name);
    if (NULL == chip)
    {
        perror("Can't open chip:");
        return NULL;
    }

    /////////////////////////////////////////////
    // line settings
    /////////////////////////////////////////////
    struct gpiod_line_settings *settings_in = gpiod_line_settings_new();
    if (NULL == settings_in)
    {
        perror("gpiod_line_settings_new()");
        goto close_chip;
    }

    if (-1 == gpiod_line_settings_set_direction(settings_in, GPIOD_LINE_DIRECTION_INPUT))
    {
        perror("gpiod_line_settings_set_direction()");
        goto free_settings;
    }

    if (-1 == gpiod_line_settings_set_edge_detection(settings_in, GPIOD_LINE_EDGE_BOTH))
    {
        perror("gpiod_line_settings_set_edge_detection()");
        goto free_settings;
    }

    /////////////////////////////////////////////
    // request configuration/input
    /////////////////////////////////////////////
    struct gpiod_request_config *config_request_in = gpiod_request_config_new();
    if (NULL == config_request_in)
    {
        perror("gpiod_request_config_new()");
        goto free_settings;
    }

    gpiod_request_config_set_consumer(config_request_in, consumer);

    /////////////////////////////////////////////
    // line config
    /////////////////////////////////////////////
    struct gpiod_line_config *line_config_in = gpiod_line_config_new();
    if (NULL == line_config_in)
    {
        perror("gpiod_line_config_new()");
        goto free_settings;
    }

    unsigned int offsets_in[] = {GPIO_number};
    int rc = gpiod_line_config_add_line_settings(line_config_in, offsets_in, count_of(offsets_in), settings_in);
    if (0 != rc)
    {
        perror("gpiod_line_config_add_line_settings()");
        goto free_line_config;
    }

    /////////////////////////////////////////////
    // line request
    /////////////////////////////////////////////
    struct gpiod_line_request *line_request_in = gpiod_chip_request_lines(
        chip, config_request_in, line_config_in);
    if (NULL == line_request_in)
    {
        perror("gpiod_chip_request_lines()");
        goto free_line_config;
    }

free_line_config:
    gpiod_request_config_free(config_request_in);

free_settings:
    gpiod_line_settings_free(settings_in);

close_chip:
    gpiod_chip_close(chip);
    return line_request_in;
}

struct gpiod_edge_event_buffer *configure_edge_events(struct gpiod_line_request *line)
{
    struct gpiod_edge_event_buffer *edge_event_buffer =
        gpiod_edge_event_buffer_new(0); // create default size buffer
    if (NULL == edge_event_buffer)
    {
        perror("gpiod_edge_event_buffer_new()");
    }
    return edge_event_buffer;
}

struct gpiod_line_request *init_output_gpio(unsigned int GPIO_number)
{
    /////////////////////////////////////////////
    // open the chip
    /////////////////////////////////////////////
    struct gpiod_chip *chip = gpiod_chip_open(chip_name);
    if (NULL == chip)
    {
        perror("Can't open chip:");
        return NULL;
    }

    /////////////////////////////////////////////
    // line settings
    /////////////////////////////////////////////
    struct gpiod_line_settings *settings_out = gpiod_line_settings_new();
    if (NULL == settings_out)
    {
        perror("gpiod_line_settings_new()");
        goto close_chip;
    }

    if (gpiod_line_settings_set_direction(settings_out,
                                          GPIOD_LINE_DIRECTION_OUTPUT) != 0)
    {
        perror("gpiod_line_settings_set_direction()");
        goto free_settings;
    }

    if (gpiod_line_settings_set_output_value(settings_out,
                                             GPIOD_LINE_VALUE_INACTIVE) != 0)
    {
        perror("gpiod_line_settings_set_output_value()");
        goto free_settings;
    }

    /////////////////////////////////////////////
    // line config
    /////////////////////////////////////////////
    struct gpiod_line_config *line_config_out = gpiod_line_config_new();
    if (NULL == line_config_out)
    {
        perror("gpiod_line_config_new()");
        goto free_settings;
    }

    unsigned int offsets_out[] = {GPIO_number};
    if (gpiod_line_config_add_line_settings(line_config_out, offsets_out, count_of(offsets_out), settings_out) != 0)
    {
        perror("gpiod_line_config_add_line_settings()");
        goto free_settings;
    }

    /////////////////////////////////////////////
    // request configuration
    /////////////////////////////////////////////
    struct gpiod_request_config *config_request_out = gpiod_request_config_new();
    if (NULL == config_request_out)
    {
        perror("gpiod_request_config_new()");
        goto close_chip;
    }

    gpiod_request_config_set_consumer(config_request_out, consumer);

    /////////////////////////////////////////////
    // line request
    /////////////////////////////////////////////
    struct gpiod_line_request *line_request_out = gpiod_chip_request_lines(
        chip, config_request_out, line_config_out);
    if (NULL == line_request_out)
    {
        perror("gpiod_chip_request_lines()");
    }

    gpiod_request_config_free(config_request_out);

free_settings:
    gpiod_line_settings_free(settings_out);

close_chip:
    gpiod_chip_close(chip);
    return line_request_out;
}

enum gpiod_line_value get_input(struct gpiod_line_request *input,
                                struct gpiod_edge_event_buffer *events,
                                unsigned int timeout_s)
{
    enum gpiod_line_value val = GPIOD_LINE_VALUE_ERROR;
    if (1 == gpiod_line_request_wait_edge_events(input, timeout_s * 1000000000)) // no pending event for 5s?
    {
        int event_count = gpiod_line_request_read_edge_events(input, events, 5);
        printf("events to read:%u\n", event_count);
        for (unsigned long i = 0; i < event_count; i++)
        {
            struct gpiod_edge_event *event = gpiod_edge_event_buffer_get_event(events, i);
            val = gpiod_edge_event_get_event_type(event);
        }
    }
    return val; // return last event if multiple
}

int main(int argc, char **argv)
{
    struct gpiod_line_request *input;
    struct gpiod_line_request *output;
    struct gpiod_edge_event_buffer *events;
    enum gpiod_line_value input_value;

    input = init_input_gpio(20);
    if (NULL == input)
    {
        fprintf(stderr, "init_input_gpio()");
        return -1;
    }
    events = configure_edge_events(input);
    if (NULL == events)
    {
        fprintf(stderr, "init_input_gpio()");
        return -1;
    }

    output = init_output_gpio(8);

    while (1)
    {
        input_value = get_input(input, events, 5);
        printf("input:%s\n", event_type_to_str(input_value));
    }

    return 0;
}