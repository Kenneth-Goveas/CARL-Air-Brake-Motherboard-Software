#include <cstdint>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <chrono>
#include <thread>

#include <sys/logging.hpp>
#include <sys/timing.hpp>

namespace timing :: intern {

std::string mod = "timing";

bool fail;

std::string conv_val_to_oupt (double val);

}

namespace timing {

bool fail (void) {
    return intern::fail;
}

double get_time (void) {
    auto tstamp = std::chrono::steady_clock::now().time_since_epoch();
    double time;
    std::string oupt;

    logging::inf(intern::mod,
        "Getting current time"
    );

    time = std::chrono::duration_cast<std::chrono::nanoseconds>(tstamp).count();
    time *= 1e-9;

    oupt = intern::conv_val_to_oupt(time);
    logging::inf(intern::mod,
        "Got current time: ", oupt, "s"
    );

    return time;
}

void pause (double dur) {
    std::chrono::nanoseconds nsec((std::int64_t)(1e9 * dur));
    std::string oupt;

    oupt = intern::conv_val_to_oupt(dur);
    logging::inf(intern::mod,
        "Entering sleep interval: Dur: ", oupt, "s"
    );

    if (!(dur > 0)) {
        logging::err(intern::mod,
            "Failed to enter sleep interval (Invalid duration)"
        );
        intern::fail = true;
        return;
    }

    std::this_thread::sleep_for(nsec);

    intern::fail = false;
}

}

namespace timing :: intern {

std::string conv_val_to_oupt (double val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << std::showpos << std::scientific << std::setprecision(2) << val;
    oupt = ostr.str();

    return oupt;
}

}
