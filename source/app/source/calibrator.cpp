#include <string>

#include <math/linalg.hpp>

#include <sys/logging.hpp>
#include <sys/interrupt.hpp>
#include <sys/timing.hpp>
#include <sys/config.hpp>

#include <ui/terminal.hpp>
#include <ui/input.hpp>
#include <ui/output.hpp>
#include <ui/screen.hpp>
#include <ui/panel.hpp>

#include <dat/txtwrite.hpp>

#include <dev/i2c.hpp>
#include <dev/bno055.hpp>

namespace logging::local {
    std::string path = "log/system.log";
    std::string app = "calibrator";
    std::string lib = "main";
}

namespace timing::local {
    std::string dur_name = "sleep_duration";
    double dur;
}

namespace config::local {
    std::string path = "config/calibrator.conf";
}

namespace panel::local {
    bool lev1_err_dev, lev2_err_dev, dat1_err_dev, dat2_err_dev;
}

namespace txtwrite::local {
    std::string path = "output/calibration-data.csv";
}

namespace i2c::local {
    std::string path_name = "device_path";
    std::string path;
}

namespace bno055::local {
    int lev_sys1, lev_acc1, lev_mag1, lev_gyr1;
    int lev_sys2, lev_acc2, lev_mag2, lev_gyr2;

    int rad_acc1, rad_mag1;
    int rad_acc2, rad_mag2;

    linalg::ivector off_acc1, off_mag1, off_gyr1;
    linalg::ivector off_acc2, off_mag2, off_gyr2;

    bool valid_rad_acc1, valid_rad_mag1;
    bool valid_rad_acc2, valid_rad_mag2;

    bool valid_off_acc1, valid_off_mag1, valid_off_gyr1;
    bool valid_off_acc2, valid_off_mag2, valid_off_gyr2;
}

