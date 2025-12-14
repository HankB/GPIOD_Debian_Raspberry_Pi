/*
Open the chip and read some info.

Build:
   gcc -Wall -o line-settings line-settings.c -l gpiod
*/

#include <stdlib.h>
#include <stdio.h>
#include <gpiod.h>
#include <errno.h>

int main(int argc, char **argv)
{
    printf("API version:%s:\n", gpiod_api_version());

    struct gpiod_line_settings *settings = gpiod_line_settings_new();
    if (NULL == settings)
    {
        perror("gpiod_line_settings_new()");
        exit(errno);
    }
    printf("Settings object acquired\n");
    printf("   direction: %d\n", gpiod_line_settings_get_direction(settings));
    printf("      change: %d\n",
           gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_INPUT));
    printf("   direction: %d\n", gpiod_line_settings_get_direction(settings));
    printf("      change: %d\n",
           gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT));
    printf("   direction: %d\n", gpiod_line_settings_get_direction(settings));

    gpiod_line_settings_free(settings);
    return 0;
}
