#include <chrono>
#include <thread>
#include <string>

#include <sys/logging.hpp>
#include <sys/timing.hpp>

namespace timing :: intern {

std::string mod = "timing";

bool fail;

}

namespace timing {

bool fail (void) {
    return intern::fail;
}

double get_time (void) {
    double time;

    logging::inf(intern::mod,
        "Getting time"
    );

    auto tstmp = std::chrono::steady_clock::now().time_since_epoch();
    auto sec = std::chrono::duration_cast<std::chrono::duration<double>>(tstmp);

    time = sec.count();

    logging::inf(intern::mod,
        "Got time: ", time, "s"
    );

    return time;
}

void sleep (double dur) {
    logging::inf(intern::mod,
        "Sleeping: Dur: ", dur, "s"
    );

    if (dur <= 0) {
        logging::err(intern::mod,
            "Failed to sleep (Invalid duration)"
        );
        intern::fail = true;
        return;
    }

    std::this_thread::sleep_for(std::chrono::duration<double>(dur));

    intern::fail = false;
}

}
