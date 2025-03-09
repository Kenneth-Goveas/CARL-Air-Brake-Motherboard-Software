#include <cstring>
#include <cerrno>

#include <string>
#include <vector>

#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>

#include <sys/logging.hpp>

#include <ipc/semaphore.hpp>

namespace semaphore :: intern {

std::string mod = "semaphore";

bool fail;

std::vector<sem_t *> sem;

}

namespace semaphore {

bool fail (void) {
    return intern::fail;
}

bool exists (std::string name) {
    int ret;
    sem_t * sem;
    bool flag;

    logging::inf(intern::mod,
        "Checking if semaphore exists: Name: ", name
    );

    logging::inf(intern::mod,
        "Opening semaphore"
    );

    sem = sem_open(name.c_str(), O_RDWR);
    if (sem == SEM_FAILED) {
        if (errno != ENOENT) {
            logging::err(intern::mod,
                "Failed to open semaphore (", std::strerror(errno), ")"
            );
            logging::err(intern::mod,
                "Failed to check if semaphore exists"
            );
            intern::fail = true;
            return flag;
        } else {
            logging::inf(intern::mod,
                "Semaphore does not exist"
            );
            flag = false;
            intern::fail = false;
            return flag;
        }
    }

    logging::inf(intern::mod,
        "Closing semaphore"
    );

    ret = sem_close(sem);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to close semaphore (", std::strerror(errno), ")"
        );
        logging::err(intern::mod,
            "Failed to check if semaphore exists"
        );
        intern::fail = true;
        return flag;
    }

    logging::inf(intern::mod,
        "Semaphore exists"
    );

    flag = true;

    intern::fail = false;
    return flag;
}

void create (std::string name) {
    int ret;
    sem_t * sem;

    logging::inf(intern::mod,
        "Creating semaphore: Name: ", name
    );

    logging::inf(intern::mod,
        "Opening semaphore"
    );

    sem = sem_open(name.c_str(), O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH, 1);
    if (sem == SEM_FAILED) {
        logging::err(intern::mod,
            "Failed to open semaphore (", std::strerror(errno), ")"
        );
        logging::err(intern::mod,
            "Failed to create semaphore"
        );
        intern::fail = true;
        return;
    }

    logging::inf(intern::mod,
        "Closing semaphore"
    );

    ret = sem_close(sem);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to close semaphore (", std::strerror(errno), ")"
        );
        logging::wrn(intern::mod,
            "Removing semaphore"
        );
        ret = sem_unlink(name.c_str());
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to remove semaphore (", std::strerror(errno), ")"
            );
        }
        logging::err(intern::mod,
            "Failed to create semaphore"
        );
        intern::fail = true;
        return;
    }

    intern::fail = false;
}

void remove (std::string name) {
    int ret;

    logging::inf(intern::mod,
        "Removing semaphore: Name: ", name
    );

    ret = sem_unlink(name.c_str());
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to remove semaphore (", std::strerror(errno), ")"
        );
        intern::fail = true;
        return;
    }

    intern::fail = false;
}

int open (std::string name) {
    int id = intern::sem.size() + 1;
    sem_t * sem;

    logging::inf(intern::mod,
        "Opening semaphore #", id, ": Name: ", name
    );

    sem = sem_open(name.c_str(), O_RDWR);
    if (sem == SEM_FAILED) {
        logging::err(intern::mod,
            "Failed to open semaphore #", id, " (", std::strerror(errno), ")"
        );
        intern::fail = true;
        return id;
    }

    intern::sem.push_back(sem);

    intern::fail = false;
    return id;
}

void close (void) {
    int ret;

    intern::fail = false;
    for (int i = 0; i < intern::sem.size(); i++) {
        logging::inf(intern::mod,
            "Closing semaphore #", i + 1
        );
        ret = sem_close(intern::sem[i]);
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to close semaphore #", i + 1, " (", std::strerror(errno), ")"
            );
            intern::fail = true;
        }
    }

    intern::sem.clear();
}

void acquire (int id) {
    int ret;

    logging::inf(intern::mod,
        "Acquiring semaphore #", id
    );

    if (id <= 0 || id > intern::sem.size()) {
        logging::err(intern::mod,
            "Failed to acquire semaphore #", id, " (Semaphore not found)"
        );
        intern::fail = true;
        return;
    }

    ret = sem_wait(intern::sem[id - 1]);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to acquire semaphore #", id, " (", std::strerror(errno), ")"
        );
        intern::fail = true;
        return;
    }

    intern::fail = false;
}

void release (int id) {
    int ret;

    logging::inf(intern::mod,
        "Releasing semaphore #", id
    );

    if (id <= 0 || id > intern::sem.size()) {
        logging::err(intern::mod,
            "Failed to release semaphore #", id, " (Semaphore not found)"
        );
        intern::fail = true;
        return;
    }

    ret = sem_post(intern::sem[id - 1]);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to release semaphore #", id, " (", std::strerror(errno), ")"
        );
        intern::fail = true;
        return;
    }

    intern::fail = false;
}

}
