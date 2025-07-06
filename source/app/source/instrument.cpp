#include <string>
#include <vector>

#include <math/linalg.hpp>

#include <sys/logging.hpp>
#include <sys/interrupt.hpp>
#include <sys/timing.hpp>
#include <sys/config.hpp>

#include <ipc/channel.hpp>

#include <dev/i2c.hpp>
#include <dev/bmp388.hpp>
#include <dev/bno055.hpp>

namespace logging::local {
    std::string path = "log/system.log";
    std::string app = "instrument";
    std::string lib = "main";
}

namespace timing::local {
    std::string dur_name = "sleep_duration";
    double time, dur;
}

namespace config::local {
    std::string path = "config/instrument.conf";
}

namespace channel::local {
    bool acces = false;
    linalg::fvector sens(17);
}

namespace i2c::local {
    std::string path_name = "device_path";
    std::string path;
}

namespace bmp388::local {
    double pres1, pres2, pres_mean;
    double temp1, temp2, temp_mean;

    bool valid1, valid2, valid_mean = false;
}

namespace bno055::local {
    std::string rad_acc_name = "accelerometer_radii", rad_mag_name = "magnetometer_radii";
    std::string off_acc_name = "accelerometer_offsets", off_mag_name = "magnetometer_offsets", off_gyr_name = "gyroscope_offsets";

    std::vector<int> rad_acc, rad_mag;
    std::vector<linalg::ivector> off_acc, off_mag, off_gyr;

    int rad_acc1, rad_mag1;
    int rad_acc2, rad_mag2;

    linalg::ivector off_acc1, off_mag1, off_gyr1;
    linalg::ivector off_acc2, off_mag2, off_gyr2;

    linalg::fvector rot1, rot2, rot_mean;
    linalg::fvector qua1, qua2, qua_mean;
    linalg::fvector lia1, lia2, lia_mean;

    bool valid1, valid2, valid_mean = false;
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

        bno055::local::rad_acc = config.get<std::vector<int>>(bno055::local::rad_acc_name);
        bno055::local::rad_mag = config.get<std::vector<int>>(bno055::local::rad_mag_name);
        bno055::local::off_acc = config.get<std::vector<linalg::ivector>>(bno055::local::off_acc_name);
        bno055::local::off_mag = config.get<std::vector<linalg::ivector>>(bno055::local::off_mag_name);
        bno055::local::off_gyr = config.get<std::vector<linalg::ivector>>(bno055::local::off_gyr_name);

        bno055::local::rad_acc1 = bno055::local::rad_acc[0];
        bno055::local::rad_mag1 = bno055::local::rad_mag[0];
        bno055::local::off_acc1 = bno055::local::off_acc[0];
        bno055::local::off_mag1 = bno055::local::off_mag[0];
        bno055::local::off_gyr1 = bno055::local::off_gyr[0];

        bno055::local::rad_acc2 = bno055::local::rad_acc[1];
        bno055::local::rad_mag2 = bno055::local::rad_mag[1];
        bno055::local::off_acc2 = bno055::local::off_acc[1];
        bno055::local::off_mag2 = bno055::local::off_mag[1];
        bno055::local::off_gyr2 = bno055::local::off_gyr[1];

        interrupt::instance interrupt;
        timing::instance timing;

        channel::instance channel(channel::mode::none);

        i2c::instance i2c(i2c::local::path);
        bmp388::instance bmp388_1(i2c, 1, 1, 2, 0, 2),
                         bmp388_2(i2c, 2, 1, 2, 0, 2);
        bno055::instance bno055_1(
                             i2c, 1, bno055::axis::pz, bno055::axis::px, bno055::axis::py,
                             bno055::local::rad_acc1, bno055::local::rad_mag1,
                             bno055::local::off_acc1, bno055::local::off_mag1, bno055::local::off_gyr1
                         ),
                         bno055_2(
                             i2c, 2, bno055::axis::pz, bno055::axis::px, bno055::axis::py,
                             bno055::local::rad_acc2, bno055::local::rad_mag2,
                             bno055::local::off_acc2, bno055::local::off_mag2, bno055::local::off_gyr2
                         );

