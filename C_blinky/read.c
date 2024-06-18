/*
* Read the state of GPIO 20 on a Raspberry Pi

Build:
   gcc -Wall -o read read.c -l gpiod
*/

#include <stdio.h>
#include <gpiod.h>

const unsigned int gpio = 20;

int main(int argc, char **argv)
{
    int rc = gpiod_ctxless_get_value("/dev/gpiochip0",
                                     gpio, false, "consumer");
    if (rc < 0)
        perror("gpiod_ctxless_get_value");
    printf("Hello World GPIO %d is %d\n", gpio, rc);
}
