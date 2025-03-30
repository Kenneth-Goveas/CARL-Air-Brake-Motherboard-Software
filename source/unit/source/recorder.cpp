#include <string>
#include <vector>

#include <math/linalg.hpp>

#include <sys/logging.hpp>
#include <sys/config.hpp>
#include <sys/option.hpp>
#include <sys/interrupt.hpp>
#include <sys/timing.hpp>

#include <ipc/channel.hpp>

#include <dat/csvwrite.hpp>

namespace logging :: local {
    std::string path = "data/system.log";
    std::string unit = "recorder";
    std::string mod = "main";
}

namespace config :: local {
    std::string path = "config/recorder.conf";
}

namespace option :: local {
    std::vector<std::string> args, names;
}

namespace timing :: local {
    std::string name_dur = "sleep_duration";
    double time, dur;
}

namespace channel :: local {
    std::string name_want_ctrl = "control", name_want_stat = "state", name_want_estm = "estimate", name_want_sens = "sensing";
    bool want_ctrl, want_stat, want_estm, want_sens;
    int count_want;

    bool acces, error;

    double ctrl;
    linalg::fvector stat, estm, sens;
}

namespace csvwrite :: local {
    std::string path = "data/flight-record.csv";
    std::vector<std::string> head;
    std::vector<double> body;
}

