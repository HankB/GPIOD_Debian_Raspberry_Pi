// follow_input example in C++

/*
Build
g++ -Wall -o follow_input follow_input.cpp -lgpiodcxx
*/

#include <iostream>
using namespace std;
#include <gpiod.hpp>

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
    chip.close();
}