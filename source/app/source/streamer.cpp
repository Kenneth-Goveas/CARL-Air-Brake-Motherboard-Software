#include <string>
#include <vector>

#include <math/linalg.hpp>

#include <sys/logging.hpp>
#include <sys/interrupt.hpp>
#include <sys/timing.hpp>
#include <sys/option.hpp>
#include <sys/config.hpp>

#include <ui/terminal.hpp>
#include <ui/input.hpp>
#include <ui/output.hpp>
#include <ui/screen.hpp>
#include <ui/panel.hpp>

#include <ipc/channel.hpp>

namespace logging::local {
    std::string path = "log/system.log";
    std::string app = "streamer";
    std::string lib = "main";
}

namespace timing::local {
    std::string dur_name = "sleep_duration";
    double dur;
}

namespace option::local {
    std::vector<std::string> names;
}

namespace config::local {
    std::string path = "config/streamer.conf";
}

namespace panel::local {
    bool ctrl_err_lock, ctrl_err_read, ctrl_err_blank;
    bool stat_err_lock, stat_err_read, stat_err_blank;
    bool estm_err_lock, estm_err_read, estm_err_blank;
    bool sens_err_lock, sens_err_read, sens_err_blank;
}

namespace channel::local {
    bool acces = false;
    double ctrl;
    linalg::fvector stat, estm, sens;
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

        terminal::instance terminal;
        input::instance input;
        output::instance output;
        screen::instance screen(terminal);
        panel::instance panel_ctrl_head(terminal, input, output, terminal::color::darkblue, 21, 3, 1, 50),
                        panel_ctrl_body(terminal, input, output, terminal::color::lightgray, 22, 3, 3, 50),
                        panel_stat_head(terminal, input, output, terminal::color::darkblue, 10, 3, 1, 50),
                        panel_stat_body(terminal, input, output, terminal::color::lightgray, 11, 3, 9, 50),
                        panel_estm_head(terminal, input, output, terminal::color::darkblue, 2, 3, 1, 50),
                        panel_estm_body(terminal, input, output, terminal::color::lightgray, 3, 3, 6, 50),
                        panel_sens_head(terminal, input, output, terminal::color::darkblue, 2, 55, 1, 50),
                        panel_sens_body(terminal, input, output, terminal::color::lightgray, 3, 55, 19, 50);

        channel::instance channel(channel::mode::none);

        screen.erase();