int main (int argc, char ** argv) {
    logging::init(logging::local::path, logging::local::unit);
    if (logging::fail()) {
        return 1;
    }

    logging::inf(logging::local::mod,
        "Starting"
    );

    interrupt::init();
    if (interrupt::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    config::init(config::local::path);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    timing::local::dur = config::get<double>(timing::local::name_dur);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        option::local::args.push_back(argv[i]);
    }

    option::local::names = {
        channel::local::name_want_ctrl, channel::local::name_want_stat, channel::local::name_want_estm, channel::local::name_want_sens
    };

    option::init(option::local::args, option::local::names);
    if (option::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    channel::local::want_ctrl = option::get(channel::local::name_want_ctrl);
    if (option::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    channel::local::want_stat = option::get(channel::local::name_want_stat);
    if (option::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    channel::local::want_estm = option::get(channel::local::name_want_estm);
    if (option::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    channel::local::want_sens = option::get(channel::local::name_want_sens);
    if (option::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    channel::local::count_want = 0;
    for (auto want : {channel::local::want_ctrl, channel::local::want_stat, channel::local::want_estm, channel::local::want_sens}) {
        if (want) {
            channel::local::count_want++;
        }
    }

    if (channel::local::count_want == 0) {
        logging::err(logging::local::mod,
            "At least one channel must be selected"
        );
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    channel::init(channel::local::want_ctrl, channel::local::want_stat, channel::local::want_estm, channel::local::want_sens);

    channel::open();
    if (channel::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    csvwrite::open(csvwrite::local::path);
    if (csvwrite::fail()) {
        channel::close();
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    csvwrite::local::head.clear();
    csvwrite::local::head.push_back("System time (s)");
    if (channel::local::want_ctrl) {
        csvwrite::local::head.push_back("Applied brake position");
    }
    if (channel::local::want_stat) {
        csvwrite::local::head.push_back("True vertical position (m)");
        csvwrite::local::head.push_back("True horizontal position (m)");
        csvwrite::local::head.push_back("True angular position (rad)");
        csvwrite::local::head.push_back("True vertical velocity (m/s)");
        csvwrite::local::head.push_back("True horizontal velocity (m/s)");
        csvwrite::local::head.push_back("True angular velocity (rad/s)");
        csvwrite::local::head.push_back("True brake position");
    }
    if (channel::local::want_estm) {
        csvwrite::local::head.push_back("Estimated vertical position (m)");
        csvwrite::local::head.push_back("Estimated angular position (rad)");
        csvwrite::local::head.push_back("Estimated vertical velocity (m/s)");
        csvwrite::local::head.push_back("Estimated angular velocity (rad/s)");
    }
    if (channel::local::want_sens) {
        csvwrite::local::head.push_back("Measured angular velocity X (rad/s)");
        csvwrite::local::head.push_back("Measured angular velocity Y (rad/s)");
        csvwrite::local::head.push_back("Measured angular velocity Z (rad/s)");
        csvwrite::local::head.push_back("Measured attitude quaternion W");
        csvwrite::local::head.push_back("Measured attitude quaternion X");
        csvwrite::local::head.push_back("Measured attitude quaternion Y");
        csvwrite::local::head.push_back("Measured attitude quaternion Z");
        csvwrite::local::head.push_back("Measured linear acceleration X (m/s²)");
        csvwrite::local::head.push_back("Measured linear acceleration Y (m/s²)");
        csvwrite::local::head.push_back("Measured linear acceleration Z (m/s²)");
        csvwrite::local::head.push_back("Measured pressure (Pa)");
        csvwrite::local::head.push_back("Measured temperature (K)");
        csvwrite::local::head.push_back("Measured angular velocity time (s)");
        csvwrite::local::head.push_back("Measured attitude quaternion time (s)");
        csvwrite::local::head.push_back("Measured linear acceleration time (s)");
        csvwrite::local::head.push_back("Measured pressure time (s)");
        csvwrite::local::head.push_back("Measured temperature time (s)");
    }

    csvwrite::put_head(csvwrite::local::head);
    if (csvwrite::fail()) {
        csvwrite::close();
        channel::close();
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    channel::local::acces = false;

    while (!interrupt::caught()) {
        timing::sleep(timing::local::dur);

        channel::local::error = false;

        if (!channel::local::acces) {
            channel::acquire_acces();
            if (channel::fail()) {
                channel::local::error = true;
            } else {
                channel::local::acces = true;
            }
        }

        timing::local::time = timing::get_time();

        if (channel::local::acces && channel::local::want_ctrl) {
            channel::local::ctrl = channel::get_ctrl();
            if (channel::fail()) {
                channel::local::error = true;
            } else if (channel::blank()) {
                channel::local::error = true;
            }
        }

        if (channel::local::acces && channel::local::want_stat) {
            channel::local::stat = channel::get_stat();
            if (channel::fail()) {
                channel::local::error = true;
            } else if (channel::blank()) {
                channel::local::error = true;
            }
        }

        if (channel::local::acces && channel::local::want_estm) {
            channel::local::estm = channel::get_estm();
            if (channel::fail()) {
                channel::local::error = true;
            } else if (channel::blank()) {
                channel::local::error = true;
            }
        }

        if (channel::local::acces && channel::local::want_sens) {
            channel::local::sens = channel::get_sens();
            if (channel::fail()) {
                channel::local::error = true;
            } else if (channel::blank()) {
                channel::local::error = true;
            }
        }

        if (channel::local::acces) {
            channel::release_acces();
            if (channel::fail()) {
                channel::local::error = true;
            } else {
                channel::local::acces = false;
            }
        }

        if (channel::local::error) {
            continue;
        }

        csvwrite::local::body.clear();
        csvwrite::local::body.push_back(timing::local::time);
        if (channel::local::want_ctrl) {
            csvwrite::local::body.push_back(channel::local::ctrl);
        }
        if (channel::local::want_stat) {
            for (int i = 0; i < 7; i++) {
                csvwrite::local::body.push_back(channel::local::stat[i]);
            }
        }
        if (channel::local::want_estm) {
            for (int i = 0; i < 4; i++) {
                csvwrite::local::body.push_back(channel::local::estm[i]);
            }
        }
        if (channel::local::want_sens) {
            for (int i = 0; i < 17; i++) {
                csvwrite::local::body.push_back(channel::local::sens[i]);
            }
        }

        csvwrite::put_body(csvwrite::local::body);
    }

    csvwrite::close();
    if (csvwrite::fail()) {
        channel::close();
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    channel::close();
    if (channel::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    logging::inf(logging::local::mod,
        "Exiting"
    );

    logging::deinit();

    return 0;
}
