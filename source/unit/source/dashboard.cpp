#include <string>
#include <vector>

#include <math/linalg.hpp>

#include <sys/logging.hpp>
#include <sys/option.hpp>
#include <sys/interrupt.hpp>

#include <ui/terminal.hpp>
#include <ui/panel.hpp>

#include <ipc/channel.hpp>

namespace logging :: local {
    std::string path = "data/system.log";
    std::string unit = "dashboard";
    std::string mod = "main";
}

namespace option :: local {
    std::vector<std::string> args, names;
    std::string name_want_ctrl = "control";
    std::string name_want_stat = "state";
    std::string name_want_estm = "estimate";
    std::string name_want_sens = "sensing";
}

namespace panel :: local {
    int id_inpt_head, id_inpt_body, id_inpt_field;
    int id_oupt_head, id_oupt_body;
}

namespace channel :: local {
    bool want_ctrl, want_stat, want_estm, want_sens;
    bool exists, blank;
}

namespace local {
    int count_want;
    bool err_syn, err_dim;
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

    for (int i = 1; i < argc; i++) {
        option::local::args.push_back(argv[i]);
    }

    option::local::names = {
        option::local::name_want_ctrl, option::local::name_want_stat, option::local::name_want_estm, option::local::name_want_sens
    };

    option::init(option::local::args, option::local::names);
    if (option::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    channel::local::want_ctrl = option::get(option::local::name_want_ctrl);
    if (option::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    channel::local::want_stat = option::get(option::local::name_want_stat);
    if (option::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    channel::local::want_estm = option::get(option::local::name_want_estm);
    if (option::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    channel::local::want_sens = option::get(option::local::name_want_sens);
    if (option::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    local::count_want = 0;
    for (auto want : {channel::local::want_ctrl, channel::local::want_stat, channel::local::want_estm, channel::local::want_sens}) {
        if (want) {
            local::count_want++;
        }
    }

    if (local::count_want != 1) {
        logging::err(logging::local::mod,
            "Exactly one channel must be selected"
        );
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    channel::init(channel::local::want_ctrl, channel::local::want_stat, channel::local::want_estm, channel::local::want_sens);

    channel::local::exists = channel::exists();
    if (channel::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    if (!channel::local::exists) {
        logging::err(logging::local::mod,
            "Selected channel does not exist"
        );
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    channel::open();
    if (channel::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    panel::init();

    local::err_syn = false;
    local::err_dim = false;

    while (!interrupt::caught()) {
        channel::acquire_acces();
        if (channel::fail()) {
            panel::deinit();
            channel::close();
            logging::wrn(logging::local::mod,
                "Exiting"
            );
            logging::deinit();
            return 1;
        }

        if (channel::local::want_ctrl) {
            local::ctrl = channel::get_ctrl();
            if (channel::fail()) {
                panel::deinit();
                channel::release_acces();
                channel::close();
                logging::wrn(logging::local::mod,
                    "Exiting"
                );
                logging::deinit();
                return 1;
            }
            channel::local::blank = channel::blank();
        }

        if (channel::local::want_stat) {
            local::stat = channel::get_stat();
            if (channel::fail()) {
                panel::deinit();
                channel::release_acces();
                channel::close();
                logging::wrn(logging::local::mod,
                    "Exiting"
                );
                logging::deinit();
                return 1;
            }
            channel::local::blank = channel::blank();
        }

        if (channel::local::want_estm) {
            local::estm = channel::get_estm();
            if (channel::fail()) {
                panel::deinit();
                channel::release_acces();
                channel::close();
                logging::wrn(logging::local::mod,
                    "Exiting"
                );
                logging::deinit();
                return 1;
            }
            channel::local::blank = channel::blank();
        }

        if (channel::local::want_sens) {
            local::sens = channel::get_sens();
            if (channel::fail()) {
                panel::deinit();
                channel::release_acces();
                channel::close();
                logging::wrn(logging::local::mod,
                    "Exiting"
                );
                logging::deinit();
                return 1;
            }
            channel::local::blank = channel::blank();
        }

        channel::release_acces();
        if (channel::fail()) {
            panel::deinit();
            channel::close();
            logging::wrn(logging::local::mod,
                "Exiting"
            );
            logging::deinit();
            return 1;
        }

        panel::close();

        if (channel::local::want_ctrl) {
            if (channel::local::blank) {
                panel::local::id_oupt_head = panel::open(terminal::darkred, 7, 2, 1, 45);
                panel::local::id_oupt_body = panel::open(terminal::lightgray, 8, 2, 3, 45);
            } else {
                panel::local::id_oupt_head = panel::open(terminal::darkblue, 7, 2, 1, 45);
                panel::local::id_oupt_body = panel::open(terminal::lightgray, 8, 2, 3, 45);
            }

            panel::put(terminal::white, panel::local::id_oupt_head, 1, 2, "Control");
            panel::put(terminal::black, panel::local::id_oupt_body, 2, 2, "Brake position");

            if (!channel::local::blank) {
                panel::put(terminal::black, panel::local::id_oupt_body, 2, 34, local::ctrl);
            }
        }

        if (channel::local::want_stat) {
            if (channel::local::blank) {
                panel::local::id_oupt_head = panel::open(terminal::darkred, 7, 2, 1, 45);
                panel::local::id_oupt_body = panel::open(terminal::lightgray, 8, 2, 9, 45);
            } else {
                panel::local::id_oupt_head = panel::open(terminal::darkblue, 7, 2, 1, 45);
                panel::local::id_oupt_body = panel::open(terminal::lightgray, 8, 2, 9, 45);
            }

            panel::put(terminal::white, panel::local::id_oupt_head, 1, 2, "State");
            panel::put(terminal::black, panel::local::id_oupt_body, 2, 2, "Vertical position (m)");
            panel::put(terminal::black, panel::local::id_oupt_body, 3, 2, "Horizontal position (m)");
            panel::put(terminal::black, panel::local::id_oupt_body, 4, 2, "Angular position (rad)");
            panel::put(terminal::black, panel::local::id_oupt_body, 5, 2, "Vertical velocity (m/s)");
            panel::put(terminal::black, panel::local::id_oupt_body, 6, 2, "Horizontal velocity (m/s)");
            panel::put(terminal::black, panel::local::id_oupt_body, 7, 2, "Angular velocity (rad/s)");
            panel::put(terminal::black, panel::local::id_oupt_body, 8, 2, "Brake position");

            if (!channel::local::blank) {
                panel::put(terminal::black, panel::local::id_oupt_body, 2, 34, local::stat[0]);
                panel::put(terminal::black, panel::local::id_oupt_body, 3, 34, local::stat[1]);
                panel::put(terminal::black, panel::local::id_oupt_body, 4, 34, local::stat[2]);
                panel::put(terminal::black, panel::local::id_oupt_body, 5, 34, local::stat[3]);
                panel::put(terminal::black, panel::local::id_oupt_body, 6, 34, local::stat[4]);
                panel::put(terminal::black, panel::local::id_oupt_body, 7, 34, local::stat[5]);
                panel::put(terminal::black, panel::local::id_oupt_body, 8, 34, local::stat[6]);
            }
        }

        if (channel::local::want_estm) {
            if (channel::local::blank) {
                panel::local::id_oupt_head = panel::open(terminal::darkred, 7, 2, 1, 45);
                panel::local::id_oupt_body = panel::open(terminal::lightgray, 8, 2, 6, 45);
            } else {
                panel::local::id_oupt_head = panel::open(terminal::darkblue, 7, 2, 1, 45);
                panel::local::id_oupt_body = panel::open(terminal::lightgray, 8, 2, 6, 45);
            }

            panel::put(terminal::white, panel::local::id_oupt_head, 1, 2, "Estimate");
            panel::put(terminal::black, panel::local::id_oupt_body, 2, 2, "Vertical position (m)");
            panel::put(terminal::black, panel::local::id_oupt_body, 3, 2, "Angular position (rad)");
            panel::put(terminal::black, panel::local::id_oupt_body, 4, 2, "Vertical velocity (m/s)");
            panel::put(terminal::black, panel::local::id_oupt_body, 5, 2, "Angular velocity (rad/s)");

            if (!channel::local::blank) {
                panel::put(terminal::black, panel::local::id_oupt_body, 2, 34, local::estm[0]);
                panel::put(terminal::black, panel::local::id_oupt_body, 3, 34, local::estm[1]);
                panel::put(terminal::black, panel::local::id_oupt_body, 4, 34, local::estm[2]);
                panel::put(terminal::black, panel::local::id_oupt_body, 5, 34, local::estm[3]);
            }
        }

        if (channel::local::want_sens) {
            if (channel::local::blank) {
                panel::local::id_oupt_head = panel::open(terminal::darkred, 7, 2, 1, 45);
                panel::local::id_oupt_body = panel::open(terminal::lightgray, 8, 2, 19, 45);
            } else {
                panel::local::id_oupt_head = panel::open(terminal::darkblue, 7, 2, 1, 45);
                panel::local::id_oupt_body = panel::open(terminal::lightgray, 8, 2, 19, 45);
            }

            panel::put(terminal::white, panel::local::id_oupt_head, 1, 2, "Sensing");
            panel::put(terminal::black, panel::local::id_oupt_body, 2, 2, "Angular velocity X (rad/s)");
            panel::put(terminal::black, panel::local::id_oupt_body, 3, 2, "Angular velocity X (rad/s)");
            panel::put(terminal::black, panel::local::id_oupt_body, 4, 2, "Angular velocity X (rad/s)");
            panel::put(terminal::black, panel::local::id_oupt_body, 5, 2, "Attitude quaternion W");
            panel::put(terminal::black, panel::local::id_oupt_body, 6, 2, "Attitude quaternion X");
            panel::put(terminal::black, panel::local::id_oupt_body, 7, 2, "Attitude quaternion Y");
            panel::put(terminal::black, panel::local::id_oupt_body, 8, 2, "Attitude quaternion Z");
            panel::put(terminal::black, panel::local::id_oupt_body, 9, 2, "Linear acceleration X (m/s²)");
            panel::put(terminal::black, panel::local::id_oupt_body, 10, 2, "Linear acceleration Y (m/s²)");
            panel::put(terminal::black, panel::local::id_oupt_body, 11, 2, "Linear acceleration Z (m/s²)");
            panel::put(terminal::black, panel::local::id_oupt_body, 12, 2, "Pressure (Pa)");
            panel::put(terminal::black, panel::local::id_oupt_body, 13, 2, "Temperature (K)");
            panel::put(terminal::black, panel::local::id_oupt_body, 14, 2, "Angular velocity time (s)");
            panel::put(terminal::black, panel::local::id_oupt_body, 15, 2, "Attitude quaternion time (s)");
            panel::put(terminal::black, panel::local::id_oupt_body, 16, 2, "Linear acceleration time (s)");
            panel::put(terminal::black, panel::local::id_oupt_body, 17, 2, "Pressure time (s)");
            panel::put(terminal::black, panel::local::id_oupt_body, 18, 2, "Temperature time (s)");

            if (!channel::local::blank) {
                panel::put(terminal::black, panel::local::id_oupt_body, 2, 34, local::sens[0]);
                panel::put(terminal::black, panel::local::id_oupt_body, 3, 34, local::sens[1]);
                panel::put(terminal::black, panel::local::id_oupt_body, 4, 34, local::sens[2]);
                panel::put(terminal::black, panel::local::id_oupt_body, 5, 34, local::sens[3]);
                panel::put(terminal::black, panel::local::id_oupt_body, 6, 34, local::sens[4]);
                panel::put(terminal::black, panel::local::id_oupt_body, 7, 34, local::sens[5]);
                panel::put(terminal::black, panel::local::id_oupt_body, 8, 34, local::sens[6]);
                panel::put(terminal::black, panel::local::id_oupt_body, 9, 34, local::sens[7]);
                panel::put(terminal::black, panel::local::id_oupt_body, 10, 34, local::sens[8]);
                panel::put(terminal::black, panel::local::id_oupt_body, 11, 34, local::sens[9]);
                panel::put(terminal::black, panel::local::id_oupt_body, 12, 34, local::sens[10]);
                panel::put(terminal::black, panel::local::id_oupt_body, 13, 34, local::sens[11]);
                panel::put(terminal::black, panel::local::id_oupt_body, 14, 34, local::sens[12]);
                panel::put(terminal::black, panel::local::id_oupt_body, 15, 34, local::sens[13]);
                panel::put(terminal::black, panel::local::id_oupt_body, 16, 34, local::sens[14]);
                panel::put(terminal::black, panel::local::id_oupt_body, 17, 34, local::sens[15]);
                panel::put(terminal::black, panel::local::id_oupt_body, 18, 34, local::sens[16]);
            }
        }

        panel::local::id_inpt_head = panel::open(terminal::darkmagenta, 2, 2, 1, 90);
        panel::local::id_inpt_body = panel::open(terminal::lightgray, 3, 2, 3, 90);
        panel::local::id_inpt_field = panel::open(terminal::darkgray, 4, 3, 1, 88);

        panel::put(terminal::white, panel::local::id_inpt_head, 1, 2, "Input");
        if (local::err_syn) {
            panel::put(terminal::darkred, panel::local::id_inpt_body, 3, 2, "Syntax error");
        } else if (local::err_dim) {
            panel::put(terminal::darkred, panel::local::id_inpt_body, 3, 2, "Dimension error");
        }

        local::err_syn = false;
        local::err_dim = false;

        if (channel::local::want_ctrl) {
            local::ctrl = panel::get<double>(terminal::white, panel::local::id_inpt_field, 1, 1);
            if (panel::fail()) {
                local::err_syn = true;
                continue;
            } else if (panel::eof()) {
                break;
            }
        }

        if (channel::local::want_stat) {
            local::stat = panel::get<linalg::fvector>(terminal::white, panel::local::id_inpt_field, 1, 1);
            if (panel::fail()) {
                local::err_syn = true;
                continue;
            } else if (panel::eof()) {
                break;
            } else if (linalg::dim(local::stat) != 7) {
                local::err_dim = true;
                continue;
            }
        }

        if (channel::local::want_estm) {
            local::estm = panel::get<linalg::fvector>(terminal::white, panel::local::id_inpt_field, 1, 1);
            if (panel::fail()) {
                local::err_syn = true;
                continue;
            } else if (panel::eof()) {
                break;
            } else if (linalg::dim(local::estm) != 4) {
                local::err_dim = true;
                continue;
            }
        }

        if (channel::local::want_sens) {
            local::sens = panel::get<linalg::fvector>(terminal::white, panel::local::id_inpt_field, 1, 1);
            if (panel::fail()) {
                local::err_syn = true;
                continue;
            } else if (panel::eof()) {
                break;
            } else if (linalg::dim(local::sens) != 17) {
                local::err_dim = true;
                continue;
            }
        }

        channel::acquire_acces();
        if (channel::fail()) {
            panel::deinit();
            channel::close();
            logging::wrn(logging::local::mod,
                "Exiting"
            );
            logging::deinit();
            return 1;
        }

        if (channel::local::want_ctrl) {
            channel::put_ctrl(local::ctrl);
            if (channel::fail()) {
                panel::deinit();
                channel::release_acces();
                channel::close();
                logging::wrn(logging::local::mod,
                    "Exiting"
                );
                logging::deinit();
                return 1;
            }
        }

        if (channel::local::want_stat) {
            channel::put_stat(local::stat);
            if (channel::fail()) {
                panel::deinit();
                channel::release_acces();
                channel::close();
                logging::wrn(logging::local::mod,
                    "Exiting"
                );
                logging::deinit();
                return 1;
            }
        }

        if (channel::local::want_estm) {
            channel::put_estm(local::estm);
            if (channel::fail()) {
                panel::deinit();
                channel::release_acces();
                channel::close();
                logging::wrn(logging::local::mod,
                    "Exiting"
                );
                logging::deinit();
                return 1;
            }
        }

        if (channel::local::want_sens) {
            channel::put_sens(local::sens);
            if (channel::fail()) {
                panel::deinit();
                channel::release_acces();
                channel::close();
                logging::wrn(logging::local::mod,
                    "Exiting"
                );
                logging::deinit();
                return 1;
            }
        }

        channel::release_acces();
        if (channel::fail()) {
            panel::deinit();
            channel::close();
            logging::wrn(logging::local::mod,
                "Exiting"
            );
            logging::deinit();
            return 1;
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
