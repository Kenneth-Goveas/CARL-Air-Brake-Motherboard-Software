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
    std::chrono::steady_clock::time_point tpnt;
    std::chrono::steady_clock::duration tdur;
    std::chrono::duration<double> sec;
    double time;
    std::string oupt;

    logging::inf(intern::mod,
        "Getting current time"
    );

    tpnt = std::chrono::steady_clock::now();
    tdur = tpnt.time_since_epoch();

    sec = std::chrono::duration_cast<std::chrono::duration<double>>(tdur);
    time = sec.count();

    oupt = intern::conv_val_to_oupt(time);
    logging::inf(intern::mod,
        "Got current time: ", oupt, "s"
    );

    return time;
}

void pause (double dur) {
    std::chrono::duration<double> sec(dur);
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

    std::this_thread::sleep_for(sec);

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
