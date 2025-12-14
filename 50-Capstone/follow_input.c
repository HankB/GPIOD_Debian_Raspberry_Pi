/*
Drive output on GPIO 8 to match the input on GPIO 24

Logs of copy and paste from examples at
https://github.com/brgl/libgpiod/tree/master/examples
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
        perror("gpiod_chip_request_lines()");
    }
    return edge_event_buffer;
}

int main(int argc, char **argv)
{
    struct gpiod_line_request *input;
    struct gpiod_edge_event_buffer *events;

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

    unsigned long long ts_previous = 0; // to calculate delta nanoseconds between events
    while (1)
    {
        if (1 != gpiod_line_request_wait_edge_events(input, 5000000000)) // no pending event for 5s?
        {
            break;
        }
        int event_count = gpiod_line_request_read_edge_events(input, events, 5);
        printf("events to read:%u\n", event_count);
        for (unsigned long i = 0; i < event_count; i++)
        {
            struct gpiod_edge_event *event = gpiod_edge_event_buffer_get_event(events, i);
            unsigned long long ts = gpiod_edge_event_get_timestamp_ns(event);
            printf("ix:%lu GPIO:%u type:%s, global_seqno:%lu line_seqno:%lu timestamp:%llu delta:%llu\n",
                   i,
                   gpiod_edge_event_get_line_offset(event),
                   event_type_to_str(gpiod_edge_event_get_event_type(event)),
                   gpiod_edge_event_get_global_seqno(event),
                   gpiod_edge_event_get_line_seqno(event),
                   ts, ts - ts_previous);
            ts_previous = ts;
        }
    }

    return 0;
}