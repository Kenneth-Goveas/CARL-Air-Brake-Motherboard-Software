#include <string>
#include <vector>

#include <math/linalg.hpp>

#include <sys/logging.hpp>
#include <sys/config.hpp>
#include <sys/interrupt.hpp>
#include <sys/timing.hpp>

#include <ipc/channel.hpp>

#include <dev/i2c.hpp>
#include <dev/bmp388.hpp>
#include <dev/bno055.hpp>

namespace logging :: local {
    std::string path = "data/system.log";
    std::string unit = "instrument";
    std::string mod = "main";
}

namespace config :: local {
    std::string path = "config/instrument.conf";
}

namespace timing :: local {
    std::string name_dur = "sleep_duration";
    double time, dur;
}

namespace channel :: local {
    bool want_ctrl = false, want_stat = false, want_estm = false, want_sens = true;
    bool acces;
    linalg::fvector sens(17);
}

namespace i2c :: local {
    std::string name_path = "device_path";
    std::string path;
}

namespace bmp388 :: local {
    std::vector<double> pres(2), temp(2);
    std::vector<bool> valid(2);

    double pres_mean, temp_mean;
    bool valid_mean;
}

namespace bno055 :: local {
    std::string name_rad_acc = "accelerometer_radii", name_rad_mag = "magnetometer_radii";
    std::vector<int> rad_acc, rad_mag;

    std::string name_off_acc = "accelerometer_offsets", name_off_mag = "magnetometer_offsets", name_off_gyr = "gyroscope_offsets";
    std::vector<linalg::ivector> off_acc, off_mag, off_gyr;

    std::vector<linalg::fvector> rot(2), qua(2), lia(2);
    std::vector<bool> valid(2);

    linalg::fvector rot_mean, qua_mean, lia_mean;
    bool valid_mean;
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

