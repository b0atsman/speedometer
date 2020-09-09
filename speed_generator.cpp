#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include <signal.h>
#include "data_bus.h"

using namespace std::chrono;
using namespace std;

bool run {false};

// degrees to radians conversion 
inline double DegToRad(const double deg) { return deg * M_PI / 180.0; }

// static class for time based speed value generation
class SpeedGenerator
{
private:

    // ms divider for slow oscillating speed 
    static const auto seed_slow_divider {50};
    // ms divider for fast oscillating speed 
    static const auto seed_fast_divider {5};
    // multiplier for slow oscillating speed 
    static const auto speed_high_multiplier {50};
    // multiplier for fast oscillating speed 
    static const auto speed_low_multiplier {5};

    SpeedGenerator() {};

public:

    // speed generation routine
    static double generateSpeed(long int seed_ms) 
    {
	static_assert(seed_slow_divider > 0, "Invalid (zero) slow seed divider!");
	static_assert(seed_fast_divider > 0, "Invalid (zero) fast seed divider!");

        // calculating seeds
        int seed_slow = (seed_ms / seed_slow_divider) % 360;
        int seed_fast = (seed_ms / seed_fast_divider) % 360;
    
        // calculating integer slow and fast oscillating speed parts, multiplied by precision depth
        double speed_high = (sin(DegToRad(seed_slow)) + 1) * speed_high_multiplier;
        double speed_low = (sin(DegToRad(seed_fast)) + 1) * speed_low_multiplier;

        // returning sum of speeds divided by precision depth 
        return (speed_high + speed_low);
    }
};

void signalHandler(int sig)
{
    run = false;
    deinit();

    (void) signal(SIGINT, SIG_DFL);
}

int main()
{
    // generation interval
    const auto gen_interval = 500ms;
    // generated speed value
    double speed;
    // ms seed
    long int seed;
 
/* 
    // test cases
    cout << SpeedGenerator::generateSpeed(0) << endl;		// 55.0
    cout << SpeedGenerator::generateSpeed(50) << endl;		// 56.73
    cout << SpeedGenerator::generateSpeed(100) << endl;		// 58.45
    cout << SpeedGenerator::generateSpeed(360) << endl;		// 65.84
    cout << SpeedGenerator::generateSpeed(1000) << endl;	// 70.38
*/ 
    (void) signal(SIGINT, signalHandler);

    auto res = init();
    if (res > 0) {
        cout << "Data bus init error! (" << res << ")" << endl;
	return 1;
    }

    run = true;	

    cout << "Speed value generator started. Press Ctrl+C to stop..." << endl;
    
    while (run) {
        // calculating ms seed from system time	
        seed = duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
	
        // generate speed value	
	speed = SpeedGenerator::generateSpeed(seed);
	
	// poulate speed value 
        populate(speed);
	
	// sleep for generation interval	
        this_thread::sleep_for(gen_interval);
    }
}
