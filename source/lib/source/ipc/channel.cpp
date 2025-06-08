#include <cmath>

#include <math/linalg.hpp>
#include <sys/logging.hpp>
#include <ipc/semaphore.hpp>
#include <ipc/sharedmem.hpp>
#include <ipc/channel.hpp>

namespace channel {

bool tracker::exist = false;

bool tracker::instantiate (void) {
    if (tracker::exist) {
        return false;
    } else {
        tracker::exist = true;
        return true;
    }
}

instance::instance (mode mode) try
  : logging("channel"), init(false), locked(false),
    acces(
        "acces",
        (mode == mode::none) ? semaphore::mode::none :
        (mode == mode::create) ? semaphore::mode::create :
        (mode == mode::remove) ? semaphore::mode::remove : semaphore::mode::both
    ),
    ctrl_drdy(
        "ctrl-drdy",
        (mode == mode::none) ? sharedmem::mode::none :
        (mode == mode::create) ? sharedmem::mode::create :
        (mode == mode::remove) ? sharedmem::mode::remove : sharedmem::mode::both
    ),
    stat_drdy(
        "stat-drdy",
        (mode == mode::none) ? sharedmem::mode::none :
        (mode == mode::create) ? sharedmem::mode::create :
        (mode == mode::remove) ? sharedmem::mode::remove : sharedmem::mode::both
    ),
    estm_drdy(
        "estm-drdy",
        (mode == mode::none) ? sharedmem::mode::none :
        (mode == mode::create) ? sharedmem::mode::create :
        (mode == mode::remove) ? sharedmem::mode::remove : sharedmem::mode::both
    ),
    sens_drdy(
        "sens-drdy",
        (mode == mode::none) ? sharedmem::mode::none :
        (mode == mode::create) ? sharedmem::mode::create :
        (mode == mode::remove) ? sharedmem::mode::remove : sharedmem::mode::both
    ),
    ctrl_data(
        "ctrl-data",
        (mode == mode::none) ? sharedmem::mode::none :
        (mode == mode::create) ? sharedmem::mode::create :
        (mode == mode::remove) ? sharedmem::mode::remove : sharedmem::mode::both
    ),
    stat_data(
        "stat-data", 7,
        (mode == mode::none) ? sharedmem::mode::none :
        (mode == mode::create) ? sharedmem::mode::create :
        (mode == mode::remove) ? sharedmem::mode::remove : sharedmem::mode::both
    ),
    estm_data(
        "estm-data", 4,
        (mode == mode::none) ? sharedmem::mode::none :
        (mode == mode::create) ? sharedmem::mode::create :
        (mode == mode::remove) ? sharedmem::mode::remove : sharedmem::mode::both
    ),
    sens_data(
        "sens-data", 17,
        (mode == mode::none) ? sharedmem::mode::none :
        (mode == mode::create) ? sharedmem::mode::create :
        (mode == mode::remove) ? sharedmem::mode::remove : sharedmem::mode::both
    ) {
    this->logging.inf("Initializing instance");

    if (!instance::instantiate()) {
        this->logging.err("Failed to initialize instance (Instance already exists)");
        throw except_ctor::fail;
    }

    this->init = true;
} catch (semaphore::instance::except_ctor) {
    throw except_ctor::fail;
} catch (sharedmem::instance<bool>::except_ctor) {
    throw except_ctor::fail;
} catch (sharedmem::instance<double>::except_ctor) {
    throw except_ctor::fail;
} catch (sharedmem::instance<linalg::fvector>::except_ctor) {
    throw except_ctor::fail;
}

void instance::put_ctrl (double ctrl) {
    this->logging.inf("Writing control signal to channel: Ctrl: ", ctrl);

    if (!this->init) {
        this->logging.err("Failed to write control signal to channel (Instance not initialized)");
        throw except_put_ctrl::fail;
    }

    if (!this->locked) {
        this->logging.err("Failed to write control signal to channel (Access not acquired)");
        throw except_put_ctrl::fail;
    }

    if (ctrl < 0 || ctrl > 1) {
        this->logging.err("Failed to write control signal to channel (Invalid control signal)");
        throw except_put_ctrl::fail;
    }

    try {
        this->ctrl_data.put(ctrl);
        this->ctrl_drdy.put(true);
    } catch (...) {
        this->logging.err("Failed to write control signal to channel");
        throw except_put_ctrl::fail;
    }
}

void instance::put_stat (const linalg::fvector & stat) {
    this->logging.inf("Writing state vector to channel: Stat: ", stat);

    if (!this->init) {
        this->logging.err("Failed to write state vector to channel (Instance not initialized)");
        throw except_put_stat::fail;
    }

    if (!this->locked) {
        this->logging.err("Failed to write state vector to channel (Access not acquired)");
        throw except_put_stat::fail;
    }

    if (linalg::dim(stat) != 7) {
        this->logging.err("Failed to write state vector to channel (Invalid state vector)");
        throw except_put_stat::fail;
    }

    if (stat[2] < -M_PI || stat[2] > M_PI) {
        this->logging.err("Failed to write state vector to channel (Invalid state vector)");
        throw except_put_stat::fail;
    }

    if (stat[6] < 0 || stat[6] > 1) {
        this->logging.err("Failed to write state vector to channel (Invalid state vector)");
        throw except_put_stat::fail;
    }

    try {
        this->stat_data.put(stat);
        this->stat_drdy.put(true);
    } catch (...) {
        this->logging.err("Failed to write state vector to channel");
        throw except_put_stat::fail;
    }
}

void instance::put_estm (const linalg::fvector & estm) {
    this->logging.inf("Writing estimate vector to channel: Estm: ", estm);

    if (!this->init) {
        this->logging.err("Failed to write estimate vector to channel (Instance not initialized)");
        throw except_put_estm::fail;
    }

    if (!this->locked) {
        this->logging.err("Failed to write estimate vector to channel (Access not acquired)");
        throw except_put_estm::fail;
    }

    if (linalg::dim(estm) != 4) {
        this->logging.err("Failed to write estimate vector to channel (Invalid estimate vector)");
        throw except_put_estm::fail;
    }

    if (estm[1] < 0 || estm[1] > M_PI) {
        this->logging.err("Failed to write estimate vector to channel (Invalid estimate vector)");
        throw except_put_estm::fail;
    }

    try {
        this->estm_data.put(estm);
        this->estm_drdy.put(true);
    } catch (...) {
        this->logging.err("Failed to write estimate vector to channel");
        throw except_put_estm::fail;
    }
}

void instance::put_sens (const linalg::fvector & sens) {
    this->logging.inf("Writing sensing vector to channel: Sens: ", sens);

    if (!this->init) {
        this->logging.err("Failed to write sensing vector to channel (Instance not initialized)");
        throw except_put_sens::fail;
    }

    if (!this->locked) {
        this->logging.err("Failed to write sensing vector to channel (Access not acquired)");
        throw except_put_sens::fail;
    }

    if (linalg::dim(sens) != 17) {
        this->logging.err("Failed to write sensing vector to channel (Invalid sensing vector)");
        throw except_put_sens::fail;
    }

    if (sens[10] <= 0) {
        this->logging.err("Failed to write sensing vector to channel (Invalid sensing vector)");
        throw except_put_sens::fail;
    }

    if (sens[11] <= 0) {
        this->logging.err("Failed to write sensing vector to channel (Invalid sensing vector)");
        throw except_put_sens::fail;
    }

    if (sens[12] <= 0) {
        this->logging.err("Failed to write sensing vector to channel (Invalid sensing vector)");
        throw except_put_sens::fail;
    }

    if (sens[13] <= 0) {
        this->logging.err("Failed to write sensing vector to channel (Invalid sensing vector)");
        throw except_put_sens::fail;
    }

    if (sens[14] <= 0) {
        this->logging.err("Failed to write sensing vector to channel (Invalid sensing vector)");
        throw except_put_sens::fail;
    }

    if (sens[15] <= 0) {
        this->logging.err("Failed to write sensing vector to channel (Invalid sensing vector)");
        throw except_put_sens::fail;
    }

    if (sens[16] <= 0) {
        this->logging.err("Failed to write sensing vector to channel (Invalid sensing vector)");
        throw except_put_sens::fail;
    }

    try {
        this->sens_data.put(sens);
        this->sens_drdy.put(true);
    } catch (...) {
        this->logging.err("Failed to write sensing vector to channel");
        throw except_put_sens::fail;
    }
}

double instance::get_ctrl (void) {
    bool flag;
    double ctrl;

    this->logging.inf("Reading control signal from channel");

    if (!this->init) {
        this->logging.err("Failed to read control signal from channel (Instance not initialized)");
        throw except_get_ctrl::fail;
    }

    if (!this->locked) {
        this->logging.err("Failed to read control signal from channel (Access not acquired)");
        throw except_get_ctrl::fail;
    }

    try {
        flag = this->ctrl_drdy.get();
    } catch (...) {
        this->logging.err("Failed to read control signal from channel");
        throw except_get_ctrl::fail;
    }

    if (!flag) {
        this->logging.wrn("No control signal in channel");
        throw except_get_ctrl::blank;
    }

    try {
        ctrl = this->ctrl_data.get();
    } catch (...) {
        this->logging.err("Failed to read control signal from channel");
        throw except_get_ctrl::fail;
    }

    this->logging.inf("Read control signal from channel: Ctrl: ", ctrl);

    return ctrl;
}

linalg::fvector instance::get_stat (void) {
    bool flag;
    linalg::fvector stat;

    this->logging.inf("Reading state vector from channel");

    if (!this->init) {
        this->logging.err("Failed to read state vector from channel (Instance not initialized)");
        throw except_get_stat::fail;
    }

    if (!this->locked) {
        this->logging.err("Failed to read state vector from channel (Access not acquired)");
        throw except_get_stat::fail;
    }

    try {
        flag = this->stat_drdy.get();
    } catch (...) {
        this->logging.err("Failed to read state vector from channel");
        throw except_get_stat::fail;
    }

    if (!flag) {
        this->logging.wrn("No state vector in channel");
        throw except_get_stat::blank;
    }

    try {
        stat = this->stat_data.get();
    } catch (...) {
        this->logging.err("Failed to read state vector from channel");
        throw except_get_stat::fail;
    }

    this->logging.inf("Read state vector from channel: Stat: ", stat);

    return stat;
}

linalg::fvector instance::get_estm (void) {
    bool flag;
    linalg::fvector estm;

    this->logging.inf("Reading estimate vector from channel");

    if (!this->init) {
        this->logging.err("Failed to read estimate vector from channel (Instance not initialized)");
        throw except_get_estm::fail;
    }

    if (!this->locked) {
        this->logging.err("Failed to read estimate vector from channel (Access not acquired)");
        throw except_get_estm::fail;
    }

    try {
        flag = this->estm_drdy.get();
    } catch (...) {
        this->logging.err("Failed to read estimate vector from channel");
        throw except_get_estm::fail;
    }

    if (!flag) {
        this->logging.wrn("No estimate vector in channel");
        throw except_get_estm::blank;
    }

    try {
        estm = this->estm_data.get();
    } catch (...) {
        this->logging.err("Failed to read estimate vector from channel");
        throw except_get_estm::fail;
    }

    this->logging.inf("Read estimate vector from channel: Estm: ", estm);

    return estm;
}

linalg::fvector instance::get_sens (void) {
    bool flag;
    linalg::fvector sens;

    this->logging.inf("Reading sensing vector from channel");

    if (!this->init) {
        this->logging.err("Failed to read sensing vector from channel (Instance not initialized)");
        throw except_get_sens::fail;
    }

    if (!this->locked) {
        this->logging.err("Failed to read sensing vector from channel (Access not acquired)");
        throw except_get_sens::fail;
    }

    try {
        flag = this->sens_drdy.get();
    } catch (...) {
        this->logging.err("Failed to read sensing vector from channel");
        throw except_get_sens::fail;
    }

    if (!flag) {
        this->logging.wrn("No sensing vector in channel");
        throw except_get_sens::blank;
    }

    try {
        sens = this->sens_data.get();
    } catch (...) {
        this->logging.err("Failed to read sensing vector from channel");
        throw except_get_sens::fail;
    }

    this->logging.inf("Read sensing vector from channel: Sens: ", sens);

    return sens;
}

void instance::clear (void) {
    this->logging.inf("Clearing channels");

    if (!this->init) {
        this->logging.err("Failed to clear channels (Instance not initialized)");
        throw except_clear::fail;
    }

    if (!this->locked) {
        this->logging.err("Failed to clear channels (Access not acquired)");
        throw except_clear::fail;
    }

    try {
        this->ctrl_drdy.put(false);
        this->stat_drdy.put(false);
        this->estm_drdy.put(false);
        this->sens_drdy.put(false);
    } catch (...) {
        this->logging.err("Failed to clear channels");
        throw except_clear::fail;
    }
}

void instance::acquire (void) {
    this->logging.inf("Acquiring access");

    if (!this->init) {
        this->logging.err("Failed to acquire access (Instance not initialized)");
        throw except_acquire::fail;
    }

    if (this->locked) {
        this->logging.err("Failed to acquire access (Access already acquired)");
        throw except_acquire::fail;
    }

    try {
        this->acces.acquire();
    } catch (...) {
        this->logging.err("Failed to acquire access");
        throw except_acquire::fail;
    }

    this->locked = true;
}

void instance::release (void) {
    this->logging.inf("Releasing access");

    if (!this->init) {
        this->logging.err("Failed to release access (Instance not initialized)");
        throw except_release::fail;
    }

    if (!this->locked) {
        this->logging.err("Failed to release access (Access already released)");
        throw except_release::fail;
    }

    try {
        this->acces.release();
    } catch (...) {
        this->logging.err("Failed to release access");
        throw except_release::fail;
    }

    this->locked = false;
}

}
