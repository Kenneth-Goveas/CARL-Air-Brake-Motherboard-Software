#include <string>
#include <vector>

#include <math/linalg.hpp>

#include <sys/logging.hpp>
#include <sys/config.hpp>
#include <sys/option.hpp>
#include <sys/interrupt.hpp>
#include <sys/timing.hpp>

#include <ui/terminal.hpp>
#include <ui/panel.hpp>

#include <ipc/channel.hpp>

namespace logging :: local {
    std::string path = "log/system.log";
    std::string unit = "streamer";
    std::string mod = "main";
}

namespace config :: local {
    std::string path = "config/streamer.conf";
}

namespace option :: local {
    std::vector<std::string> args, names;
}

namespace timing :: local {
    std::string name_dur = "sleep_duration";
    double dur;
}

namespace panel :: local {
    int id_ctrl_head, id_ctrl_body;
    bool err_ctrl_lock, err_ctrl_read, err_ctrl_blank;

    int id_stat_head, id_stat_body;
    bool err_stat_lock, err_stat_read, err_stat_blank;

    int id_estm_head, id_estm_body;
    bool err_estm_lock, err_estm_read, err_estm_blank;

    int id_sens_head, id_sens_body;
    bool err_sens_lock, err_sens_read, err_sens_blank;
}

namespace channel :: local {
    std::string name_want_ctrl = "control", name_want_stat = "state", name_want_estm = "estimate", name_want_sens = "sensing";
    bool want_ctrl, want_stat, want_estm, want_sens;
    int count_want;

    bool acces;

    double ctrl;
    linalg::fvector stat, estm, sens;
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

    panel::init();

    channel::local::acces = false;

