/*
Open the chip and read some info.

Build:
   gcc -Wall -o request-configuration request-configuration.c -l gpiod
*/

#include <stdlib.h>
#include <stdio.h>
#include <gpiod.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    printf("API version:%s:\n", gpiod_api_version());

    struct gpiod_request_config *request = gpiod_request_config_new();
    if (NULL == request)
    {
        perror("gpiod_request_config_new()");
        exit(errno);
    }
    printf("Request configuration object acquired\n");

    gpiod_request_config_set_consumer(request, "request_config");
    printf("Request consumer set to '%s'\n", gpiod_request_config_get_consumer(request));

    printf("Default event buffer size %d\n", gpiod_request_config_get_event_buffer_size(request));

    // explore allowable size (until we roll over.)
    size_t requested_size = 1;
    size_t realized_size;
    do
    {
        requested_size *= 2;
        gpiod_request_config_set_event_buffer_size(request, requested_size);
        realized_size = gpiod_request_config_get_event_buffer_size(request);
        printf("Event buffer size requested %u, realized %u\n", requested_size, realized_size);
    } while ((requested_size == realized_size) && (0 != requested_size));

    gpiod_request_config_free(request);
    return 0;
}
