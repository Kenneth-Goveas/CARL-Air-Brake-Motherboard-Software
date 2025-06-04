#include <chrono>
#include <thread>

#include <sys/logging.hpp>
#include <sys/timing.hpp>

namespace timing {

bool tracker::exist = false;

bool tracker::instantiate (void) {
    if (tracker::exist) {
        return false;
    } else {
        tracker::exist = true;
        return true;
    }
}

instance::instance (void) : logging("timing"), init(false) {
    this->logging.inf("Initializing instance");

    if (!instance::instantiate()) {
        this->logging.err("Failed to initialize instance (Instance already exists)");
        throw except_ctor::fail;
    }

    this->init = true;
}

double instance::get_time (void) {
    double time;

    this->logging.inf("Getting time stamp");

    if (!this->init) {
        this->logging.err("Failed to get time stamp (Instance not initialized)");
        throw except_get_time::fail;
    }

    auto tstamp = std::chrono::steady_clock::now().time_since_epoch();
    auto sec = std::chrono::duration_cast<std::chrono::duration<double>>(tstamp);

    time = sec.count();

    this->logging.inf("Got time stamp: ", time);

    return time;
}

void instance::sleep (double dur) {
    this->logging.inf("Sleeping: Dur: ", dur);

    if (!this->init) {
        this->logging.err("Failed to sleep (Instance not initialized)");
        throw except_sleep::fail;
    }

    if (dur < 0) {
        this->logging.inf("Failed to sleep (Invalid duration)");
        throw except_sleep::fail;
    }

    std::this_thread::sleep_for(std::chrono::duration<double>(dur));
}

}
