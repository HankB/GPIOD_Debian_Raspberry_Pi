/*
* Drive GPIO 8 on a Raspberry Pi

Build:
   gcc -Wall -o drive drive.c -l gpiod
*/

#include <stdio.h>
#include <unistd.h>
#include <gpiod.h>

const unsigned int gpio = 8;

void set_cb(void *unused)
{
    sleep(1);
}

int main(int argc, char **argv)
{
    int rc = gpiod_ctxless_set_value("/dev/gpiochip0",
                                     gpio, 1, false, "consumer", set_cb, 0);
    if (rc < 0)
        perror("gpiod_ctxless_get_value");
    printf("Hello World, rc is %d\n", rc);
}
