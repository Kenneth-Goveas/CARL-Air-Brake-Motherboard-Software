#include <string>
#include <vector>

#include <math/linalg.hpp>

#include <sys/logging.hpp>
#include <sys/interrupt.hpp>
#include <sys/timing.hpp>
#include <sys/option.hpp>
#include <sys/config.hpp>

#include <dat/csvwrite.hpp>

#include <ipc/channel.hpp>

namespace logging::local {
    std::string path = "log/system.log";
    std::string app = "recorder";
    std::string lib = "main";
}

namespace timing::local {
    std::string dur_name = "sleep_duration";
    double time, dur;
}

namespace option::local {
    std::vector<std::string> names;
}

namespace config::local {
    std::string path = "config/recorder.conf";
}

namespace channel::local {
    bool acces = false, error;
    double ctrl;
    linalg::fvector stat, estm, sens;
}

namespace csvwrite::local {
    std::string path = "output/flight-data.csv";
    std::vector<std::string> head;
    std::vector<double> body;
}

namespace local {
    std::string use_ctrl_name = "control", use_stat_name = "state", use_estm_name = "estimate", use_sens_name = "sensing";
    bool use_ctrl, use_stat, use_estm, use_sens;
    int use_count = 0;

    class except {};
}

int main (int argc, char ** argv) {
    if (!logging::instance::start(logging::local::path, logging::local::app)) {
        return 1;
    }

    logging::instance logging(logging::local::lib);
    logging.inf("Starting");

    try {
        option::local::names = {local::use_ctrl_name, local::use_stat_name, local::use_estm_name, local::use_sens_name};
        option::instance option(argc, argv, option::local::names);

        local::use_ctrl = option.get(local::use_ctrl_name);
        local::use_stat = option.get(local::use_stat_name);
        local::use_estm = option.get(local::use_estm_name);
        local::use_sens = option.get(local::use_sens_name);

        for (auto use : {local::use_ctrl, local::use_stat, local::use_estm, local::use_sens}) {
            if (use) {
                local::use_count++;
            }
        }

        if (local::use_count == 0) {
            logging.err("At least one channel must be selected");
            throw local::except{};
        }

        config::instance config(config::local::path);

        timing::local::dur = config.get<double>(timing::local::dur_name);

        interrupt::instance interrupt;
        timing::instance timing;

        csvwrite::instance csvwrite(csvwrite::local::path);

        channel::instance channel(channel::mode::none);

        csvwrite::local::head.push_back("System time (s)");

        if (local::use_ctrl) {
            csvwrite::local::head.push_back("Applied brake position");
        }

        if (local::use_stat) {
            csvwrite::local::head.push_back("True vertical position (m)");
            csvwrite::local::head.push_back("True horizontal position (m)");
            csvwrite::local::head.push_back("True angular position (rad)");
            csvwrite::local::head.push_back("True vertical velocity (m/s)");
            csvwrite::local::head.push_back("True horizontal velocity (m/s)");
            csvwrite::local::head.push_back("True angular velocity (rad/s)");
            csvwrite::local::head.push_back("True brake position");
        }

        if (local::use_estm) {
            csvwrite::local::head.push_back("Estimated vertical position (m)");
            csvwrite::local::head.push_back("Estimated angular position (rad)");
            csvwrite::local::head.push_back("Estimated vertical velocity (m/s)");
            csvwrite::local::head.push_back("Estimated angular velocity (rad/s)");
        }

        if (local::use_sens) {
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

        csvwrite.put_head(csvwrite::local::head);

        while (!interrupt.caught()) {
            timing.sleep(timing::local::dur);

            channel::local::error = false;

            try {
                if (!channel::local::acces) {
                    channel.acquire();
                    channel::local::acces = true;
                }
            } catch (...) {
                channel::local::error = true;
            }

            timing::local::time = timing.get_time();

            try {
                if (channel::local::acces && local::use_ctrl) {
                    channel::local::ctrl = channel.get_ctrl();
                }
            } catch (...) {
                channel::local::error = true;
            }

            try {
                if (channel::local::acces && local::use_stat) {
                    channel::local::stat = channel.get_stat();
                }
            } catch (...) {
                channel::local::error = true;
            }

            try {
                if (channel::local::acces && local::use_estm) {
                    channel::local::estm = channel.get_estm();
                }
            } catch (...) {
                channel::local::error = true;
            }

            try {
                if (channel::local::acces && local::use_sens) {
                    channel::local::sens = channel.get_sens();
                }
            } catch (...) {
                channel::local::error = true;
            }

            try {
                if (channel::local::acces) {
                    channel.release();
                    channel::local::acces = false;
                }
            } catch (...) {
                channel::local::error = true;
            }

            if (channel::local::error) {
                continue;
            }

            csvwrite::local::body.clear();
            csvwrite::local::body.push_back(timing::local::time);
            if (local::use_ctrl) {
                csvwrite::local::body.push_back(channel::local::ctrl);
            }
            if (local::use_stat) {
                for (int i = 0; i < 7; i++) {
                    csvwrite::local::body.push_back(channel::local::stat[i]);
                }
            }
            if (local::use_estm) {
                for (int i = 0; i < 4; i++) {
                    csvwrite::local::body.push_back(channel::local::estm[i]);
                }
            }
            if (local::use_sens) {
                for (int i = 0; i < 17; i++) {
                    csvwrite::local::body.push_back(channel::local::sens[i]);
                }
            }

            csvwrite.put_body(csvwrite::local::body);
        }
    } catch (...) {
        logging.wrn("Exiting");
        logging::instance::stop();
        return 1;
    }

    logging.inf("Exiting");
    logging::instance::stop();

    return 0;
}