    bno055::local::rad_acc = config::get<std::vector<int>>(bno055::local::name_rad_acc);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    bno055::local::rad_mag = config::get<std::vector<int>>(bno055::local::name_rad_mag);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    bno055::local::off_acc = config::get<std::vector<linalg::ivector>>(bno055::local::name_off_acc);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    bno055::local::off_mag = config::get<std::vector<linalg::ivector>>(bno055::local::name_off_mag);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    bno055::local::off_gyr = config::get<std::vector<linalg::ivector>>(bno055::local::name_off_gyr);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    if (bno055::local::rad_acc.size() != 2) {
        logging::err(logging::local::mod,
            "Exactly two accelerometer radii must be specified"
        );
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    if (bno055::local::rad_mag.size() != 2) {
        logging::err(logging::local::mod,
            "Exactly two magnetometer radii must be specified"
        );
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    if (bno055::local::off_acc.size() != 2) {
        logging::err(logging::local::mod,
            "Exactly two accelerometer offsets must be specified"
        );
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    if (bno055::local::off_mag.size() != 2) {
        logging::err(logging::local::mod,
            "Exactly two magnetometer offsets must be specified"
        );
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    if (bno055::local::off_gyr.size() != 2) {
        logging::err(logging::local::mod,
            "Exactly two gyroscope offsets must be specified"
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

    i2c::init(i2c::local::path);
    if (i2c::fail()) {
        channel::close();
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    bmp388::init(1, bmp388::drate_100_hz, bmp388::ovsmp_4x, bmp388::ovsmp_1x, bmp388::iir_3);
    if (bmp388::fail()) {
        i2c::deinit();
        channel::close();
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    bmp388::init(2, bmp388::drate_100_hz, bmp388::ovsmp_4x, bmp388::ovsmp_1x, bmp388::iir_3);
    if (bmp388::fail()) {
        i2c::deinit();
        channel::close();
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    bno055::init(
        1, bno055::axis_pz, bno055::axis_px, bno055::axis_py,
        bno055::local::rad_acc[0], bno055::local::rad_mag[0],
        bno055::local::off_acc[0], bno055::local::off_mag[0], bno055::local::off_gyr[0]
    );
    if (bno055::fail()) {
        i2c::deinit();
        channel::close();
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    bno055::init(
        2, bno055::axis_pz, bno055::axis_px, bno055::axis_py,
        bno055::local::rad_acc[1], bno055::local::rad_mag[1],
        bno055::local::off_acc[1], bno055::local::off_mag[1], bno055::local::off_gyr[1]
    );
    if (bno055::fail()) {
        i2c::deinit();
        channel::close();
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    channel::local::acces = false;

    bmp388::local::valid_mean = false;
    bno055::local::valid_mean = false;

    while (!interrupt::caught()) {
        timing::sleep(timing::local::dur);

        timing::local::time = timing::get_time();

        bmp388::update(1);
        if (bmp388::fail() || bmp388::incomp()) {
            bmp388::local::valid[0] = false;
        } else {
            bmp388::local::valid[0] = true;

            bmp388::local::pres[0] = bmp388::get_pres(1);
            bmp388::local::temp[0] = bmp388::get_temp(1);
        }

        bmp388::update(2);
        if (bmp388::fail() || bmp388::incomp()) {
            bmp388::local::valid[1] = false;
        } else {
            bmp388::local::valid[1] = true;

            bmp388::local::pres[1] = bmp388::get_pres(2);
            bmp388::local::temp[1] = bmp388::get_temp(2);
        }

        bno055::update(1);
        if (bno055::fail()) {
            bno055::local::valid[0] = false;
        } else {
            bno055::local::valid[0] = true;

            bno055::local::rot[0] = bno055::get_rot(1);
            bno055::local::qua[0] = bno055::get_qua(1);
            bno055::local::lia[0] = bno055::get_lia(1);
        }

        bno055::update(2);
        if (bno055::fail()) {
            bno055::local::valid[1] = false;
        } else {
            bno055::local::valid[1] = true;

            bno055::local::rot[1] = bno055::get_rot(2);
            bno055::local::qua[1] = bno055::get_qua(2);
            bno055::local::lia[1] = bno055::get_lia(2);
        }

        if (bmp388::local::valid[0] && bmp388::local::valid[1]) {
            bmp388::local::pres_mean = 0.5 * (bmp388::local::pres[0] + bmp388::local::pres[1]);
            bmp388::local::temp_mean = 0.5 * (bmp388::local::temp[0] + bmp388::local::temp[1]);
        } else if (bmp388::local::valid[0]) {
            bmp388::local::pres_mean = bmp388::local::pres[0];
            bmp388::local::temp_mean = bmp388::local::temp[0];
        } else if (bmp388::local::valid[1]) {
            bmp388::local::pres_mean = bmp388::local::pres[1];
            bmp388::local::temp_mean = bmp388::local::temp[1];
        }

        if (bno055::local::valid[0] && bno055::local::valid[1]) {
            bno055::local::rot_mean = 0.5 * (bno055::local::rot[0] + bno055::local::rot[1]);
            bno055::local::qua_mean = 0.5 * (bno055::local::qua[0] + bno055::local::qua[1]);
            bno055::local::lia_mean = 0.5 * (bno055::local::lia[0] + bno055::local::lia[1]);
            bno055::local::qua_mean /= linalg::norm(bno055::local::qua_mean);
        } else if (bno055::local::valid[0]) {
            bno055::local::rot_mean = bno055::local::rot[0];
            bno055::local::qua_mean = bno055::local::qua[0];
            bno055::local::lia_mean = bno055::local::lia[0];
        } else if (bno055::local::valid[1]) {
            bno055::local::rot_mean = bno055::local::rot[1];
            bno055::local::qua_mean = bno055::local::qua[1];
            bno055::local::lia_mean = bno055::local::lia[1];
        }

        if (bmp388::local::valid[0] || bmp388::local::valid[1]) {
            bmp388::local::valid_mean = true;
            channel::local::sens[10] = bmp388::local::pres_mean;
            channel::local::sens[11] = bmp388::local::temp_mean;
            channel::local::sens[15] = timing::local::time;
            channel::local::sens[16] = timing::local::time;
        }

        if (bno055::local::valid[0] || bno055::local::valid[1]) {
            bno055::local::valid_mean = true;
            channel::local::sens[0] = bno055::local::rot_mean[0];
            channel::local::sens[1] = bno055::local::rot_mean[1];
            channel::local::sens[2] = bno055::local::rot_mean[2];
            channel::local::sens[3] = bno055::local::qua_mean[0];
            channel::local::sens[4] = bno055::local::qua_mean[1];
            channel::local::sens[5] = bno055::local::qua_mean[2];
            channel::local::sens[6] = bno055::local::qua_mean[3];
            channel::local::sens[7] = bno055::local::lia_mean[0];
            channel::local::sens[8] = bno055::local::lia_mean[1];
            channel::local::sens[9] = bno055::local::lia_mean[2];
            channel::local::sens[12] = timing::local::time;
            channel::local::sens[13] = timing::local::time;
            channel::local::sens[14] = timing::local::time;
        }

        if (!(bmp388::local::valid_mean && bno055::local::valid_mean)) {
            continue;
        }

        if (!channel::local::acces) {
            channel::acquire_acces();
            if (!channel::fail()) {
                channel::local::acces = true;
            }
        }

        if (channel::local::acces) {
            channel::put_sens(channel::local::sens);
        }

        if (channel::local::acces) {
            channel::release_acces();
            if (!channel::fail()) {
                channel::local::acces = false;
            }
        }
    }

    i2c::deinit();
    if (i2c::fail()) {
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
