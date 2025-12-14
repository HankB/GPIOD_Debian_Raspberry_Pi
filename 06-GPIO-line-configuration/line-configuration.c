/*
Open the chip and read some info.

Build:
   gcc -Wall -o line-configuration line-configuration.c -l gpiod
*/

#include <stdlib.h>
#include <stdio.h>
#include <gpiod.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    printf("API version:%s:\n", gpiod_api_version());

    struct gpiod_line_config *config = gpiod_line_config_new();
    if (NULL == config)
    {
        perror("gpiod_line_config_new()");
        exit(errno);
    }
    printf("Line configuration object acquired\n");

    struct gpiod_line_settings *settings = gpiod_line_settings_new();
    if (NULL == settings)
    {
        perror("gpiod_line_settings_new()");
        exit(errno);
    }
    printf("Line settings object acquired\n");

    // set to output and set output active
    printf("      change: %d\n",
           gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT));
    enum gpiod_line_value values[] = {GPIOD_LINE_VALUE_INACTIVE};
    int rc = gpiod_line_config_set_output_values(config, values, 1);
    printf("line set to output and set active\n");

    unsigned int offsets[1] = {8};
    rc = gpiod_line_config_add_line_settings(config, offsets, 1, settings);
    if (0 != rc)
    {
        int saved_errno = errno;
        perror("gpiod_line_config_add_line_settings()");
        gpiod_line_config_free(config);
        gpiod_line_settings_free(settings);
        return saved_errno;
    }

    printf("gpiod_line_settings added to gpiod_line_config\n");

    sleep(1);
    gpiod_line_config_free(config);
    gpiod_line_settings_free(settings);
    return 0;
}