        while (!interrupt.caught()) {
            timing.sleep(timing::local::dur);

            timing::local::time = timing.get_time();

            try {
                bmp388_1.update();
                bmp388::local::pres1 = bmp388_1.get_pres();
                bmp388::local::temp1 = bmp388_1.get_temp();
                bmp388::local::valid1 = true;
            } catch (...) {
                bmp388::local::valid1 = false;
            }

            try {
                bmp388_2.update();
                bmp388::local::pres2 = bmp388_2.get_pres();
                bmp388::local::temp2 = bmp388_2.get_temp();
                bmp388::local::valid2 = true;
            } catch (...) {
                bmp388::local::valid2 = false;
            }

            try {
                bno055_1.update();
                bno055::local::rot1 = bno055_1.get_rot();
                bno055::local::qua1 = bno055_1.get_qua();
                bno055::local::lia1 = bno055_1.get_lia();
                bno055::local::valid1 = true;
            } catch (...) {
                bno055::local::valid1 = false;
            }

            try {
                bno055_2.update();
                bno055::local::rot2 = bno055_2.get_rot();
                bno055::local::qua2 = bno055_2.get_qua();
                bno055::local::lia2 = bno055_2.get_lia();
                bno055::local::valid2 = true;
            } catch (...) {
                bno055::local::valid2 = false;
            }

            if (bmp388::local::valid1 && bmp388::local::valid2) {
                bmp388::local::pres_mean = 0.5 * (bmp388::local::pres1 + bmp388::local::pres2);
                bmp388::local::temp_mean = 0.5 * (bmp388::local::temp1 + bmp388::local::temp2);
            } else if (bmp388::local::valid1) {
                bmp388::local::pres_mean = bmp388::local::pres1;
                bmp388::local::temp_mean = bmp388::local::temp1;
            } else if (bmp388::local::valid2) {
                bmp388::local::pres_mean = bmp388::local::pres2;
                bmp388::local::temp_mean = bmp388::local::temp2;
            }

            if (bno055::local::valid1 && bno055::local::valid2) {
                bno055::local::rot_mean = 0.5 * (bno055::local::rot1 + bno055::local::rot2);
                bno055::local::qua_mean = 0.5 * (bno055::local::qua1 + bno055::local::qua2);
                bno055::local::lia_mean = 0.5 * (bno055::local::lia1 + bno055::local::lia2);
                bno055::local::qua_mean /= linalg::norm(bno055::local::qua_mean);
            } else if (bno055::local::valid1) {
                bno055::local::rot_mean = bno055::local::rot1;
                bno055::local::qua_mean = bno055::local::qua1;
                bno055::local::lia_mean = bno055::local::lia1;
            } else if (bno055::local::valid2) {
                bno055::local::rot_mean = bno055::local::rot2;
                bno055::local::qua_mean = bno055::local::qua2;
                bno055::local::lia_mean = bno055::local::lia2;
            }

            if (bmp388::local::valid1 || bmp388::local::valid2) {
                bmp388::local::valid_mean = true;
                channel::local::sens[10] = bmp388::local::pres_mean;
                channel::local::sens[11] = bmp388::local::temp_mean;
                channel::local::sens[15] = timing::local::time;
                channel::local::sens[16] = timing::local::time;
            }

            if (bno055::local::valid1 || bno055::local::valid2) {
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

            try {
                if (!channel::local::acces) {
                    channel.acquire();
                    channel::local::acces = true;
                }
            } catch (...) {}

            try {
                if (channel::local::acces) {
                    channel.put_sens(channel::local::sens);
                }
            } catch (...) {}

            try {
                if (channel::local::acces) {
                    channel.release();
                    channel::local::acces = false;
                }
            } catch (...) {}
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