int main (void) {
    if (!logging::instance::start(logging::local::path, logging::local::app)) {
        return 1;
    }

    logging::instance logging(logging::local::lib);
    logging.inf("Starting");

    try {
        config::instance config(config::local::path);

        timing::local::dur = config.get<double>(timing::local::dur_name);

        i2c::local::path = config.get<std::string>(i2c::local::path_name);

        interrupt::instance interrupt;
        timing::instance timing;

        terminal::instance terminal;
        input::instance input;
        output::instance output;
        screen::instance screen(terminal);
        panel::instance panel_lev1_head(terminal, input, output, terminal::color::darkblue, 2, 3, 1, 50),
                        panel_lev1_body(terminal, input, output, terminal::color::lightgray, 3, 3, 6, 50),
                        panel_lev2_head(terminal, input, output, terminal::color::darkblue, 2, 55, 1, 50),
                        panel_lev2_body(terminal, input, output, terminal::color::lightgray, 3, 55, 6, 50),
                        panel_dat1_head(terminal, input, output, terminal::color::darkblue, 10, 3, 1, 50),
                        panel_dat1_body(terminal, input, output, terminal::color::lightgray, 11, 3, 7, 50),
                        panel_dat2_head(terminal, input, output, terminal::color::darkblue, 10, 55, 1, 50),
                        panel_dat2_body(terminal, input, output, terminal::color::lightgray, 11, 55, 7, 50);

        txtwrite::instance txtwrite(txtwrite::local::path);

        i2c::instance i2c(i2c::local::path);
        bno055::instance bno055_1(i2c, 1, bno055::axis::pz, bno055::axis::px, bno055::axis::py),
                         bno055_2(i2c, 2, bno055::axis::pz, bno055::axis::px, bno055::axis::py);

        while (!interrupt.caught()) {
            timing.sleep(timing::local::dur);

            panel::local::lev1_err_dev = false;
            panel::local::lev2_err_dev = false;
            panel::local::dat1_err_dev = false;
            panel::local::dat2_err_dev = false;

            try {
                bno055_1.update();

                bno055::local::lev_sys1 = bno055_1.get_lev_sys();
                bno055::local::lev_acc1 = bno055_1.get_lev_acc();
                bno055::local::lev_mag1 = bno055_1.get_lev_mag();
                bno055::local::lev_gyr1 = bno055_1.get_lev_gyr();

                bno055::local::rad_acc1 = bno055_1.get_rad_acc();
                bno055::local::rad_mag1 = bno055_1.get_rad_mag();

                bno055::local::off_acc1 = bno055_1.get_off_acc();
                bno055::local::off_mag1 = bno055_1.get_off_mag();
                bno055::local::off_gyr1 = bno055_1.get_off_gyr();

                bno055::local::valid_rad_acc1 = bno055_1.valid_rad_acc();
                bno055::local::valid_rad_mag1 = bno055_1.valid_rad_mag();

                bno055::local::valid_off_acc1 = bno055_1.valid_off_acc();
                bno055::local::valid_off_mag1 = bno055_1.valid_off_mag();
                bno055::local::valid_off_gyr1 = bno055_1.valid_off_gyr();
            } catch (...) {
                panel::local::lev1_err_dev = true;
                panel::local::dat1_err_dev = true;
            }

            try {
                bno055_2.update();

                bno055::local::lev_sys2 = bno055_2.get_lev_sys();
                bno055::local::lev_acc2 = bno055_2.get_lev_acc();
                bno055::local::lev_mag2 = bno055_2.get_lev_mag();
                bno055::local::lev_gyr2 = bno055_2.get_lev_gyr();

                bno055::local::rad_acc2 = bno055_2.get_rad_acc();
                bno055::local::rad_mag2 = bno055_2.get_rad_mag();

                bno055::local::off_acc2 = bno055_2.get_off_acc();
                bno055::local::off_mag2 = bno055_2.get_off_mag();
                bno055::local::off_gyr2 = bno055_2.get_off_gyr();

                bno055::local::valid_rad_acc2 = bno055_2.valid_rad_acc();
                bno055::local::valid_rad_mag2 = bno055_2.valid_rad_mag();

                bno055::local::valid_off_acc2 = bno055_2.valid_off_acc();
                bno055::local::valid_off_mag2 = bno055_2.valid_off_mag();
                bno055::local::valid_off_gyr2 = bno055_2.valid_off_gyr();
            } catch (...) {
                panel::local::lev2_err_dev = true;
                panel::local::dat2_err_dev = true;
            }

            panel_lev1_head.redraw();
            panel_lev1_body.redraw();
            panel_lev1_head.put(terminal::color::white, 1, 2, "Sensor #1 calibration levels");
            panel_lev1_body.put(terminal::color::black, 2, 2, "System level");
            panel_lev1_body.put(terminal::color::black, 3, 2, "Accelerometer level");
            panel_lev1_body.put(terminal::color::black, 4, 2, "Magnetometer level");
            panel_lev1_body.put(terminal::color::black, 5, 2, "Gyroscope level");

            panel_lev2_head.redraw();
            panel_lev2_body.redraw();
            panel_lev2_head.put(terminal::color::white, 1, 2, "Sensor #2 calibration levels");
            panel_lev2_body.put(terminal::color::black, 2, 2, "System level");
            panel_lev2_body.put(terminal::color::black, 3, 2, "Accelerometer level");
            panel_lev2_body.put(terminal::color::black, 4, 2, "Magnetometer level");
            panel_lev2_body.put(terminal::color::black, 5, 2, "Gyroscope level");

            panel_dat1_head.redraw();
            panel_dat1_body.redraw();
            panel_dat1_head.put(terminal::color::white, 1, 2, "Sensor #1 calibration data");
            panel_dat1_body.put(terminal::color::black, 2, 2, "Accelerometer radius");
            panel_dat1_body.put(terminal::color::black, 3, 2, "Magnetometer radius");
            panel_dat1_body.put(terminal::color::black, 4, 2, "Accelerometer offset");
            panel_dat1_body.put(terminal::color::black, 5, 2, "Magnetometer offset");
            panel_dat1_body.put(terminal::color::black, 6, 2, "Gyroscope offset");

            panel_dat2_head.redraw();
            panel_dat2_body.redraw();
            panel_dat2_head.put(terminal::color::white, 1, 2, "Sensor #2 calibration data");
            panel_dat2_body.put(terminal::color::black, 2, 2, "Accelerometer radius");
            panel_dat2_body.put(terminal::color::black, 3, 2, "Magnetometer radius");
            panel_dat2_body.put(terminal::color::black, 4, 2, "Accelerometer offset");
            panel_dat2_body.put(terminal::color::black, 5, 2, "Magnetometer offset");
            panel_dat2_body.put(terminal::color::black, 6, 2, "Gyroscope offset");

            if (panel::local::lev1_err_dev) {
                panel_lev1_body.put(terminal::color::darkred, 6, 2, "Device error");
            } else {
                if (bno055::local::lev_sys1 < 3) {
                    panel_lev1_body.put(terminal::color::darkred, 2, 26, bno055::local::lev_sys1);
                } else {
                    panel_lev1_body.put(terminal::color::darkgreen, 2, 26, bno055::local::lev_sys1);
                }
                if (bno055::local::lev_acc1 < 3) {
                    panel_lev1_body.put(terminal::color::darkred, 3, 26, bno055::local::lev_acc1);
                } else {
                    panel_lev1_body.put(terminal::color::darkgreen, 3, 26, bno055::local::lev_acc1);
                }
                if (bno055::local::lev_mag1 < 3) {
                    panel_lev1_body.put(terminal::color::darkred, 4, 26, bno055::local::lev_mag1);
                } else {
                    panel_lev1_body.put(terminal::color::darkgreen, 4, 26, bno055::local::lev_mag1);
                }
                if (bno055::local::lev_gyr1 < 3) {
                    panel_lev1_body.put(terminal::color::darkred, 5, 26, bno055::local::lev_gyr1);
                } else {
                    panel_lev1_body.put(terminal::color::darkgreen, 5, 26, bno055::local::lev_gyr1);
                }
            }

            if (panel::local::lev2_err_dev) {
                panel_lev2_body.put(terminal::color::darkred, 6, 2, "Device error");
            } else {
                if (bno055::local::lev_sys2 < 3) {
                    panel_lev2_body.put(terminal::color::darkred, 2, 26, bno055::local::lev_sys2);
                } else {
                    panel_lev2_body.put(terminal::color::darkgreen, 2, 26, bno055::local::lev_sys2);
                }
                if (bno055::local::lev_acc2 < 3) {
                    panel_lev2_body.put(terminal::color::darkred, 3, 26, bno055::local::lev_acc2);
                } else {
                    panel_lev2_body.put(terminal::color::darkgreen, 3, 26, bno055::local::lev_acc2);
                }
                if (bno055::local::lev_mag2 < 3) {
                    panel_lev2_body.put(terminal::color::darkred, 4, 26, bno055::local::lev_mag2);
                } else {
                    panel_lev2_body.put(terminal::color::darkgreen, 4, 26, bno055::local::lev_mag2);
                }
                if (bno055::local::lev_gyr2 < 3) {
                    panel_lev2_body.put(terminal::color::darkred, 5, 26, bno055::local::lev_gyr2);
                } else {
                    panel_lev2_body.put(terminal::color::darkgreen, 5, 26, bno055::local::lev_gyr2);
                }
            }

            if (panel::local::dat1_err_dev) {
                panel_dat1_body.put(terminal::color::darkred, 7, 2, "Device error");
            } else {
                if (!bno055::local::valid_rad_acc1) {
                    panel_dat1_body.put(terminal::color::darkred, 2, 26, bno055::local::rad_acc1);
                } else {
                    panel_dat1_body.put(terminal::color::darkgreen, 2, 26, bno055::local::rad_acc1);
                }
                if (!bno055::local::valid_rad_mag1) {
                    panel_dat1_body.put(terminal::color::darkred, 3, 26, bno055::local::rad_mag1);
                } else {
                    panel_dat1_body.put(terminal::color::darkgreen, 3, 26, bno055::local::rad_mag1);
                }
                if (!bno055::local::valid_off_acc1) {
                    panel_dat1_body.put(terminal::color::darkred, 4, 26, bno055::local::off_acc1);
                } else {
                    panel_dat1_body.put(terminal::color::darkgreen, 4, 26, bno055::local::off_acc1);
                }
                if (!bno055::local::valid_off_mag1) {
                    panel_dat1_body.put(terminal::color::darkred, 5, 26, bno055::local::off_mag1);
                } else {
                    panel_dat1_body.put(terminal::color::darkgreen, 5, 26, bno055::local::off_mag1);
                }
                if (!bno055::local::valid_off_gyr1) {
                    panel_dat1_body.put(terminal::color::darkred, 6, 26, bno055::local::off_gyr1);
                } else {
                    panel_dat1_body.put(terminal::color::darkgreen, 6, 26, bno055::local::off_gyr1);
                }
            }

            if (panel::local::dat2_err_dev) {
                panel_dat2_body.put(terminal::color::darkred, 7, 2, "Device error");
            } else {
                if (!bno055::local::valid_rad_acc2) {
                    panel_dat2_body.put(terminal::color::darkred, 2, 26, bno055::local::rad_acc2);
                } else {
                    panel_dat2_body.put(terminal::color::darkgreen, 2, 26, bno055::local::rad_acc2);
                }
                if (!bno055::local::valid_rad_mag2) {
                    panel_dat2_body.put(terminal::color::darkred, 3, 26, bno055::local::rad_mag2);
                } else {
                    panel_dat2_body.put(terminal::color::darkgreen, 3, 26, bno055::local::rad_mag2);
                }
                if (!bno055::local::valid_off_acc2) {
                    panel_dat2_body.put(terminal::color::darkred, 4, 26, bno055::local::off_acc2);
                } else {
                    panel_dat2_body.put(terminal::color::darkgreen, 4, 26, bno055::local::off_acc2);
                }
                if (!bno055::local::valid_off_mag2) {
                    panel_dat2_body.put(terminal::color::darkred, 5, 26, bno055::local::off_mag2);
                } else {
                    panel_dat2_body.put(terminal::color::darkgreen, 5, 26, bno055::local::off_mag2);
                }
                if (!bno055::local::valid_off_gyr2) {
                    panel_dat2_body.put(terminal::color::darkred, 6, 26, bno055::local::off_gyr2);
                } else {
                    panel_dat2_body.put(terminal::color::darkgreen, 6, 26, bno055::local::off_gyr2);
                }
            }
        }

        txtwrite.put("Sensor #1 calibration data:");
        txtwrite.newline();

        txtwrite.put("  Accelerometer radius: ");
        txtwrite.put(bno055::local::rad_acc1);
        txtwrite.newline();

        txtwrite.put("  Magnetometer radius:  ");
        txtwrite.put(bno055::local::rad_mag1);
        txtwrite.newline();

        txtwrite.put("  Accelerometer offset: ");
        txtwrite.put(bno055::local::off_acc1);
        txtwrite.newline();

        txtwrite.put("  Magnetometer offset:  ");
        txtwrite.put(bno055::local::off_mag1);
        txtwrite.newline();

        txtwrite.put("  Gyroscope offset:     ");
        txtwrite.put(bno055::local::off_gyr1);
        txtwrite.newline();

        txtwrite.newline();

        txtwrite.put("Sensor #2 calibration data:");
        txtwrite.newline();

        txtwrite.put("  Accelerometer radius: ");
        txtwrite.put(bno055::local::rad_acc2);
        txtwrite.newline();

        txtwrite.put("  Magnetometer radius:  ");
        txtwrite.put(bno055::local::rad_mag2);
        txtwrite.newline();

        txtwrite.put("  Accelerometer offset: ");
        txtwrite.put(bno055::local::off_acc2);
        txtwrite.newline();

        txtwrite.put("  Magnetometer offset:  ");
        txtwrite.put(bno055::local::off_mag2);
        txtwrite.newline();

        txtwrite.put("  Gyroscope offset:     ");
        txtwrite.put(bno055::local::off_gyr2);
        txtwrite.newline();
    } catch (...) {
        logging.wrn("Exiting");
        logging::instance::stop();
        return 1;
    }

    logging.inf("Exiting");
    logging::instance::stop();

    return 0;
}
