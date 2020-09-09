#include <iostream>
#include <thread>
#include <iomanip>
#include <signal.h>
#include "data_bus.h"

using namespace std::chrono_literals;
using namespace std;

bool run {false};

void signalHandler(int sig)
{
    run = false;
    deinit();

    (void) signal(SIGINT, SIG_DFL);
}

int main()
{
    // read period
    const auto read_interval = 500ms;

    double speed;

    (void) signal(SIGINT, signalHandler);

    auto res = init();
    if (res > 0) {
        cout << "Data bus init error! (" << res << ")" << endl;
	return 1;
    }

    run = true;	

    while(run) {
        // consume speed value
	speed = consume();

        // set fixed floating format & precision for cout
        cout.setf(ios::fixed, ios::floatfield); 
        cout.precision(2);

        // print speed value
        cout << "Speed: " << setw(6) << speed << " kph" << endl;
        
        // sleep for read interval
        this_thread::sleep_for(read_interval);
    }
}
