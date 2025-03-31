#include <string>
#include <vector>

#include <math/linalg.hpp>

#include <sys/logging.hpp>
#include <sys/config.hpp>
#include <sys/interrupt.hpp>
#include <sys/timing.hpp>

#include <ui/terminal.hpp>
#include <ui/console.hpp>
#include <ui/panel.hpp>

#include <dev/i2c.hpp>
#include <dev/bno055.hpp>

namespace logging :: local {
    std::string path = "data/system.log";
    std::string unit = "calibrator";
    std::string mod = "main";
}

namespace config :: local {
    std::string path = "config/calibrator.conf";
}

namespace timing :: local {
    std::string name_dur = "sleep_duration";
    double dur;
}

namespace panel :: local {
    std::vector<int> id_lev_head(2), id_lev_body(2);
    std::vector<bool> err_lev_dev(2);

    std::vector<int> id_dat_head(2), id_dat_body(2);
    std::vector<bool> err_dat_dev(2);
}

namespace i2c :: local {
    std::string name_path = "device_path";
    std::string path;
}

namespace bno055 :: local {
    std::vector<int> lev_sys(2), lev_acc(2), lev_mag(2), lev_gyr(2);
    std::vector<int> rad_acc(2), rad_mag(2);
    std::vector<linalg::ivector> off_acc(2), off_mag(2), off_gyr(2);

    std::vector<bool> valid_rad_acc(2), valid_rad_mag(2);
    std::vector<bool> valid_off_acc(2), valid_off_mag(2), valid_off_gyr(2);
}