    while (!interrupt::caught()) {
        timing::sleep(timing::local::dur);

        panel::local::err_ctrl_lock = false;
        panel::local::err_ctrl_read = false;
        panel::local::err_ctrl_blank = false;

        panel::local::err_stat_lock = false;
        panel::local::err_stat_read = false;
        panel::local::err_stat_blank = false;

        panel::local::err_estm_lock = false;
        panel::local::err_estm_read = false;
        panel::local::err_estm_blank = false;

        panel::local::err_sens_lock = false;
        panel::local::err_sens_read = false;
        panel::local::err_sens_blank = false;

        if (!channel::local::acces) {
            channel::acquire_acces();
            if (channel::fail()) {
                panel::local::err_ctrl_lock = true;
                panel::local::err_stat_lock = true;
                panel::local::err_estm_lock = true;
                panel::local::err_sens_lock = true;
            } else {
                channel::local::acces = true;
            }
        }

        if (channel::local::acces && channel::local::want_ctrl) {
            channel::local::ctrl = channel::get_ctrl();
            if (channel::fail()) {
                panel::local::err_ctrl_read = true;
            } else if (channel::blank()) {
                panel::local::err_ctrl_blank = true;
            }
        }

        if (channel::local::acces && channel::local::want_stat) {
            channel::local::stat = channel::get_stat();
            if (channel::fail()) {
                panel::local::err_stat_read = true;
            } else if (channel::blank()) {
                panel::local::err_stat_blank = true;
            }
        }

        if (channel::local::acces && channel::local::want_estm) {
            channel::local::estm = channel::get_estm();
            if (channel::fail()) {
                panel::local::err_estm_read = true;
            } else if (channel::blank()) {
                panel::local::err_estm_blank = true;
            }
        }

        if (channel::local::acces && channel::local::want_sens) {
            channel::local::sens = channel::get_sens();
            if (channel::fail()) {
                panel::local::err_sens_read = true;
            } else if (channel::blank()) {
                panel::local::err_sens_blank = true;
            }
        }

        if (channel::local::acces) {
            channel::release_acces();
            if (channel::fail()) {
                panel::local::err_ctrl_lock = true;
                panel::local::err_stat_lock = true;
                panel::local::err_estm_lock = true;
                panel::local::err_sens_lock = true;
            } else {
                channel::local::acces = false;
            }
        }

        panel::close();

        if (channel::local::want_ctrl) {
            if (panel::local::err_ctrl_lock || panel::local::err_ctrl_read || panel::local::err_ctrl_blank) {
                panel::local::id_ctrl_head = panel::open(terminal::darkred, 2, 3, 1, 50);
                panel::local::id_ctrl_body = panel::open(terminal::lightgray, 3, 3, 3, 50);
            } else {
                panel::local::id_ctrl_head = panel::open(terminal::darkblue, 2, 3, 1, 50);
                panel::local::id_ctrl_body = panel::open(terminal::lightgray, 3, 3, 3, 50);
            }

            panel::put(terminal::white, panel::local::id_ctrl_head, 1, 2, "Control");
            panel::put(terminal::black, panel::local::id_ctrl_body, 2, 2, "Brake position");

            if (panel::local::err_ctrl_lock) {
                panel::put(terminal::darkred, panel::local::id_ctrl_body, 3, 2, "Channel lock error");
            } else if (panel::local::err_ctrl_read) {
                panel::put(terminal::darkred, panel::local::id_ctrl_body, 3, 2, "Channel read error");
            } else if (panel::local::err_ctrl_blank) {
                panel::put(terminal::darkred, panel::local::id_ctrl_body, 3, 2, "Channel blank error");
            } else {
                panel::put(terminal::black, panel::local::id_ctrl_body, 2, 39, channel::local::ctrl);
            }
        }

        if (channel::local::want_stat) {
            if (panel::local::err_stat_lock || panel::local::err_stat_read || panel::local::err_stat_blank) {
                panel::local::id_stat_head = panel::open(terminal::darkred, 15, 3, 1, 50);
                panel::local::id_stat_body = panel::open(terminal::lightgray, 16, 3, 9, 50);
            } else {
                panel::local::id_stat_head = panel::open(terminal::darkblue, 15, 3, 1, 50);
                panel::local::id_stat_body = panel::open(terminal::lightgray, 16, 3, 9, 50);
            }

            panel::put(terminal::white, panel::local::id_stat_head, 1, 2, "State");
            panel::put(terminal::black, panel::local::id_stat_body, 2, 2, "Vertical position (m)");
            panel::put(terminal::black, panel::local::id_stat_body, 3, 2, "Horizontal position (m)");
            panel::put(terminal::black, panel::local::id_stat_body, 4, 2, "Angular position (rad)");
            panel::put(terminal::black, panel::local::id_stat_body, 5, 2, "Vertical velocity (m/s)");
            panel::put(terminal::black, panel::local::id_stat_body, 6, 2, "Horizontal velocity (m/s)");
            panel::put(terminal::black, panel::local::id_stat_body, 7, 2, "Angular velocity (rad/s)");
            panel::put(terminal::black, panel::local::id_stat_body, 8, 2, "Brake position");

            if (panel::local::err_stat_lock) {
                panel::put(terminal::darkred, panel::local::id_stat_body, 9, 2, "Channel lock error");
            } else if (panel::local::err_stat_read) {
                panel::put(terminal::darkred, panel::local::id_stat_body, 9, 2, "Channel read error");
            } else if (panel::local::err_stat_blank) {
                panel::put(terminal::darkred, panel::local::id_stat_body, 9, 2, "Channel blank error");
            } else {
                for (int i = 0; i < 7; i++) {
                    panel::put(terminal::black, panel::local::id_stat_body, 2 + i, 39, channel::local::stat[i]);
                }
            }
        }

        if (channel::local::want_estm) {
            if (panel::local::err_estm_lock || panel::local::err_estm_read || panel::local::err_estm_blank) {
                panel::local::id_estm_head = panel::open(terminal::darkred, 7, 3, 1, 50);
                panel::local::id_estm_body = panel::open(terminal::lightgray, 8, 3, 6, 50);
            } else {
                panel::local::id_estm_head = panel::open(terminal::darkblue, 7, 3, 1, 50);
                panel::local::id_estm_body = panel::open(terminal::lightgray, 8, 3, 6, 50);
            }

            panel::put(terminal::white, panel::local::id_estm_head, 1, 2, "Estimate");
            panel::put(terminal::black, panel::local::id_estm_body, 2, 2, "Vertical position (m)");
            panel::put(terminal::black, panel::local::id_estm_body, 3, 2, "Angular position (rad)");
            panel::put(terminal::black, panel::local::id_estm_body, 4, 2, "Vertical velocity (m/s)");
            panel::put(terminal::black, panel::local::id_estm_body, 5, 2, "Angular velocity (rad/s)");

            if (panel::local::err_estm_lock) {
                panel::put(terminal::darkred, panel::local::id_estm_body, 6, 2, "Channel lock error");
            } else if (panel::local::err_estm_read) {
                panel::put(terminal::darkred, panel::local::id_estm_body, 6, 2, "Channel read error");
            } else if (panel::local::err_estm_blank) {
                panel::put(terminal::darkred, panel::local::id_estm_body, 6, 2, "Channel blank error");
            } else {
                for (int i = 0; i < 4; i++) {
                    panel::put(terminal::black, panel::local::id_estm_body, 2 + i, 39, channel::local::estm[i]);
                }
            }
        }

        if (channel::local::want_sens) {
            if (panel::local::err_sens_lock || panel::local::err_sens_read || panel::local::err_sens_blank) {
                panel::local::id_sens_head = panel::open(terminal::darkred, 2, 55, 1, 50);
                panel::local::id_sens_body = panel::open(terminal::lightgray, 3, 55, 19, 50);
            } else {
                panel::local::id_sens_head = panel::open(terminal::darkblue, 2, 55, 1, 50);
                panel::local::id_sens_body = panel::open(terminal::lightgray, 3, 55, 19, 50);
            }

            panel::put(terminal::white, panel::local::id_sens_head, 1, 2, "Sensing");
            panel::put(terminal::black, panel::local::id_sens_body, 2, 2, "Angular velocity X (rad/s)");
            panel::put(terminal::black, panel::local::id_sens_body, 3, 2, "Angular velocity Y (rad/s)");
            panel::put(terminal::black, panel::local::id_sens_body, 4, 2, "Angular velocity Z (rad/s)");
            panel::put(terminal::black, panel::local::id_sens_body, 5, 2, "Attitude quaternion W");
            panel::put(terminal::black, panel::local::id_sens_body, 6, 2, "Attitude quaternion X");
            panel::put(terminal::black, panel::local::id_sens_body, 7, 2, "Attitude quaternion Y");
            panel::put(terminal::black, panel::local::id_sens_body, 8, 2, "Attitude quaternion Z");
            panel::put(terminal::black, panel::local::id_sens_body, 9, 2, "Linear acceleration X (m/s²)");
            panel::put(terminal::black, panel::local::id_sens_body, 10, 2, "Linear acceleration Y (m/s²)");
            panel::put(terminal::black, panel::local::id_sens_body, 11, 2, "Linear acceleration Z (m/s²)");
            panel::put(terminal::black, panel::local::id_sens_body, 12, 2, "Pressure (Pa)");
            panel::put(terminal::black, panel::local::id_sens_body, 13, 2, "Temperature (K)");
            panel::put(terminal::black, panel::local::id_sens_body, 14, 2, "Angular velocity time (s)");
            panel::put(terminal::black, panel::local::id_sens_body, 15, 2, "Attitude quaternion time (s)");
            panel::put(terminal::black, panel::local::id_sens_body, 16, 2, "Linear acceleration time (s)");
            panel::put(terminal::black, panel::local::id_sens_body, 17, 2, "Pressure time (s)");
            panel::put(terminal::black, panel::local::id_sens_body, 18, 2, "Temperature time (s)");

            if (panel::local::err_sens_lock) {
                panel::put(terminal::darkred, panel::local::id_sens_body, 19, 2, "Channel lock error");
            } else if (panel::local::err_sens_read) {
                panel::put(terminal::darkred, panel::local::id_sens_body, 19, 2, "Channel read error");
            } else if (panel::local::err_sens_blank) {
                panel::put(terminal::darkred, panel::local::id_sens_body, 19, 2, "Channel blank error");
            } else {
                for (int i = 0; i < 17; i++) {
                    panel::put(terminal::black, panel::local::id_sens_body, 2 + i, 39, channel::local::sens[i]);
                }
            }
        }
    }

    panel::deinit();

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
