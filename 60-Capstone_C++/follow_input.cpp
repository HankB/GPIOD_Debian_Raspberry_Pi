// follow_input example in C++

/*
Build
g++ -Wall -o follow_input follow_input.cpp -lgpiodcxxstd
*/

#include <iostream>
using namespace std;
#include <gpiod.hpp>

namespace
{

    /* Example configuration - customize to suit your situation. */
    const ::filesystem::path chip_path("/dev/gpiochip0");
    const ::gpiod::line::offset input_line_offset = 20;
    const ::gpiod::line::offset output_line_offset = 8;
    auto timeout = ::chrono::nanoseconds((long long unsigned)5 * 1000000000); // timeout in nano-seconds
    const char *consumer = "C++ follower";

    const char *edge_event_type_str(const ::gpiod::edge_event &event)
    {
        switch (event.type())
        {
        case ::gpiod::edge_event::event_type::RISING_EDGE:
            return "Rising";
        case ::gpiod::edge_event::event_type::FALLING_EDGE:
            return "Falling";
        default:
            return "Unknown";
        }
    }

} /* namespace */

int main(int argc, char **argv)
{
    cout << "GPIOD version " << gpiod::api_version() << endl;

    // chip object

    gpiod::chip chip("/dev/gpiochip0");
    if (!chip)
    {
        cout << "chip not constructed" << endl;
        exit(-1);
    }
    else
    {
        cout << "chip is constructed and useable" << endl;
    }
    gpiod::chip_info info = chip.get_info();
    cout << "name:" << info.name() << " label:" << info.label() << endl;

    gpiod::edge_event_buffer events;
    cout << "empty buffer holds " << events.num_events() << " events"
         << " and has a capacity of " << events.capacity() << endl;

    // input processing from the GPIOD example watch_line_rising.cpp
    // except we want both rising and falling events and a timeout.

    auto input_request =
        chip.prepare_request()
            .set_consumer(consumer)
            .add_line_settings(
                input_line_offset,
                ::gpiod::line_settings()
                    .set_direction(
                        ::gpiod::line::direction::INPUT)
                    .set_bias(
                        ::gpiod::line::bias::PULL_UP)
                    .set_edge_detection(
                        ::gpiod::line::edge::BOTH))
            .do_request();

    // output processing copied substantially from toggle_line_value.cpp

    auto output_request =
        ::gpiod::chip(chip_path)
            .prepare_request()
            .set_consumer(consumer)
            .add_line_settings(
                output_line_offset,
                ::gpiod::line_settings().set_direction(
                    ::gpiod::line::direction::OUTPUT))
            .do_request();

    // initialize the output to match the input
    output_request.set_value(output_line_offset,
                             input_request.get_value(input_line_offset));

    while (input_request.wait_edge_events(timeout))
    {

        /* Blocks until at least one event is available. */
        input_request.read_edge_events(events);

        for (const auto &event : events)
        {

            ::cout << "line: " << event.line_offset()
                   << "  type: " << ::setw(7) << ::left << edge_event_type_str(event)
                   << "  event #" << event.line_seqno()
                   << ::endl;
            output_request.set_value(output_line_offset, (event.type() == ::gpiod::edge_event::event_type::RISING_EDGE) ? gpiod::line::value::ACTIVE : gpiod::line::value::INACTIVE);
        }
    }

    chip.close();
}