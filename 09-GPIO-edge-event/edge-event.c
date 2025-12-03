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
struct gpiod_edge_event_buffer *edge_event_buffer;

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

    /////////////////////////////////////////////
    // line settings, input GPIO20
    /////////////////////////////////////////////
    struct gpiod_line_settings *settings_in = gpiod_line_settings_new();
    if (NULL == settings_in)
    {
        int saved_error = errno;
        perror("gpiod_line_settings_new()");
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

    if (-1 == gpiod_line_settings_set_edge_detection(settings_in, GPIOD_LINE_EDGE_BOTH))
    {
        int saved_errno = errno;
        perror("gpiod_line_settings_set_edge_detection()");
        gpiod_line_settings_free(settings_in);
        gpiod_chip_close(chip);
        return saved_errno;
    }

    /////////////////////////////////////////////
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

    unsigned int offsets_in[] = {20};
    int rc = gpiod_line_config_add_line_settings(line_config_in, offsets_in, count_of(offsets_in), settings_in);
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

    edge_event_buffer = gpiod_edge_event_buffer_new(0); // create default size buffer
    if (NULL == edge_event_buffer)
    {
        int saved_errno = errno;
        perror("gpiod_edge_event_buffer_new()");
        gpiod_line_settings_free(settings_in);
        gpiod_request_config_free(config_request_in);
        gpiod_chip_close(chip);
        return saved_errno;
    }

    // loop a bit while the pushbutton is pressed or not

    unsigned long long ts_previous = 0; // to calculate delta nanoseconds between events
    while (1)
    {
        if (1 != gpiod_line_request_wait_edge_events(line_request_in, 5000000000)) // no pending event?
        {
            break;
        }
        int event_count = gpiod_line_request_read_edge_events(line_request_in, edge_event_buffer, 5);
        printf("events to read:%u\n", event_count);
        for (unsigned long i = 0; i < event_count; i++)
        {
            struct gpiod_edge_event *event = gpiod_edge_event_buffer_get_event(edge_event_buffer, i);
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

    /////////////////////////////////////////////
    // release all resources
    /////////////////////////////////////////////
    gpiod_edge_event_buffer_free(edge_event_buffer);
    gpiod_line_request_release(line_request_in);
    gpiod_line_settings_free(settings_in);
    gpiod_request_config_free(config_request_in);
    gpiod_chip_close(chip);

    return 0;
}
