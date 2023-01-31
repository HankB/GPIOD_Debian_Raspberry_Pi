/*
* Explore various GPIO line related APIs

Build
    gcc -Wall -o line_rqst line_rqst.c -l gpiod
*/

#include <stdio.h>
#include <gpiod.h>

void report_line_attributes(struct gpiod_line *line, const char *name)
{
    bool is_free = gpiod_line_is_free(line);
    printf("Is free        %s is %s\n", name, is_free ? "yes" : "no");

    bool is_requested = gpiod_line_is_requested(line);
    printf("Is requested   %s is %s\n", name, is_requested ? "yes" : "no");

    int bias = gpiod_line_bias(line);
    printf("Bias of        %s is %d\n", name, bias);

    const char *consumer = gpiod_line_consumer(line);
    printf("Consumer of    %s is %s\n", name, consumer);

    int direction = gpiod_line_direction(line);
    printf("Direction of   %s is %d\n", name, direction);

    bool used = gpiod_line_is_used(line);
    printf("Is             %s used %s\n", name, used ? "yes" : "no");

}

int main(int argc, char **argv)
{
    // need to open the chip first
    struct gpiod_chip *chip = gpiod_chip_open("/dev/gpiochip0");
    if (chip == NULL)
    {
        perror("gpiod_chip_open()");
        return 1;
    }

    // next find a couple lines
    struct gpiod_line_bulk bulk;
    const char *names[] = {"SPI_CE1_N", "SPI_CE0_N", "STATUS_LED_G", 0};
    int rc = gpiod_chip_find_lines(chip,
                                   names,
                                   &bulk);
    if (rc != 0)
    {
        printf("rc = %d ", rc);
        perror("gpiod_chip_find_lines()");
        gpiod_chip_close(chip);
        return 1;
    }
    else
    {
        printf("found %d lines\n\n", bulk.num_lines);
    }

    const struct gpiod_line_request_config config =
        {"line_rqst",
         GPIOD_LINE_REQUEST_DIRECTION_AS_IS,
         0};

    // find the offset of the lines
    for (int i = 0; i < bulk.num_lines; i++)
    {

        printf("untouched =====\n");
        report_line_attributes(bulk.lines[i], names[i]);

        int rc = gpiod_line_request(bulk.lines[i], &config, 1);
        printf("line_request() %s is %d\n", names[i], rc);
        printf("requested =====\n");

        if (rc < 0)
            perror("               gpiod_line_request()");

        report_line_attributes(bulk.lines[i], names[i]);
        gpiod_line_release(bulk.lines[i]);
        printf("released ======\n");
        report_line_attributes(bulk.lines[i], names[i]);
        printf("===============\n\n");
}

    gpiod_chip_close(chip);
}