int main (void) {
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

    i2c::local::path = config::get<std::string>(i2c::local::name_path);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    i2c::init(i2c::local::path);
    if (i2c::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    bno055::init(1, bno055::axis_pz, bno055::axis_px, bno055::axis_py);
    if (bno055::fail()) {
        i2c::deinit();
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    bno055::init(2, bno055::axis_pz, bno055::axis_px, bno055::axis_py);
    if (bno055::fail()) {
        i2c::deinit();
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    panel::init();

    while (!interrupt::caught()) {
        timing::sleep(timing::local::dur);

        panel::local::err_lev_dev[0] = false;
        panel::local::err_dat_dev[0] = false;

        panel::local::err_lev_dev[1] = false;
        panel::local::err_dat_dev[1] = false;

        bno055::update(1);
        if (bno055::fail()) {
            panel::local::err_lev_dev[0] = true;
            panel::local::err_dat_dev[0] = true;
        } else {
            bno055::local::lev_sys[0] = bno055::get_lev_sys(1);
            bno055::local::lev_acc[0] = bno055::get_lev_acc(1);
            bno055::local::lev_mag[0] = bno055::get_lev_mag(1);
            bno055::local::lev_gyr[0] = bno055::get_lev_gyr(1);

            bno055::local::rad_acc[0] = bno055::get_rad_acc(1);
            bno055::local::rad_mag[0] = bno055::get_rad_mag(1);

            bno055::local::off_acc[0] = bno055::get_off_acc(1);
            bno055::local::off_mag[0] = bno055::get_off_mag(1);
            bno055::local::off_gyr[0] = bno055::get_off_gyr(1);

            bno055::local::valid_rad_acc[0] = bno055::valid_rad_acc(1);
            bno055::local::valid_rad_mag[0] = bno055::valid_rad_mag(1);

            bno055::local::valid_off_acc[0] = bno055::valid_off_acc(1);
            bno055::local::valid_off_mag[0] = bno055::valid_off_mag(1);
            bno055::local::valid_off_gyr[0] = bno055::valid_off_gyr(1);
        }

        bno055::update(2);
        if (bno055::fail()) {
            panel::local::err_lev_dev[1] = true;
            panel::local::err_dat_dev[1] = true;
        } else {
            bno055::local::lev_sys[1] = bno055::get_lev_sys(2);
            bno055::local::lev_acc[1] = bno055::get_lev_acc(2);
            bno055::local::lev_mag[1] = bno055::get_lev_mag(2);
            bno055::local::lev_gyr[1] = bno055::get_lev_gyr(2);

            bno055::local::rad_acc[1] = bno055::get_rad_acc(2);
            bno055::local::rad_mag[1] = bno055::get_rad_mag(2);

            bno055::local::off_acc[1] = bno055::get_off_acc(2);
            bno055::local::off_mag[1] = bno055::get_off_mag(2);
            bno055::local::off_gyr[1] = bno055::get_off_gyr(2);

            bno055::local::valid_rad_acc[1] = bno055::valid_rad_acc(2);
            bno055::local::valid_rad_mag[1] = bno055::valid_rad_mag(2);

            bno055::local::valid_off_acc[1] = bno055::valid_off_acc(2);
            bno055::local::valid_off_mag[1] = bno055::valid_off_mag(2);
            bno055::local::valid_off_gyr[1] = bno055::valid_off_gyr(2);
        }

        panel::close();

        if (panel::local::err_lev_dev[0]) {
            panel::local::id_lev_head[0] = panel::open(terminal::darkred, 2, 3, 1, 50);
            panel::local::id_lev_body[0] = panel::open(terminal::lightgray, 3, 3, 6, 50);
        } else {
            panel::local::id_lev_head[0] = panel::open(terminal::darkblue, 2, 3, 1, 50);
            panel::local::id_lev_body[0] = panel::open(terminal::lightgray, 3, 3, 6, 50);
        }

        panel::put(terminal::white, panel::local::id_lev_head[0], 1, 2, "Sensor #1 calibration levels");
        panel::put(terminal::black, panel::local::id_lev_body[0], 2, 2, "System level");
        panel::put(terminal::black, panel::local::id_lev_body[0], 3, 2, "Accelerometer level");
        panel::put(terminal::black, panel::local::id_lev_body[0], 4, 2, "Magnetometer level");
        panel::put(terminal::black, panel::local::id_lev_body[0], 5, 2, "Gyroscope level");

        if (panel::local::err_lev_dev[0]) {
            panel::put(terminal::darkred, panel::local::id_lev_body[0], 6, 2, "Device error");
        } else {
            if (bno055::local::lev_sys[0] < 3) {
                panel::put(terminal::darkred, panel::local::id_lev_body[0], 2, 26, bno055::local::lev_sys[0]);
            } else {
                panel::put(terminal::darkgreen, panel::local::id_lev_body[0], 2, 26, bno055::local::lev_sys[0]);
            }
            if (bno055::local::lev_acc[0] < 3) {
                panel::put(terminal::darkred, panel::local::id_lev_body[0], 3, 26, bno055::local::lev_acc[0]);
            } else {
                panel::put(terminal::darkgreen, panel::local::id_lev_body[0], 3, 26, bno055::local::lev_acc[0]);
            }
            if (bno055::local::lev_mag[0] < 3) {
                panel::put(terminal::darkred, panel::local::id_lev_body[0], 4, 26, bno055::local::lev_mag[0]);
            } else {
                panel::put(terminal::darkgreen, panel::local::id_lev_body[0], 4, 26, bno055::local::lev_mag[0]);
            }
            if (bno055::local::lev_gyr[0] < 3) {
                panel::put(terminal::darkred, panel::local::id_lev_body[0], 5, 26, bno055::local::lev_gyr[0]);
            } else {
                panel::put(terminal::darkgreen, panel::local::id_lev_body[0], 5, 26, bno055::local::lev_gyr[0]);
            }
        }

        if (panel::local::err_dat_dev[0]) {
            panel::local::id_dat_head[0] = panel::open(terminal::darkred, 10, 3, 1, 50);
            panel::local::id_dat_body[0] = panel::open(terminal::lightgray, 11, 3, 7, 50);
        } else {
            panel::local::id_dat_head[0] = panel::open(terminal::darkblue, 10, 3, 1, 50);
            panel::local::id_dat_body[0] = panel::open(terminal::lightgray, 11, 3, 7, 50);
        }

        panel::put(terminal::white, panel::local::id_dat_head[0], 1, 2, "Sensor #1 calibration data");
        panel::put(terminal::black, panel::local::id_dat_body[0], 2, 2, "Accelerometer radius");
        panel::put(terminal::black, panel::local::id_dat_body[0], 3, 2, "Magnetometer radius");
        panel::put(terminal::black, panel::local::id_dat_body[0], 4, 2, "Accelerometer offset");
        panel::put(terminal::black, panel::local::id_dat_body[0], 5, 2, "Magnetometer offset");
        panel::put(terminal::black, panel::local::id_dat_body[0], 6, 2, "Gyroscope offset");

        if (panel::local::err_dat_dev[0]) {
            panel::put(terminal::darkred, panel::local::id_dat_body[0], 7, 2, "Device error");
        } else {
            if (!bno055::local::valid_rad_acc[0]) {
                panel::put(terminal::darkred, panel::local::id_dat_body[0], 2, 26, bno055::local::rad_acc[0]);
            } else {
                panel::put(terminal::darkgreen, panel::local::id_dat_body[0], 2, 26, bno055::local::rad_acc[0]);
            }
            if (!bno055::local::valid_rad_mag[0]) {
                panel::put(terminal::darkred, panel::local::id_dat_body[0], 3, 26, bno055::local::rad_mag[0]);
            } else {
                panel::put(terminal::darkgreen, panel::local::id_dat_body[0], 3, 26, bno055::local::rad_mag[0]);
            }
            if (!bno055::local::valid_off_acc[0]) {
                panel::put(terminal::darkred, panel::local::id_dat_body[0], 4, 26, bno055::local::off_acc[0]);
            } else {
                panel::put(terminal::darkgreen, panel::local::id_dat_body[0], 4, 26, bno055::local::off_acc[0]);
            }
            if (!bno055::local::valid_off_mag[0]) {
                panel::put(terminal::darkred, panel::local::id_dat_body[0], 5, 26, bno055::local::off_mag[0]);
            } else {
                panel::put(terminal::darkgreen, panel::local::id_dat_body[0], 5, 26, bno055::local::off_mag[0]);
            }
            if (!bno055::local::valid_off_gyr[0]) {
                panel::put(terminal::darkred, panel::local::id_dat_body[0], 6, 26, bno055::local::off_gyr[0]);
            } else {
                panel::put(terminal::darkgreen, panel::local::id_dat_body[0], 6, 26, bno055::local::off_gyr[0]);
            }
        }

        if (panel::local::err_lev_dev[1]) {
            panel::local::id_lev_head[1] = panel::open(terminal::darkred, 2, 55, 1, 50);
            panel::local::id_lev_body[1] = panel::open(terminal::lightgray, 3, 55, 6, 50);
        } else {
            panel::local::id_lev_head[1] = panel::open(terminal::darkblue, 2, 55, 1, 50);
            panel::local::id_lev_body[1] = panel::open(terminal::lightgray, 3, 55, 6, 50);
        }

        panel::put(terminal::white, panel::local::id_lev_head[1], 1, 2, "Sensor #2 calibration levels");
        panel::put(terminal::black, panel::local::id_lev_body[1], 2, 2, "System level");
        panel::put(terminal::black, panel::local::id_lev_body[1], 3, 2, "Accelerometer level");
        panel::put(terminal::black, panel::local::id_lev_body[1], 4, 2, "Magnetometer level");
        panel::put(terminal::black, panel::local::id_lev_body[1], 5, 2, "Gyroscope level");

        if (panel::local::err_lev_dev[1]) {
            panel::put(terminal::darkred, panel::local::id_lev_body[1], 6, 2, "Device error");
        } else {
            if (bno055::local::lev_sys[1] < 3) {
                panel::put(terminal::darkred, panel::local::id_lev_body[1], 2, 26, bno055::local::lev_sys[1]);
            } else {
                panel::put(terminal::darkgreen, panel::local::id_lev_body[1], 2, 26, bno055::local::lev_sys[1]);
            }
            if (bno055::local::lev_acc[1] < 3) {
                panel::put(terminal::darkred, panel::local::id_lev_body[1], 3, 26, bno055::local::lev_acc[1]);
            } else {
                panel::put(terminal::darkgreen, panel::local::id_lev_body[1], 3, 26, bno055::local::lev_acc[1]);
            }
            if (bno055::local::lev_mag[1] < 3) {
                panel::put(terminal::darkred, panel::local::id_lev_body[1], 4, 26, bno055::local::lev_mag[1]);
            } else {
                panel::put(terminal::darkgreen, panel::local::id_lev_body[1], 4, 26, bno055::local::lev_mag[1]);
            }
            if (bno055::local::lev_gyr[1] < 3) {
                panel::put(terminal::darkred, panel::local::id_lev_body[1], 5, 26, bno055::local::lev_gyr[1]);
            } else {
                panel::put(terminal::darkgreen, panel::local::id_lev_body[1], 5, 26, bno055::local::lev_gyr[1]);
            }
        }

        if (panel::local::err_dat_dev[1]) {
            panel::local::id_dat_head[1] = panel::open(terminal::darkred, 10, 55, 1, 50);
            panel::local::id_dat_body[1] = panel::open(terminal::lightgray, 11, 55, 7, 50);
        } else {
            panel::local::id_dat_head[1] = panel::open(terminal::darkblue, 10, 55, 1, 50);
            panel::local::id_dat_body[1] = panel::open(terminal::lightgray, 11, 55, 7, 50);
        }

        panel::put(terminal::white, panel::local::id_dat_head[1], 1, 2, "Sensor #2 calibration data");
        panel::put(terminal::black, panel::local::id_dat_body[1], 2, 2, "Accelerometer radius");
        panel::put(terminal::black, panel::local::id_dat_body[1], 3, 2, "Magnetometer radius");
        panel::put(terminal::black, panel::local::id_dat_body[1], 4, 2, "Accelerometer offset");
        panel::put(terminal::black, panel::local::id_dat_body[1], 5, 2, "Magnetometer offset");
        panel::put(terminal::black, panel::local::id_dat_body[1], 6, 2, "Gyroscope offset");

        if (panel::local::err_dat_dev[1]) {
            panel::put(terminal::darkred, panel::local::id_dat_body[1], 7, 2, "Device error");
        } else {
            if (!bno055::local::valid_rad_acc[1]) {
                panel::put(terminal::darkred, panel::local::id_dat_body[1], 2, 26, bno055::local::rad_acc[1]);
            } else {
                panel::put(terminal::darkgreen, panel::local::id_dat_body[1], 2, 26, bno055::local::rad_acc[1]);
            }
            if (!bno055::local::valid_rad_mag[1]) {
                panel::put(terminal::darkred, panel::local::id_dat_body[1], 3, 26, bno055::local::rad_mag[1]);
            } else {
                panel::put(terminal::darkgreen, panel::local::id_dat_body[1], 3, 26, bno055::local::rad_mag[1]);
            }
            if (!bno055::local::valid_off_acc[1]) {
                panel::put(terminal::darkred, panel::local::id_dat_body[1], 4, 26, bno055::local::off_acc[1]);
            } else {
                panel::put(terminal::darkgreen, panel::local::id_dat_body[1], 4, 26, bno055::local::off_acc[1]);
            }
            if (!bno055::local::valid_off_mag[1]) {
                panel::put(terminal::darkred, panel::local::id_dat_body[1], 5, 26, bno055::local::off_mag[1]);
            } else {
                panel::put(terminal::darkgreen, panel::local::id_dat_body[1], 5, 26, bno055::local::off_mag[1]);
            }
            if (!bno055::local::valid_off_gyr[1]) {
                panel::put(terminal::darkred, panel::local::id_dat_body[1], 6, 26, bno055::local::off_gyr[1]);
            } else {
                panel::put(terminal::darkgreen, panel::local::id_dat_body[1], 6, 26, bno055::local::off_gyr[1]);
            }
        }
    }

    panel::deinit();

    console::put(terminal::darkyellow, 1, "Calibration radii");
    console::newline();

    console::put(terminal::lightgray, 5, "Accelerometer");
    console::put(terminal::lightgray, 25, bno055::local::rad_acc);
    console::newline();

    console::put(terminal::lightgray, 5, "Magnetometer");
    console::put(terminal::lightgray, 25, bno055::local::rad_mag);
    console::newline();

    console::put(terminal::darkyellow, 1, "Calibration offsets");
    console::newline();

    console::put(terminal::lightgray, 5, "Accelerometer");
    console::put(terminal::lightgray, 25, bno055::local::off_acc);
    console::newline();

    console::put(terminal::lightgray, 5, "Magnetometer");
    console::put(terminal::lightgray, 25, bno055::local::off_mag);
    console::newline();

    console::put(terminal::lightgray, 5, "Gyroscope");
    console::put(terminal::lightgray, 25, bno055::local::off_gyr);
    console::newline();

    i2c::deinit();
    if (i2c::fail()) {
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