        while (!interrupt.caught()) {
            timing.sleep(timing::local::dur);

            if (local::use_ctrl) {
                panel_ctrl_head.redraw();
                panel_ctrl_body.redraw();
                panel_ctrl_head.put(terminal::color::white, 1, 2, "Control");
                panel_ctrl_body.put(terminal::color::black, 2, 2, "Brake position");
            }

            if (local::use_stat) {
                panel_stat_head.redraw();
                panel_stat_body.redraw();
                panel_stat_head.put(terminal::color::white, 1, 2, "State");
                panel_stat_body.put(terminal::color::black, 2, 2, "Vertical position (m)");
                panel_stat_body.put(terminal::color::black, 3, 2, "Horizontal position (m)");
                panel_stat_body.put(terminal::color::black, 4, 2, "Angular position (rad)");
                panel_stat_body.put(terminal::color::black, 5, 2, "Vertical velocity (m/s)");
                panel_stat_body.put(terminal::color::black, 6, 2, "Horizontal velocity (m/s)");
                panel_stat_body.put(terminal::color::black, 7, 2, "Angular velocity (rad/s)");
                panel_stat_body.put(terminal::color::black, 8, 2, "Brake position");
            }

            if (local::use_estm) {
                panel_estm_head.redraw();
                panel_estm_body.redraw();
                panel_estm_head.put(terminal::color::white, 1, 2, "Estimate");
                panel_estm_body.put(terminal::color::black, 2, 2, "Vertical position (m)");
                panel_estm_body.put(terminal::color::black, 3, 2, "Angular position (rad)");
                panel_estm_body.put(terminal::color::black, 4, 2, "Vertical velocity (m/s)");
                panel_estm_body.put(terminal::color::black, 5, 2, "Angular velocity (rad/s)");
            }

            if (local::use_sens) {
                panel_sens_head.redraw();
                panel_sens_body.redraw();
                panel_sens_head.put(terminal::color::white, 1, 2, "Sensing");
                panel_sens_body.put(terminal::color::black, 2, 2, "Angular velocity X (rad/s)");
                panel_sens_body.put(terminal::color::black, 3, 2, "Angular velocity Y (rad/s)");
                panel_sens_body.put(terminal::color::black, 4, 2, "Angular velocity Z (rad/s)");
                panel_sens_body.put(terminal::color::black, 5, 2, "Attitude quaternion W");
                panel_sens_body.put(terminal::color::black, 6, 2, "Attitude quaternion X");
                panel_sens_body.put(terminal::color::black, 7, 2, "Attitude quaternion Y");
                panel_sens_body.put(terminal::color::black, 8, 2, "Attitude quaternion Z");
                panel_sens_body.put(terminal::color::black, 9, 2, "Linear acceleration X (m/s²)");
                panel_sens_body.put(terminal::color::black, 10, 2, "Linear acceleration Y (m/s²)");
                panel_sens_body.put(terminal::color::black, 11, 2, "Linear acceleration Z (m/s²)");
                panel_sens_body.put(terminal::color::black, 12, 2, "Pressure (Pa)");
                panel_sens_body.put(terminal::color::black, 13, 2, "Temperature (K)");
                panel_sens_body.put(terminal::color::black, 14, 2, "Angular velocity time (s)");
                panel_sens_body.put(terminal::color::black, 15, 2, "Attitude quaternion time (s)");
                panel_sens_body.put(terminal::color::black, 16, 2, "Linear acceleration time (s)");
                panel_sens_body.put(terminal::color::black, 17, 2, "Pressure time (s)");
                panel_sens_body.put(terminal::color::black, 18, 2, "Temperature time (s)");
            }

            panel::local::ctrl_err_lock = false;
            panel::local::ctrl_err_read = false;
            panel::local::ctrl_err_blank = false;

            panel::local::stat_err_lock = false;
            panel::local::stat_err_read = false;
            panel::local::stat_err_blank = false;

            panel::local::estm_err_lock = false;
            panel::local::estm_err_read = false;
            panel::local::estm_err_blank = false;

            panel::local::sens_err_lock = false;
            panel::local::sens_err_read = false;
            panel::local::sens_err_blank = false;

            try {
                if (!channel::local::acces) {
                    channel.acquire();
                    channel::local::acces = true;
                }
            } catch (...) {
                panel::local::ctrl_err_lock = true;
                panel::local::stat_err_lock = true;
                panel::local::estm_err_lock = true;
                panel::local::sens_err_lock = true;
            }

            try {
                if (channel::local::acces && local::use_ctrl) {
                    channel::local::ctrl = channel.get_ctrl();
                }
            } catch (channel::instance::except_get_ctrl except) {
                switch (except) {
                    case channel::instance::except_get_ctrl::fail:
                        panel::local::ctrl_err_read = true;
                        break;
                    case channel::instance::except_get_ctrl::blank:
                        panel::local::ctrl_err_blank = true;
                        break;
                }
            }

            try {
                if (channel::local::acces && local::use_stat) {
                    channel::local::stat = channel.get_stat();
                }
            } catch (channel::instance::except_get_stat except) {
                switch (except) {
                    case channel::instance::except_get_stat::fail:
                        panel::local::stat_err_read = true;
                        break;
                    case channel::instance::except_get_stat::blank:
                        panel::local::stat_err_blank = true;
                        break;
                }
            }

            try {
                if (channel::local::acces && local::use_estm) {
                    channel::local::estm = channel.get_estm();
                }
            } catch (channel::instance::except_get_estm except) {
                switch (except) {
                    case channel::instance::except_get_estm::fail:
                        panel::local::estm_err_read = true;
                        break;
                    case channel::instance::except_get_estm::blank:
                        panel::local::estm_err_blank = true;
                        break;
                }
            }

            try {
                if (channel::local::acces && local::use_sens) {
                    channel::local::sens = channel.get_sens();
                }
            } catch (channel::instance::except_get_sens except) {
                switch (except) {
                    case channel::instance::except_get_sens::fail:
                        panel::local::sens_err_read = true;
                        break;
                    case channel::instance::except_get_sens::blank:
                        panel::local::sens_err_blank = true;
                        break;
                }
            }

            try {
                if (channel::local::acces) {
                    channel.release();
                    channel::local::acces = false;
                }
            } catch (...) {
                panel::local::ctrl_err_lock = true;
                panel::local::stat_err_lock = true;
                panel::local::estm_err_lock = true;
                panel::local::sens_err_lock = true;
            }

            if (local::use_ctrl) {
                if (panel::local::ctrl_err_lock) {
                    panel_ctrl_body.put(terminal::color::darkred, 3, 2, "Channel lock error");
                } else if (panel::local::ctrl_err_read) {
                    panel_ctrl_body.put(terminal::color::darkred, 3, 2, "Channel read error");
                } else if (panel::local::ctrl_err_blank) {
                    panel_ctrl_body.put(terminal::color::darkred, 3, 2, "Channel blank error");
                } else {
                    panel_ctrl_body.put(terminal::color::black, 2, 39, channel::local::ctrl);
                }
            }

            if (local::use_stat) {
                if (panel::local::stat_err_lock) {
                    panel_stat_body.put(terminal::color::darkred, 9, 2, "Channel lock error");
                } else if (panel::local::stat_err_read) {
                    panel_stat_body.put(terminal::color::darkred, 9, 2, "Channel read error");
                } else if (panel::local::stat_err_blank) {
                    panel_stat_body.put(terminal::color::darkred, 9, 2, "Channel blank error");
                } else {
                    for (int i = 0; i < 7; i++) {
                        panel_stat_body.put(terminal::color::black, 2 + i, 39, channel::local::stat[i]);
                    }
                }
            }

            if (local::use_estm) {
                if (panel::local::estm_err_lock) {
                    panel_estm_body.put(terminal::color::darkred, 6, 2, "Channel lock error");
                } else if (panel::local::estm_err_read) {
                    panel_estm_body.put(terminal::color::darkred, 6, 2, "Channel read error");
                } else if (panel::local::estm_err_blank) {
                    panel_estm_body.put(terminal::color::darkred, 6, 2, "Channel blank error");
                } else {
                    for (int i = 0; i < 4; i++) {
                        panel_estm_body.put(terminal::color::black, 2 + i, 39, channel::local::estm[i]);
                    }
                }
            }

            if (local::use_sens) {
                if (panel::local::sens_err_lock) {
                    panel_sens_body.put(terminal::color::darkred, 19, 2, "Channel lock error");
                } else if (panel::local::sens_err_read) {
                    panel_sens_body.put(terminal::color::darkred, 19, 2, "Channel read error");
                } else if (panel::local::sens_err_blank) {
                    panel_sens_body.put(terminal::color::darkred, 19, 2, "Channel blank error");
                } else {
                    for (int i = 0; i < 17; i++) {
                        panel_sens_body.put(terminal::color::black, 2 + i, 39, channel::local::sens[i]);
                    }
                }
            }
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
