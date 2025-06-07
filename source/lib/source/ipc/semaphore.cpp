#include <cstring>
#include <cerrno>

#include <string>

#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>

#include <sys/logging.hpp>
#include <ipc/semaphore.hpp>

namespace semaphore {

int tracker::count = 0;

int tracker::instantiate (void) {
    tracker::count++;
    return tracker::count;
}

instance::instance (const std::string & name, mode mode)
  : logging("semaphore"), init(false), id(instance::instantiate()), name(name), locked(false),
    create(mode == mode::create || mode == mode::both), remove(mode == mode::remove || mode == mode::both) {

    this->logging.inf("Initializing instance #", this->id, ": Name: ", this->name);

    this->logging.inf("Opening semaphore #", this->id);
    if (this->create) {
        this->sem = sem_open(this->name.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH, 1);
    } else {
        this->sem = sem_open(this->name.c_str(), O_RDWR);
    }
    if (this->sem == SEM_FAILED) {
        this->logging.err("Failed to open semaphore #", this->id, " (", std::strerror(errno), ")");
        this->logging.err("Failed to initialize instance #", this->id);
        throw except_ctor::fail;
    }

    this->init = true;
}

instance::~instance (void) {
    int ret;

    if (this->init) {
        if (this->locked) {
            this->logging.inf("Releasing semaphore #", this->id);
            ret = sem_post(this->sem);
            if (ret < 0) {
                this->logging.err("Failed to release semaphore #", this->id, " (", std::strerror(errno), ")");
            }
        }

        this->logging.inf("Closing semaphore #", this->id);
        ret = sem_close(this->sem);
        if (ret < 0) {
            this->logging.err("Failed to close semaphore #", this->id, " (", std::strerror(errno), ")");
        }

        if (this->remove) {
            this->logging.inf("Unlinking semaphore #", this->id);
            ret = sem_unlink(this->name.c_str());
            if (ret < 0) {
                this->logging.err("Failed to unlink semaphore #", this->id, " (", std::strerror(errno), ")");
            }
        }
    }
}

void instance::acquire (void) {
    int ret;

    this->logging.inf("Acquiring semaphore #", this->id);

    if (!this->init) {
        this->logging.err("Failed to acquire semaphore #", this->id, " (Instance not initialized)");
        throw except_acquire::fail;
    }

    if (this->locked) {
        this->logging.err("Failed to acquire semaphore #", this->id, " (Semaphore already acquired)");
        throw except_acquire::fail;
    }

    ret = sem_wait(this->sem);
    if (ret < 0) {
        this->logging.err("Failed to acquire semaphore #", this->id, " (", std::strerror(errno), ")");
        throw except_acquire::fail;
    }

    this->locked = true;
}

void instance::release (void) {
    int ret;

    this->logging.inf("Releasing semaphore #", this->id);

    if (!this->init) {
        this->logging.err("Failed to release semaphore #", this->id, " (Instance not initialized)");
        throw except_release::fail;
    }

    if (!this->locked) {
        this->logging.err("Failed to release semaphore #", this->id, " (Semaphore already released)");
        throw except_release::fail;
    }

    ret = sem_post(this->sem);
    if (ret < 0) {
        this->logging.err("Failed to release semaphore #", this->id, " (", std::strerror(errno), ")");
        throw except_release::fail;
    }

    this->locked = false;
}

}
