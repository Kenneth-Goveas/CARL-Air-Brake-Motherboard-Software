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
}

namespace panel :: local {
    int id_inpt_head, id_inpt_body, id_inpt_field;
    bool err_inpt_lock, err_inpt_write, err_inpt_syntax;

    int id_oupt_head, id_oupt_body;
    bool err_oupt_lock, err_oupt_read, err_oupt_blank;
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

    if (channel::local::count_want != 1) {
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

    panel::local::err_inpt_lock = false;
    panel::local::err_inpt_write = false;
    panel::local::err_inpt_syntax = false;

    while (!interrupt::caught()) {
        panel::local::err_oupt_lock = false;
        panel::local::err_oupt_read = false;
        panel::local::err_oupt_blank = false;

        if (!channel::local::acces) {
            channel::acquire_acces();
            if (channel::fail()) {
                panel::local::err_oupt_lock = true;
            } else {
                channel::local::acces = true;
            }
        }

        if (channel::local::acces && channel::local::want_ctrl) {
            channel::local::ctrl = channel::get_ctrl();
            if (channel::fail()) {
                panel::local::err_oupt_read = true;
            } else if (channel::blank()) {
                panel::local::err_oupt_blank = true;
            }
        }

        if (channel::local::acces && channel::local::want_stat) {
            channel::local::stat = channel::get_stat();
            if (channel::fail()) {
                panel::local::err_oupt_read = true;
            } else if (channel::blank()) {
                panel::local::err_oupt_blank = true;
            }
        }

        if (channel::local::acces && channel::local::want_estm) {
            channel::local::estm = channel::get_estm();
            if (channel::fail()) {
                panel::local::err_oupt_read = true;
            } else if (channel::blank()) {
                panel::local::err_oupt_blank = true;
            }
        }

        if (channel::local::acces && channel::local::want_sens) {
            channel::local::sens = channel::get_sens();
            if (channel::fail()) {
                panel::local::err_oupt_read = true;
            } else if (channel::blank()) {
                panel::local::err_oupt_blank = true;
            }
        }

        if (channel::local::acces) {
            channel::release_acces();
            if (channel::fail()) {
                panel::local::err_oupt_lock = true;
            } else {
                channel::local::acces = false;
            }
        }

        panel::close();

        if (channel::local::want_ctrl) {
            if (panel::local::err_oupt_lock || panel::local::err_oupt_read || panel::local::err_oupt_blank) {
                panel::local::id_oupt_head = panel::open(terminal::darkred, 7, 3, 1, 50);
                panel::local::id_oupt_body = panel::open(terminal::lightgray, 8, 3, 3, 50);
            } else {
                panel::local::id_oupt_head = panel::open(terminal::darkblue, 7, 3, 1, 50);
                panel::local::id_oupt_body = panel::open(terminal::lightgray, 8, 3, 3, 50);
            }

            panel::put(terminal::white, panel::local::id_oupt_head, 1, 2, "Control");
            panel::put(terminal::black, panel::local::id_oupt_body, 2, 2, "Brake position");

            if (panel::local::err_oupt_lock) {
                panel::put(terminal::darkred, panel::local::id_oupt_body, 3, 2, "Channel lock error");
            } else if (panel::local::err_oupt_read) {
                panel::put(terminal::darkred, panel::local::id_oupt_body, 3, 2, "Channel read error");
            } else if (panel::local::err_oupt_blank) {
                panel::put(terminal::darkred, panel::local::id_oupt_body, 3, 2, "Channel blank error");
            } else {
                panel::put(terminal::black, panel::local::id_oupt_body, 2, 39, channel::local::ctrl);
            }
        }

        if (channel::local::want_stat) {
            if (panel::local::err_oupt_lock || panel::local::err_oupt_read || panel::local::err_oupt_blank) {
                panel::local::id_oupt_head = panel::open(terminal::darkred, 7, 3, 1, 50);
                panel::local::id_oupt_body = panel::open(terminal::lightgray, 8, 3, 9, 50);
            } else {
                panel::local::id_oupt_head = panel::open(terminal::darkblue, 7, 3, 1, 50);
                panel::local::id_oupt_body = panel::open(terminal::lightgray, 8, 3, 9, 50);
            }

            panel::put(terminal::white, panel::local::id_oupt_head, 1, 2, "State");
            panel::put(terminal::black, panel::local::id_oupt_body, 2, 2, "Vertical position (m)");
            panel::put(terminal::black, panel::local::id_oupt_body, 3, 2, "Horizontal position (m)");
            panel::put(terminal::black, panel::local::id_oupt_body, 4, 2, "Angular position (rad)");
            panel::put(terminal::black, panel::local::id_oupt_body, 5, 2, "Vertical velocity (m/s)");
            panel::put(terminal::black, panel::local::id_oupt_body, 6, 2, "Horizontal velocity (m/s)");
            panel::put(terminal::black, panel::local::id_oupt_body, 7, 2, "Angular velocity (rad/s)");
            panel::put(terminal::black, panel::local::id_oupt_body, 8, 2, "Brake position");

            if (panel::local::err_oupt_lock) {
                panel::put(terminal::darkred, panel::local::id_oupt_body, 9, 2, "Channel lock error");
            } else if (panel::local::err_oupt_read) {
                panel::put(terminal::darkred, panel::local::id_oupt_body, 9, 2, "Channel read error");
            } else if (panel::local::err_oupt_blank) {
                panel::put(terminal::darkred, panel::local::id_oupt_body, 9, 2, "Channel blank error");
            } else {
                for (int i = 0; i < 7; i++) {
                    panel::put(terminal::black, panel::local::id_oupt_body, 2 + i, 39, channel::local::stat[i]);
                }
            }
        }

        if (channel::local::want_estm) {
            if (panel::local::err_oupt_lock || panel::local::err_oupt_read || panel::local::err_oupt_blank) {
                panel::local::id_oupt_head = panel::open(terminal::darkred, 7, 3, 1, 50);
                panel::local::id_oupt_body = panel::open(terminal::lightgray, 8, 3, 6, 50);
            } else {
                panel::local::id_oupt_head = panel::open(terminal::darkblue, 7, 3, 1, 50);
                panel::local::id_oupt_body = panel::open(terminal::lightgray, 8, 3, 6, 50);
            }

            panel::put(terminal::white, panel::local::id_oupt_head, 1, 2, "Estimate");
            panel::put(terminal::black, panel::local::id_oupt_body, 2, 2, "Vertical position (m)");
            panel::put(terminal::black, panel::local::id_oupt_body, 3, 2, "Angular position (rad)");
            panel::put(terminal::black, panel::local::id_oupt_body, 4, 2, "Vertical velocity (m/s)");
            panel::put(terminal::black, panel::local::id_oupt_body, 5, 2, "Angular velocity (rad/s)");

            if (panel::local::err_oupt_lock) {
                panel::put(terminal::darkred, panel::local::id_oupt_body, 6, 2, "Channel lock error");
            } else if (panel::local::err_oupt_read) {
                panel::put(terminal::darkred, panel::local::id_oupt_body, 6, 2, "Channel read error");
            } else if (panel::local::err_oupt_blank) {
                panel::put(terminal::darkred, panel::local::id_oupt_body, 6, 2, "Channel blank error");
            } else {
                for (int i = 0; i < 4; i++) {
                    panel::put(terminal::black, panel::local::id_oupt_body, 2 + i, 39, channel::local::estm[i]);
                }
            }
        }

        if (channel::local::want_sens) {
            if (panel::local::err_oupt_lock || panel::local::err_oupt_read || panel::local::err_oupt_blank) {
                panel::local::id_oupt_head = panel::open(terminal::darkred, 7, 3, 1, 50);
                panel::local::id_oupt_body = panel::open(terminal::lightgray, 8, 3, 19, 50);
            } else {
                panel::local::id_oupt_head = panel::open(terminal::darkblue, 7, 3, 1, 50);
                panel::local::id_oupt_body = panel::open(terminal::lightgray, 8, 3, 19, 50);
            }

            panel::put(terminal::white, panel::local::id_oupt_head, 1, 2, "Sensing");
            panel::put(terminal::black, panel::local::id_oupt_body, 2, 2, "Angular velocity X (rad/s)");
            panel::put(terminal::black, panel::local::id_oupt_body, 3, 2, "Angular velocity Y (rad/s)");
            panel::put(terminal::black, panel::local::id_oupt_body, 4, 2, "Angular velocity Z (rad/s)");
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

            if (panel::local::err_oupt_lock) {
                panel::put(terminal::darkred, panel::local::id_oupt_body, 19, 2, "Channel lock error");
            } else if (panel::local::err_oupt_read) {
                panel::put(terminal::darkred, panel::local::id_oupt_body, 19, 2, "Channel read error");
            } else if (panel::local::err_oupt_blank) {
                panel::put(terminal::darkred, panel::local::id_oupt_body, 19, 2, "Channel blank error");
            } else {
                for (int i = 0; i < 17; i++) {
                    panel::put(terminal::black, panel::local::id_oupt_body, 2 + i, 39, channel::local::sens[i]);
                }
            }
        }

        if (panel::local::err_inpt_lock || panel::local::err_inpt_write || panel::local::err_inpt_syntax) {
            panel::local::id_inpt_head = panel::open(terminal::darkred, 2, 3, 1, 50);
            panel::local::id_inpt_body = panel::open(terminal::lightgray, 3, 3, 3, 50);
            panel::local::id_inpt_field = panel::open(terminal::darkgray, 4, 4, 1, 48);
        } else {
            panel::local::id_inpt_head = panel::open(terminal::darkblue, 2, 3, 1, 50);
            panel::local::id_inpt_body = panel::open(terminal::lightgray, 3, 3, 3, 50);
            panel::local::id_inpt_field = panel::open(terminal::darkgray, 4, 4, 1, 48);
        }

        panel::put(terminal::white, panel::local::id_inpt_head, 1, 2, "Input");

        if (panel::local::err_inpt_lock) {
            panel::put(terminal::darkred, panel::local::id_inpt_body, 3, 2, "Channel lock error");
        } else if (panel::local::err_inpt_write) {
            panel::put(terminal::darkred, panel::local::id_inpt_body, 3, 2, "Channel write error");
        } else if (panel::local::err_inpt_syntax) {
            panel::put(terminal::darkred, panel::local::id_inpt_body, 3, 2, "Input syntax error");
        }

        panel::local::err_inpt_lock = false;
        panel::local::err_inpt_write = false;
        panel::local::err_inpt_syntax = false;

        if (channel::local::want_ctrl) {
            channel::local::ctrl = panel::get<double>(terminal::white, panel::local::id_inpt_field, 1, 1);
            if (panel::fail()) {
                panel::local::err_inpt_syntax = true;
                continue;
            } else if (panel::eof()) {
                break;
            }
        }

        if (channel::local::want_stat) {
            channel::local::stat = panel::get<linalg::fvector>(terminal::white, panel::local::id_inpt_field, 1, 1);
            if (panel::fail()) {
                panel::local::err_inpt_syntax = true;
                continue;
            } else if (panel::eof()) {
                break;
            }
        }

        if (channel::local::want_estm) {
            channel::local::estm = panel::get<linalg::fvector>(terminal::white, panel::local::id_inpt_field, 1, 1);
            if (panel::fail()) {
                panel::local::err_inpt_syntax = true;
                continue;
            } else if (panel::eof()) {
                break;
            }
        }

        if (channel::local::want_sens) {
            channel::local::sens = panel::get<linalg::fvector>(terminal::white, panel::local::id_inpt_field, 1, 1);
            if (panel::fail()) {
                panel::local::err_inpt_syntax = true;
                continue;
            } else if (panel::eof()) {
                break;
            }
        }

        if (!channel::local::acces) {
            channel::acquire_acces();
            if (channel::fail()) {
                panel::local::err_inpt_lock = true;
            } else {
                channel::local::acces = true;
            }
        }

        if (channel::local::acces && channel::local::want_ctrl) {
            channel::put_ctrl(channel::local::ctrl);
            if (channel::fail()) {
                panel::local::err_inpt_write = true;
            }
        }

        if (channel::local::acces && channel::local::want_stat) {
            channel::put_stat(channel::local::stat);
            if (channel::fail()) {
                panel::local::err_inpt_write = true;
            }
        }

        if (channel::local::acces && channel::local::want_estm) {
            channel::put_estm(channel::local::estm);
            if (channel::fail()) {
                panel::local::err_inpt_write = true;
            }
        }

        if (channel::local::acces && channel::local::want_sens) {
            channel::put_sens(channel::local::sens);
            if (channel::fail()) {
                panel::local::err_inpt_write = true;
            }
        }

        if (channel::local::acces) {
            channel::release_acces();
            if (channel::fail()) {
                panel::local::err_inpt_lock = true;
            } else {
                channel::local::acces = false;
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
