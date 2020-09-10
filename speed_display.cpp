#include <iostream>
#include <thread>
#include <iomanip>
#include <signal.h>
#include "data_bus.h"

using namespace std::chrono_literals;
using namespace std;

// main loop run flag
bool run {false};

// signal handler
void signalHandler(int sig)
{
    run = false;
}

int main()
{
    // read interval
    const auto read_interval = 500ms;

    // read speed value
    double speed;

    // overtaking signal handling for SIGINT
    (void) signal(SIGINT, signalHandler);

    // data bus initialization
    auto res = data_bus::init();
    if (res > 0) 
    {
        cout << "Data bus init error! (" << res << ")" << endl;
	return 1;
    }

    run = true;	

    cout << "Speed display started. Press Ctrl+C to stop..." << endl;

    // main loop
    while(run) 
    {
        // consume speed value
	speed = data_bus::consume();

        // set fixed floating format & precision for cout
        cout.setf(ios::fixed, ios::floatfield); 
        cout.precision(2);

        // print speed value
        cout << "Speed: " << setw(6) << speed << " kph" << endl;
        
        // sleep for read interval
        this_thread::sleep_for(read_interval);
    }

    data_bus::deInit();

    return 0;
}
