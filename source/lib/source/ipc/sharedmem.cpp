#include <cstddef>
#include <cstring>
#include <cerrno>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <math/linalg.hpp>

#include <sys/logging.hpp>

#include <ipc/sharedmem.hpp>

namespace sharedmem :: intern {

std::string mod = "sharedmem";

bool fail;

std::vector<std::size_t> len;
std::vector<void *> data;

std::string conv_val_to_oupt (bool val);
std::string conv_val_to_oupt (int val);
std::string conv_val_to_oupt (double val);
std::string conv_val_to_oupt (linalg::ivector val);
std::string conv_val_to_oupt (linalg::fvector val);
std::string conv_val_to_oupt (linalg::imatrix val);
std::string conv_val_to_oupt (linalg::fmatrix val);

}

namespace sharedmem {

bool fail (void) {
    return intern::fail;
}

bool exists (std::string name) {
    int ret, desc;
    bool flag;

    logging::inf(intern::mod,
        "Checking if shared memory object exists: Name: “", name, "”"
    );

    logging::inf(intern::mod,
        "Opening shared memory object"
    );

    desc = shm_open(name.c_str(), O_RDWR, S_IRUSR | S_IWUSR);
    if (desc < 0) {
        if (errno != ENOENT) {
            logging::err(intern::mod,
                "Failed to open shared memory object ",
                "(", std::strerror(errno), ")"
            );
            logging::err(intern::mod,
                "Failed to check if shared memory object exists"
            );
            intern::fail = true;
            return flag;
        } else {
            logging::inf(intern::mod,
                "Shared memory object does not exist"
            );
            flag = false;
            intern::fail = false;
            return flag;
        }
    }

    logging::inf(intern::mod,
        "Closing shared memory object"
    );

    ret = ::close(desc);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to close shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::err(intern::mod,
            "Failed to check if shared memory object exists"
        );
        intern::fail = true;
        return flag;
    }

    logging::inf(intern::mod,
        "Shared memory object exists"
    );

    flag = true;

    intern::fail = false;
    return flag;
}

template <>
bool fits<bool> (std::string name) {
    int ret, desc;
    bool flag;
    std::size_t len;
    struct stat fdat;

    len = sizeof(bool);

    logging::inf(intern::mod,
        "Checking size of shared memory object: ",
        "Name: “", name, "”, Type: Boolean, Size: ", len
    );

    logging::inf(intern::mod,
        "Opening shared memory object"
    );

    desc = shm_open(name.c_str(), O_RDWR, S_IRUSR | S_IWUSR);
    if (desc < 0) {
        logging::err(intern::mod,
            "Failed to open shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::err(intern::mod,
            "Failed to check size of shared memory object"
        );
        intern::fail = true;
        return flag;
    }

    logging::inf(intern::mod,
        "Getting size of shared memory object"
    );

    ret = fstat(desc, &fdat);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to get size of shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::wrn(intern::mod,
            "Closing shared memory object"
        );
        ret = ::close(desc);
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to close shared memory object ",
                "(", std::strerror(errno), ")"
            );
        }
        logging::err(intern::mod,
            "Failed to check size of shared memory object"
        );
        intern::fail = true;
        return flag;
    }

    logging::inf(intern::mod,
        "Got size of shared memory object: Size: ", fdat.st_size
    );

    logging::inf(intern::mod,
        "Closing shared memory object"
    );

    ret = ::close(desc);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to close shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::err(intern::mod,
            "Failed to check size of shared memory object"
        );
        intern::fail = true;
        return flag;
    }

    if (fdat.st_size == len) {
        logging::inf(intern::mod,
            "Size of shared memory object matches"
        );
        flag = true;
    } else {
        logging::inf(intern::mod,
            "Size of shared memory object does not match"
        );
        flag = false;
    }

    intern::fail = false;
    return flag;
}

template <>
bool fits<int> (std::string name) {
    int ret, desc;
    bool flag;
    std::size_t len;
    struct stat fdat;

    len = sizeof(int);

    logging::inf(intern::mod,
        "Checking size of shared memory object: ",
        "Name: “", name, "”, Type: Integer scalar, Size: ", len
    );

    logging::inf(intern::mod,
        "Opening shared memory object"
    );

    desc = shm_open(name.c_str(), O_RDWR, S_IRUSR | S_IWUSR);
    if (desc < 0) {
        logging::err(intern::mod,
            "Failed to open shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::err(intern::mod,
            "Failed to check size of shared memory object"
        );
        intern::fail = true;
        return flag;
    }

    logging::inf(intern::mod,
        "Getting size of shared memory object"
    );

    ret = fstat(desc, &fdat);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to get size of shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::wrn(intern::mod,
            "Closing shared memory object"
        );
        ret = ::close(desc);
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to close shared memory object ",
                "(", std::strerror(errno), ")"
            );
        }
        logging::err(intern::mod,
            "Failed to check size of shared memory object"
        );
        intern::fail = true;
        return flag;
    }

    logging::inf(intern::mod,
        "Got size of shared memory object: Size: ", fdat.st_size
    );

    logging::inf(intern::mod,
        "Closing shared memory object"
    );

    ret = ::close(desc);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to close shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::err(intern::mod,
            "Failed to check size of shared memory object"
        );
        intern::fail = true;
        return flag;
    }

    if (fdat.st_size == len) {
        logging::inf(intern::mod,
            "Size of shared memory object matches"
        );
        flag = true;
    } else {
        logging::inf(intern::mod,
            "Size of shared memory object does not match"
        );
        flag = false;
    }

    intern::fail = false;
    return flag;
}

template <>
bool fits<double> (std::string name) {
    int ret, desc;
    bool flag;
    std::size_t len;
    struct stat fdat;

    len = sizeof(double);

    logging::inf(intern::mod,
        "Checking size of shared memory object: ",
        "Name: “", name, "”, Type: Real scalar, Size: ", len
    );

    logging::inf(intern::mod,
        "Opening shared memory object"
    );

    desc = shm_open(name.c_str(), O_RDWR, S_IRUSR | S_IWUSR);
    if (desc < 0) {
        logging::err(intern::mod,
            "Failed to open shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::err(intern::mod,
            "Failed to check size of shared memory object"
        );
        intern::fail = true;
        return flag;
    }

    logging::inf(intern::mod,
        "Getting size of shared memory object"
    );

    ret = fstat(desc, &fdat);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to get size of shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::wrn(intern::mod,
            "Closing shared memory object"
        );
        ret = ::close(desc);
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to close shared memory object ",
                "(", std::strerror(errno), ")"
            );
        }
        logging::err(intern::mod,
            "Failed to check size of shared memory object"
        );
        intern::fail = true;
        return flag;
    }

    logging::inf(intern::mod,
        "Got size of shared memory object: Size: ", fdat.st_size
    );

    logging::inf(intern::mod,
        "Closing shared memory object"
    );

    ret = ::close(desc);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to close shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::err(intern::mod,
            "Failed to check size of shared memory object"
        );
        intern::fail = true;
        return flag;
    }

    if (fdat.st_size == len) {
        logging::inf(intern::mod,
            "Size of shared memory object matches"
        );
        flag = true;
    } else {
        logging::inf(intern::mod,
            "Size of shared memory object does not match"
        );
        flag = false;
    }

    intern::fail = false;
    return flag;
}

template <>
bool fits<linalg::ivector> (std::string name, int dim) {
    int ret, desc;
    bool flag;
    std::size_t len;
    struct stat fdat;

    len = dim * sizeof(int);

    logging::inf(intern::mod,
        "Checking size of shared memory object: ",
        "Name: “", name, "”, Type: Integer vector, Dim: ", dim, ", Size: ", len
    );

    if (!(dim > 0)) {
        logging::err(intern::mod,
            "Failed to check size of shared memory object (Invalid dimension)"
        );
        intern::fail = true;
        return flag;
    }

    logging::inf(intern::mod,
        "Opening shared memory object"
    );

    desc = shm_open(name.c_str(), O_RDWR, S_IRUSR | S_IWUSR);
    if (desc < 0) {
        logging::err(intern::mod,
            "Failed to open shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::err(intern::mod,
            "Failed to check size of shared memory object"
        );
        intern::fail = true;
        return flag;
    }

    logging::inf(intern::mod,
        "Getting size of shared memory object"
    );

    ret = fstat(desc, &fdat);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to get size of shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::wrn(intern::mod,
            "Closing shared memory object"
        );
        ret = ::close(desc);
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to close shared memory object ",
                "(", std::strerror(errno), ")"
            );
        }
        logging::err(intern::mod,
            "Failed to check size of shared memory object"
        );
        intern::fail = true;
        return flag;
    }

    logging::inf(intern::mod,
        "Got size of shared memory object: Size: ", fdat.st_size
    );

    logging::inf(intern::mod,
        "Closing shared memory object"
    );

    ret = ::close(desc);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to close shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::err(intern::mod,
            "Failed to check size of shared memory object"
        );
        intern::fail = true;
        return flag;
    }

    if (fdat.st_size == len) {
        logging::inf(intern::mod,
            "Size of shared memory object matches"
        );
        flag = true;
    } else {
        logging::inf(intern::mod,
            "Size of shared memory object does not match"
        );
        flag = false;
    }

    intern::fail = false;
    return flag;
}

template <>
bool fits<linalg::fvector> (std::string name, int dim) {
    int ret, desc;
    bool flag;
    std::size_t len;
    struct stat fdat;

    len = dim * sizeof(double);

    logging::inf(intern::mod,
        "Checking size of shared memory object: ",
        "Name: “", name, "”, Type: Real vector, Dim: ", dim, ", Size: ", len
    );

    if (!(dim > 0)) {
        logging::err(intern::mod,
            "Failed to check size of shared memory object (Invalid dimension)"
        );
        intern::fail = true;
        return flag;
    }

    logging::inf(intern::mod,
        "Opening shared memory object"
    );

    desc = shm_open(name.c_str(), O_RDWR, S_IRUSR | S_IWUSR);
    if (desc < 0) {
        logging::err(intern::mod,
            "Failed to open shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::err(intern::mod,
            "Failed to check size of shared memory object"
        );
        intern::fail = true;
        return flag;
    }

    logging::inf(intern::mod,
        "Getting size of shared memory object"
    );

    ret = fstat(desc, &fdat);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to get size of shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::wrn(intern::mod,
            "Closing shared memory object"
        );
        ret = ::close(desc);
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to close shared memory object ",
                "(", std::strerror(errno), ")"
            );
        }
        logging::err(intern::mod,
            "Failed to check size of shared memory object"
        );
        intern::fail = true;
        return flag;
    }

    logging::inf(intern::mod,
        "Got size of shared memory object: Size: ", fdat.st_size
    );

    logging::inf(intern::mod,
        "Closing shared memory object"
    );

    ret = ::close(desc);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to close shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::err(intern::mod,
            "Failed to check size of shared memory object"
        );
        intern::fail = true;
        return flag;
    }

    if (fdat.st_size == len) {
        logging::inf(intern::mod,
            "Size of shared memory object matches"
        );
        flag = true;
    } else {
        logging::inf(intern::mod,
            "Size of shared memory object does not match"
        );
        flag = false;
    }

    intern::fail = false;
    return flag;
}

template <>
bool fits<linalg::imatrix> (std::string name, int rows, int cols) {
    int ret, desc;
    bool flag;
    std::size_t len;
    struct stat fdat;

    len = rows * cols * sizeof(int);

    logging::inf(intern::mod,
        "Checking size of shared memory object: ",
        "Name: “", name, "”, Type: Integer matrix, ",
        "Rows: ", rows, ", Cols: ", cols, ", Size: ", len
    );

    if (!(rows > 0 && cols > 0)) {
        logging::err(intern::mod,
            "Failed to check size of shared memory object (Invalid dimensions)"
        );
        intern::fail = true;
        return flag;
    }

    logging::inf(intern::mod,
        "Opening shared memory object"
    );

    desc = shm_open(name.c_str(), O_RDWR, S_IRUSR | S_IWUSR);
    if (desc < 0) {
        logging::err(intern::mod,
            "Failed to open shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::err(intern::mod,
            "Failed to check size of shared memory object"
        );
        intern::fail = true;
        return flag;
    }

    logging::inf(intern::mod,
        "Getting size of shared memory object"
    );

    ret = fstat(desc, &fdat);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to get size of shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::wrn(intern::mod,
            "Closing shared memory object"
        );
        ret = ::close(desc);
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to close shared memory object ",
                "(", std::strerror(errno), ")"
            );
        }
        logging::err(intern::mod,
            "Failed to check size of shared memory object"
        );
        intern::fail = true;
        return flag;
    }

    logging::inf(intern::mod,
        "Got size of shared memory object: Size: ", fdat.st_size
    );

    logging::inf(intern::mod,
        "Closing shared memory object"
    );

    ret = ::close(desc);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to close shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::err(intern::mod,
            "Failed to check size of shared memory object"
        );
        intern::fail = true;
        return flag;
    }

    if (fdat.st_size == len) {
        logging::inf(intern::mod,
            "Size of shared memory object matches"
        );
        flag = true;
    } else {
        logging::inf(intern::mod,
            "Size of shared memory object does not match"
        );
        flag = false;
    }

    intern::fail = false;
    return flag;
}

template <>
bool fits<linalg::fmatrix> (std::string name, int rows, int cols) {
    int ret, desc;
    bool flag;
    std::size_t len;
    struct stat fdat;

    len = rows * cols * sizeof(double);

    logging::inf(intern::mod,
        "Checking size of shared memory object: ",
        "Name: “", name, "”, Type: Real matrix, ",
        "Rows: ", rows, ", Cols: ", cols, ", Size: ", len
    );

    if (!(rows > 0 && cols > 0)) {
        logging::err(intern::mod,
            "Failed to check size of shared memory object (Invalid dimensions)"
        );
        intern::fail = true;
        return flag;
    }

    logging::inf(intern::mod,
        "Opening shared memory object"
    );

    desc = shm_open(name.c_str(), O_RDWR, S_IRUSR | S_IWUSR);
    if (desc < 0) {
        logging::err(intern::mod,
            "Failed to open shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::err(intern::mod,
            "Failed to check size of shared memory object"
        );
        intern::fail = true;
        return flag;
    }

    logging::inf(intern::mod,
        "Getting size of shared memory object"
    );

    ret = fstat(desc, &fdat);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to get size of shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::wrn(intern::mod,
            "Closing shared memory object"
        );
        ret = ::close(desc);
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to close shared memory object ",
                "(", std::strerror(errno), ")"
            );
        }
        logging::err(intern::mod,
            "Failed to check size of shared memory object"
        );
        intern::fail = true;
        return flag;
    }

    logging::inf(intern::mod,
        "Got size of shared memory object: Size: ", fdat.st_size
    );

    logging::inf(intern::mod,
        "Closing shared memory object"
    );

    ret = ::close(desc);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to close shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::err(intern::mod,
            "Failed to check size of shared memory object"
        );
        intern::fail = true;
        return flag;
    }

    if (fdat.st_size == len) {
        logging::inf(intern::mod,
            "Size of shared memory object matches"
        );
        flag = true;
    } else {
        logging::inf(intern::mod,
            "Size of shared memory object does not match"
        );
        flag = false;
    }

    intern::fail = false;
    return flag;
}

template <>
void create<bool> (std::string name) {
    int ret, desc;
    std::size_t len;

    len = sizeof(bool);

    logging::inf(intern::mod,
        "Creating shared memory object: ",
        "Name: “", name, "”, Type: Boolean, Size: ", len
    );

    logging::inf(intern::mod,
        "Opening shared memory object"
    );

    desc = shm_open(name.c_str(), O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (desc < 0) {
        logging::err(intern::mod,
            "Failed to open shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::err(intern::mod,
            "Failed to create shared memory object"
        );
        intern::fail = true;
        return;
    }

    logging::inf(intern::mod,
        "Resizing shared memory object"
    );

    ret = ftruncate(desc, len);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to resize shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::wrn(intern::mod,
            "Closing shared memory object"
        );
        ret = ::close(desc);
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to close shared memory object ",
                "(", std::strerror(errno), ")"
            );
        }
        logging::wrn(intern::mod,
            "Removing shared memory object"
        );
        ret = shm_unlink(name.c_str());
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to remove shared memory object ",
                "(", std::strerror(errno), ")"
            );
        }
        logging::err(intern::mod,
            "Failed to create shared memory object"
        );
        intern::fail = true;
        return;
    }

    logging::inf(intern::mod,
        "Closing shared memory object"
    );

    ret = ::close(desc);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to close shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::wrn(intern::mod,
            "Removing shared memory object"
        );
        ret = shm_unlink(name.c_str());
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to remove shared memory object ",
                "(", std::strerror(errno), ")"
            );
        }
        logging::err(intern::mod,
            "Failed to create shared memory object"
        );
        intern::fail = true;
        return;
    }

    intern::fail = false;
}

template <>
void create<int> (std::string name) {
    int ret, desc;
    std::size_t len;

    len = sizeof(int);

    logging::inf(intern::mod,
        "Creating shared memory object: ",
        "Name: “", name, "”, Type: Integer scalar, Size: ", len
    );

    logging::inf(intern::mod,
        "Opening shared memory object"
    );

    desc = shm_open(name.c_str(), O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (desc < 0) {
        logging::err(intern::mod,
            "Failed to open shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::err(intern::mod,
            "Failed to create shared memory object"
        );
        intern::fail = true;
        return;
    }

    logging::inf(intern::mod,
        "Resizing shared memory object"
    );

    ret = ftruncate(desc, len);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to resize shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::wrn(intern::mod,
            "Closing shared memory object"
        );
        ret = ::close(desc);
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to close shared memory object ",
                "(", std::strerror(errno), ")"
            );
        }
        logging::wrn(intern::mod,
            "Removing shared memory object"
        );
        ret = shm_unlink(name.c_str());
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to remove shared memory object ",
                "(", std::strerror(errno), ")"
            );
        }
        logging::err(intern::mod,
            "Failed to create shared memory object"
        );
        intern::fail = true;
        return;
    }

    logging::inf(intern::mod,
        "Closing shared memory object"
    );

    ret = ::close(desc);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to close shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::wrn(intern::mod,
            "Removing shared memory object"
        );
        ret = shm_unlink(name.c_str());
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to remove shared memory object ",
                "(", std::strerror(errno), ")"
            );
        }
        logging::err(intern::mod,
            "Failed to create shared memory object"
        );
        intern::fail = true;
        return;
    }

    intern::fail = false;
}

template <>
void create<double> (std::string name) {
    int ret, desc;
    std::size_t len;

    len = sizeof(double);

    logging::inf(intern::mod,
        "Creating shared memory object: ",
        "Name: “", name, "”, Type: Real scalar, Size: ", len
    );

    logging::inf(intern::mod,
        "Opening shared memory object"
    );

    desc = shm_open(name.c_str(), O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (desc < 0) {
        logging::err(intern::mod,
            "Failed to open shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::err(intern::mod,
            "Failed to create shared memory object"
        );
        intern::fail = true;
        return;
    }

    logging::inf(intern::mod,
        "Resizing shared memory object"
    );

    ret = ftruncate(desc, len);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to resize shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::wrn(intern::mod,
            "Closing shared memory object"
        );
        ret = ::close(desc);
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to close shared memory object ",
                "(", std::strerror(errno), ")"
            );
        }
        logging::wrn(intern::mod,
            "Removing shared memory object"
        );
        ret = shm_unlink(name.c_str());
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to remove shared memory object ",
                "(", std::strerror(errno), ")"
            );
        }
        logging::err(intern::mod,
            "Failed to create shared memory object"
        );
        intern::fail = true;
        return;
    }

    logging::inf(intern::mod,
        "Closing shared memory object"
    );

    ret = ::close(desc);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to close shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::wrn(intern::mod,
            "Removing shared memory object"
        );
        ret = shm_unlink(name.c_str());
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to remove shared memory object ",
                "(", std::strerror(errno), ")"
            );
        }
        logging::err(intern::mod,
            "Failed to create shared memory object"
        );
        intern::fail = true;
        return;
    }

    intern::fail = false;
}

template <>
void create<linalg::ivector> (std::string name, int dim) {
    int ret, desc;
    std::size_t len;

    len = dim * sizeof(int);

    logging::inf(intern::mod,
        "Creating shared memory object: ",
        "Name: “", name, "”, Type: Integer vector, Dim: ", dim, ", Size: ", len
    );

    if (!(dim > 0)) {
        logging::err(intern::mod,
            "Failed to create shared memory object (Invalid dimension)"
        );
        intern::fail = true;
        return;
    }

    logging::inf(intern::mod,
        "Opening shared memory object"
    );

    desc = shm_open(name.c_str(), O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (desc < 0) {
        logging::err(intern::mod,
            "Failed to open shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::err(intern::mod,
            "Failed to create shared memory object"
        );
        intern::fail = true;
        return;
    }

    logging::inf(intern::mod,
        "Resizing shared memory object"
    );

    ret = ftruncate(desc, len);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to resize shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::wrn(intern::mod,
            "Closing shared memory object"
        );
        ret = ::close(desc);
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to close shared memory object ",
                "(", std::strerror(errno), ")"
            );
        }
        logging::wrn(intern::mod,
            "Removing shared memory object"
        );
        ret = shm_unlink(name.c_str());
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to remove shared memory object ",
                "(", std::strerror(errno), ")"
            );
        }
        logging::err(intern::mod,
            "Failed to create shared memory object"
        );
        intern::fail = true;
        return;
    }

    logging::inf(intern::mod,
        "Closing shared memory object"
    );

    ret = ::close(desc);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to close shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::wrn(intern::mod,
            "Removing shared memory object"
        );
        ret = shm_unlink(name.c_str());
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to remove shared memory object ",
                "(", std::strerror(errno), ")"
            );
        }
        logging::err(intern::mod,
            "Failed to create shared memory object"
        );
        intern::fail = true;
        return;
    }

    intern::fail = false;
}

template <>
void create<linalg::fvector> (std::string name, int dim) {
    int ret, desc;
    std::size_t len;

    len = dim * sizeof(double);

    logging::inf(intern::mod,
        "Creating shared memory object: ",
        "Name: “", name, "”, Type: Real vector, Dim: ", dim, ", Size: ", len
    );

    if (!(dim > 0)) {
        logging::err(intern::mod,
            "Failed to create shared memory object (Invalid dimension)"
        );
        intern::fail = true;
        return;
    }

    logging::inf(intern::mod,
        "Opening shared memory object"
    );

    desc = shm_open(name.c_str(), O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (desc < 0) {
        logging::err(intern::mod,
            "Failed to open shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::err(intern::mod,
            "Failed to create shared memory object"
        );
        intern::fail = true;
        return;
    }

    logging::inf(intern::mod,
        "Resizing shared memory object"
    );

    ret = ftruncate(desc, len);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to resize shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::wrn(intern::mod,
            "Closing shared memory object"
        );
        ret = ::close(desc);
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to close shared memory object ",
                "(", std::strerror(errno), ")"
            );
        }
        logging::wrn(intern::mod,
            "Removing shared memory object"
        );
        ret = shm_unlink(name.c_str());
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to remove shared memory object ",
                "(", std::strerror(errno), ")"
            );
        }
        logging::err(intern::mod,
            "Failed to create shared memory object"
        );
        intern::fail = true;
        return;
    }

    logging::inf(intern::mod,
        "Closing shared memory object"
    );

    ret = ::close(desc);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to close shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::wrn(intern::mod,
            "Removing shared memory object"
        );
        ret = shm_unlink(name.c_str());
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to remove shared memory object ",
                "(", std::strerror(errno), ")"
            );
        }
        logging::err(intern::mod,
            "Failed to create shared memory object"
        );
        intern::fail = true;
        return;
    }

    intern::fail = false;
}

template <>
void create<linalg::imatrix> (std::string name, int rows, int cols) {
    int ret, desc;
    std::size_t len;

    len = rows * cols * sizeof(int);

    logging::inf(intern::mod,
        "Creating shared memory object: ",
        "Name: “", name, "”, Type: Integer matrix, ",
        "Rows: ", rows, ", Cols: ", cols, ", Size: ", len
    );

    if (!(rows > 0 && cols > 0)) {
        logging::err(intern::mod,
            "Failed to create shared memory object (Invalid dimensions)"
        );
        intern::fail = true;
        return;
    }

    logging::inf(intern::mod,
        "Opening shared memory object"
    );

    desc = shm_open(name.c_str(), O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (desc < 0) {
        logging::err(intern::mod,
            "Failed to open shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::err(intern::mod,
            "Failed to create shared memory object"
        );
        intern::fail = true;
        return;
    }

    logging::inf(intern::mod,
        "Resizing shared memory object"
    );

    ret = ftruncate(desc, len);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to resize shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::wrn(intern::mod,
            "Closing shared memory object"
        );
        ret = ::close(desc);
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to close shared memory object ",
                "(", std::strerror(errno), ")"
            );
        }
        logging::wrn(intern::mod,
            "Removing shared memory object"
        );
        ret = shm_unlink(name.c_str());
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to remove shared memory object ",
                "(", std::strerror(errno), ")"
            );
        }
        logging::err(intern::mod,
            "Failed to create shared memory object"
        );
        intern::fail = true;
        return;
    }

    logging::inf(intern::mod,
        "Closing shared memory object"
    );

    ret = ::close(desc);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to close shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::wrn(intern::mod,
            "Removing shared memory object"
        );
        ret = shm_unlink(name.c_str());
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to remove shared memory object ",
                "(", std::strerror(errno), ")"
            );
        }
        logging::err(intern::mod,
            "Failed to create shared memory object"
        );
        intern::fail = true;
        return;
    }

    intern::fail = false;
}

template <>
void create<linalg::fmatrix> (std::string name, int rows, int cols) {
    int ret, desc;
    std::size_t len;

    len = rows * cols * sizeof(double);

    logging::inf(intern::mod,
        "Creating shared memory object: ",
        "Name: “", name, "”, Type: Real matrix, ",
        "Rows: ", rows, ", Cols: ", cols, ", Size: ", len
    );

    if (!(rows > 0 && cols > 0)) {
        logging::err(intern::mod,
            "Failed to create shared memory object (Invalid dimensions)"
        );
        intern::fail = true;
        return;
    }

    logging::inf(intern::mod,
        "Opening shared memory object"
    );

    desc = shm_open(name.c_str(), O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (desc < 0) {
        logging::err(intern::mod,
            "Failed to open shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::err(intern::mod,
            "Failed to create shared memory object"
        );
        intern::fail = true;
        return;
    }

    logging::inf(intern::mod,
        "Resizing shared memory object"
    );

    ret = ftruncate(desc, len);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to resize shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::wrn(intern::mod,
            "Closing shared memory object"
        );
        ret = ::close(desc);
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to close shared memory object ",
                "(", std::strerror(errno), ")"
            );
        }
        logging::wrn(intern::mod,
            "Removing shared memory object"
        );
        ret = shm_unlink(name.c_str());
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to remove shared memory object ",
                "(", std::strerror(errno), ")"
            );
        }
        logging::err(intern::mod,
            "Failed to create shared memory object"
        );
        intern::fail = true;
        return;
    }

    logging::inf(intern::mod,
        "Closing shared memory object"
    );

    ret = ::close(desc);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to close shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::wrn(intern::mod,
            "Removing shared memory object"
        );
        ret = shm_unlink(name.c_str());
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to remove shared memory object ",
                "(", std::strerror(errno), ")"
            );
        }
        logging::err(intern::mod,
            "Failed to create shared memory object"
        );
        intern::fail = true;
        return;
    }

    intern::fail = false;
}

void remove (std::string name) {
    int ret;

    logging::inf(intern::mod,
        "Removing shared memory object: Name: “", name, "”"
    );

    ret = shm_unlink(name.c_str());
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to remove shared memory object ",
            "(", std::strerror(errno), ")"
        );
        intern::fail = true;
        return;
    }

    intern::fail = false;
}

int open (std::string name) {
    int ret, desc, id = intern::data.size() + 1;
    std::size_t len;
    void * data;
    struct stat fdat;

    logging::inf(intern::mod,
        "Mapping shared memory object #", id, ": Name: “", name, "”"
    );

    logging::inf(intern::mod,
        "Opening shared memory object"
    );

    desc = shm_open(name.c_str(), O_RDWR, S_IRUSR | S_IWUSR);
    if (desc < 0) {
        logging::err(intern::mod,
            "Failed to open shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::err(intern::mod,
            "Failed to map shared memory object #", id
        );
        intern::fail = true;
        return id;
    }

    logging::inf(intern::mod,
        "Getting size of shared memory object"
    );

    ret = fstat(desc, &fdat);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to get size of shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::wrn(intern::mod,
            "Closing shared memory object"
        );
        ret = ::close(desc);
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to close shared memory object ",
                "(", std::strerror(errno), ")"
            );
        }
        logging::err(intern::mod,
            "Failed to map shared memory object #", id
        );
        intern::fail = true;
        return id;
    }

    len = fdat.st_size;

    logging::inf(intern::mod,
        "Got size of shared memory object: Size: ", len
    );

    logging::inf(intern::mod,
        "Creating shared memory map"
    );

    data = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, desc, 0);
    if (data == MAP_FAILED) {
        logging::err(intern::mod,
            "Failed to create shared memory map ",
            "(", std::strerror(errno), ")"
        );
        logging::wrn(intern::mod,
            "Closing shared memory object"
        );
        ret = ::close(desc);
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to close shared memory object ",
                "(", std::strerror(errno), ")"
            );
        }
        logging::err(intern::mod,
            "Failed to map shared memory object #", id
        );
        intern::fail = true;
        return id;
    }

    logging::inf(intern::mod,
        "Closing shared memory object"
    );

    ret = ::close(desc);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to close shared memory object ",
            "(", std::strerror(errno), ")"
        );
        logging::wrn(intern::mod,
            "Removing shared memory map"
        );
        ret = munmap(data, len);
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to remove shared memory map ",
                "(", std::strerror(errno), ")"
            );
        }
        logging::err(intern::mod,
            "Failed to map shared memory object #", id
        );
        intern::fail = true;
        return id;
    }

    intern::len.push_back(len);
    intern::data.push_back(data);

    intern::fail = false;
    return id;
}

void close (void) {
    int ret;

    intern::fail = false;
    for (auto i = 0; i < intern::data.size(); i++) {
        logging::inf(intern::mod,
            "Unmapping shared memory object #", i + 1
        );
        logging::inf(intern::mod,
            "Removing shared memory map"
        );
        ret = munmap(intern::data[i], intern::len[i]);
        if (ret < 0) {
            logging::err(intern::mod,
                "Failed to remove shared memory map ",
                "(", std::strerror(errno), ")"
            );
            logging::err(intern::mod,
                "Failed to unmap shared memory object #", i + 1
            );
            intern::fail = true;
        }
    }

    intern::len.clear();
    intern::data.clear();
}

void put (int id, bool val) {
    std::size_t len;
    std::string oupt;

    len = sizeof(bool);

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Writing to shared memory object #", id, ": ",
        "Type: Boolean, Size: ", len, ", Value: ", oupt
    );

    if (!(id > 0 && id <= intern::data.size())) {
        logging::err(intern::mod,
            "Failed to write to shared memory object #", id, " ",
            "(Shared memory object not found)"
        );
        intern::fail = true;
        return;
    }

    if (!(intern::len[id - 1] == len)) {
        logging::err(intern::mod,
            "Failed to write to shared memory object #", id, " ",
            "(Incorrect size)"
        );
        intern::fail = true;
        return;
    }

    *((bool *)(intern::data[id - 1])) = val;

    intern::fail = false;
}

void put (int id, int val) {
    std::size_t len;
    std::string oupt;

    len = sizeof(int);

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Writing to shared memory object #", id, ": ",
        "Type: Integer scalar, Size: ", len, ", Value: ", oupt
    );

    if (!(id > 0 && id <= intern::data.size())) {
        logging::err(intern::mod,
            "Failed to write to shared memory object #", id, " ",
            "(Shared memory object not found)"
        );
        intern::fail = true;
        return;
    }

    if (!(intern::len[id - 1] == len)) {
        logging::err(intern::mod,
            "Failed to write to shared memory object #", id, " ",
            "(Incorrect size)"
        );
        intern::fail = true;
        return;
    }

    *((int *)(intern::data[id - 1])) = val;

    intern::fail = false;
}

void put (int id, double val) {
    std::size_t len;
    std::string oupt;

    len = sizeof(double);

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Writing to shared memory object #", id, ": ",
        "Type: Real scalar, Size: ", len, ", Value: ", oupt
    );

    if (!(id > 0 && id <= intern::data.size())) {
        logging::err(intern::mod,
            "Failed to write to shared memory object #", id, " ",
            "(Shared memory object not found)"
        );
        intern::fail = true;
        return;
    }

    if (!(intern::len[id - 1] == len)) {
        logging::err(intern::mod,
            "Failed to write to shared memory object #", id, " ",
            "(Incorrect size)"
        );
        intern::fail = true;
        return;
    }

    *((double *)(intern::data[id - 1])) = val;

    intern::fail = false;
}

void put (int id, linalg::ivector val) {
    int dim = linalg::dim(val);
    std::size_t len;
    std::string oupt;

    len = dim * sizeof(int);

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Writing to shared memory object #", id, ": ",
        "Type: Integer vector, Dim: ", dim, ", Size: ", len, ", Value: ", oupt
    );

    if (!(id > 0 && id <= intern::data.size())) {
        logging::err(intern::mod,
            "Failed to write to shared memory object #", id, " ",
            "(Shared memory object not found)"
        );
        intern::fail = true;
        return;
    }

    if (!(dim > 0)) {
        logging::err(intern::mod,
            "Failed to write to shared memory object #", id, " ",
            "(Invalid dimension)"
        );
        intern::fail = true;
        return;
    }

    if (!(intern::len[id - 1] == len)) {
        logging::err(intern::mod,
            "Failed to write to shared memory object #", id, " ",
            "(Incorrect size)"
        );
        intern::fail = true;
        return;
    }

    for (int i = 0; i < dim; i++) {
        ((int *)(intern::data[id - 1]))[i] = val[i];
    }

    intern::fail = false;
}

void put (int id, linalg::fvector val) {
    int dim = linalg::dim(val);
    std::size_t len;
    std::string oupt;

    len = dim * sizeof(double);

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Writing to shared memory object #", id, ": ",
        "Type: Real vector, Dim: ", dim, ", Size: ", len, ", Value: ", oupt
    );

    if (!(id > 0 && id <= intern::data.size())) {
        logging::err(intern::mod,
            "Failed to write to shared memory object #", id, " ",
            "(Shared memory object not found)"
        );
        intern::fail = true;
        return;
    }

    if (!(dim > 0)) {
        logging::err(intern::mod,
            "Failed to write to shared memory object #", id, " ",
            "(Invalid dimension)"
        );
        intern::fail = true;
        return;
    }

    if (!(intern::len[id - 1] == len)) {
        logging::err(intern::mod,
            "Failed to write to shared memory object #", id, " ",
            "(Incorrect size)"
        );
        intern::fail = true;
        return;
    }

    for (int i = 0; i < dim; i++) {
        ((double *)(intern::data[id - 1]))[i] = val[i];
    }

    intern::fail = false;
}

void put (int id, linalg::imatrix val) {
    int rows = linalg::rows(val), cols = linalg::cols(val);
    std::size_t len;
    std::string oupt;

    len = rows * cols * sizeof(int);

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Writing to shared memory object #", id, ": ",
        "Type: Integer matrix, Rows: ", rows, ", Cols: ", cols, ", ",
        "Size: ", len, ", Value: ", oupt
    );

    if (!(id > 0 && id <= intern::data.size())) {
        logging::err(intern::mod,
            "Failed to write to shared memory object #", id, " ",
            "(Shared memory object not found)"
        );
        intern::fail = true;
        return;
    }

    if (!(rows > 0 && cols > 0)) {
        logging::err(intern::mod,
            "Failed to write to shared memory object #", id, " ",
            "(Invalid dimensions)"
        );
        intern::fail = true;
        return;
    }

    if (!(intern::len[id - 1] == len)) {
        logging::err(intern::mod,
            "Failed to write to shared memory object #", id, " ",
            "(Incorrect size)"
        );
        intern::fail = true;
        return;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            ((int *)(intern::data[id - 1]))[i * cols + j] = val[i][j];
        }
    }

    intern::fail = false;
}

void put (int id, linalg::fmatrix val) {
    int rows = linalg::rows(val), cols = linalg::cols(val);
    std::size_t len;
    std::string oupt;

    len = rows * cols * sizeof(double);

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Writing to shared memory object #", id, ": ",
        "Type: Real matrix, Rows: ", rows, ", Cols: ", cols, ", ",
        "Size: ", len, ", Value: ", oupt
    );

    if (!(id > 0 && id <= intern::data.size())) {
        logging::err(intern::mod,
            "Failed to write to shared memory object #", id, " ",
            "(Shared memory object not found)"
        );
        intern::fail = true;
        return;
    }

    if (!(rows > 0 && cols > 0)) {
        logging::err(intern::mod,
            "Failed to write to shared memory object #", id, " ",
            "(Invalid dimensions)"
        );
        intern::fail = true;
        return;
    }

    if (!(intern::len[id - 1] == len)) {
        logging::err(intern::mod,
            "Failed to write to shared memory object #", id, " ",
            "(Incorrect size)"
        );
        intern::fail = true;
        return;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            ((double *)(intern::data[id - 1]))[i * cols + j] = val[i][j];
        }
    }

    intern::fail = false;
}

template <>
bool get<bool> (int id) {
    std::size_t len;
    bool val;
    std::string oupt;

    len = sizeof(bool);

    logging::inf(intern::mod,
        "Reading from shared memory object #", id, ": ",
        "Type: Boolean, Size: ", len
    );

    if (!(id > 0 && id <= intern::data.size())) {
        logging::err(intern::mod,
            "Failed to read from shared memory object #", id, " ",
            "(Shared memory object not found)"
        );
        intern::fail = true;
        return val;
    }

    if (!(intern::len[id - 1] == len)) {
        logging::err(intern::mod,
            "Failed to read from shared memory object #", id, " ",
            "(Incorrect size)"
        );
        intern::fail = true;
        return val;
    }

    val = *((bool *)(intern::data[id - 1]));

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Read from shared memory object #", id, ": Value: ", oupt
    );

    intern::fail = false;
    return val;
}

template <>
int get<int> (int id) {
    std::size_t len;
    int val;
    std::string oupt;

    len = sizeof(int);

    logging::inf(intern::mod,
        "Reading from shared memory object #", id, ": ",
        "Type: Integer scalar, Size: ", len
    );

    if (!(id > 0 && id <= intern::data.size())) {
        logging::err(intern::mod,
            "Failed to read from shared memory object #", id, " ",
            "(Shared memory object not found)"
        );
        intern::fail = true;
        return val;
    }

    if (!(intern::len[id - 1] == len)) {
        logging::err(intern::mod,
            "Failed to read from shared memory object #", id, " ",
            "(Incorrect size)"
        );
        intern::fail = true;
        return val;
    }

    val = *((int *)(intern::data[id - 1]));

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Read from shared memory object #", id, ": Value: ", oupt
    );

    intern::fail = false;
    return val;
}

template <>
double get<double> (int id) {
    std::size_t len;
    double val;
    std::string oupt;

    len = sizeof(double);

    logging::inf(intern::mod,
        "Reading from shared memory object #", id, ": ",
        "Type: Real scalar, Size: ", len
    );

    if (!(id > 0 && id <= intern::data.size())) {
        logging::err(intern::mod,
            "Failed to read from shared memory object #", id, " ",
            "(Shared memory object not found)"
        );
        intern::fail = true;
        return val;
    }

    if (!(intern::len[id - 1] == len)) {
        logging::err(intern::mod,
            "Failed to read from shared memory object #", id, " ",
            "(Incorrect size)"
        );
        intern::fail = true;
        return val;
    }

    val = *((double *)(intern::data[id - 1]));

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Read from shared memory object #", id, ": Value: ", oupt
    );

    intern::fail = false;
    return val;
}

template <>
linalg::ivector get<linalg::ivector> (int id, int dim) {
    std::size_t len;
    linalg::ivector val;
    std::string oupt;

    len = dim * sizeof(int);

    logging::inf(intern::mod,
        "Reading from shared memory object #", id, ": ",
        "Type: Integer vector, Dim: ", dim, ", Size: ", len
    );

    if (!(id > 0 && id <= intern::data.size())) {
        logging::err(intern::mod,
            "Failed to read from shared memory object #", id, " ",
            "(Shared memory object not found)"
        );
        intern::fail = true;
        return val;
    }

    if (!(dim > 0)) {
        logging::err(intern::mod,
            "Failed to read from shared memory object #", id, " ",
            "(Invalid dimension)"
        );
        intern::fail = true;
        return val;
    }

    if (!(intern::len[id - 1] == len)) {
        logging::err(intern::mod,
            "Failed to read from shared memory object #", id, " ",
            "(Incorrect size)"
        );
        intern::fail = true;
        return val;
    }

    linalg::resize(&val, dim);
    for (int i = 0; i < dim; i++) {
        val[i] = ((int *)(intern::data[id - 1]))[i];
    }

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Read from shared memory object #", id, ": Value: ", oupt
    );

    intern::fail = false;
    return val;
}

template <>
linalg::fvector get<linalg::fvector> (int id, int dim) {
    std::size_t len;
    linalg::fvector val;
    std::string oupt;

    len = dim * sizeof(double);

    logging::inf(intern::mod,
        "Reading from shared memory object #", id, ": ",
        "Type: Real vector, Dim: ", dim, ", Size: ", len
    );

    if (!(id > 0 && id <= intern::data.size())) {
        logging::err(intern::mod,
            "Failed to read from shared memory object #", id, " ",
            "(Shared memory object not found)"
        );
        intern::fail = true;
        return val;
    }

    if (!(dim > 0)) {
        logging::err(intern::mod,
            "Failed to read from shared memory object #", id, " ",
            "(Invalid dimension)"
        );
        intern::fail = true;
        return val;
    }

    if (!(intern::len[id - 1] == len)) {
        logging::err(intern::mod,
            "Failed to read from shared memory object #", id, " ",
            "(Incorrect size)"
        );
        intern::fail = true;
        return val;
    }

    linalg::resize(&val, dim);
    for (int i = 0; i < dim; i++) {
        val[i] = ((double *)(intern::data[id - 1]))[i];
    }

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Read from shared memory object #", id, ": Value: ", oupt
    );

    intern::fail = false;
    return val;
}

template <>
linalg::imatrix get<linalg::imatrix> (int id, int rows, int cols) {
    std::size_t len;
    linalg::imatrix val;
    std::string oupt;

    len = rows * cols * sizeof(int);

    logging::inf(intern::mod,
        "Reading from shared memory object #", id, ": ",
        "Type: Integer matrix, Rows: ", rows, ", Cols: ", cols, ", Size: ", len
    );

    if (!(id > 0 && id <= intern::data.size())) {
        logging::err(intern::mod,
            "Failed to read from shared memory object #", id, " ",
            "(Shared memory object not found)"
        );
        intern::fail = true;
        return val;
    }

    if (!(rows > 0 && cols > 0)) {
        logging::err(intern::mod,
            "Failed to read from shared memory object #", id, " ",
            "(Invalid dimensions)"
        );
        intern::fail = true;
        return val;
    }

    if (!(intern::len[id - 1] == len)) {
        logging::err(intern::mod,
            "Failed to read from shared memory object #", id, " ",
            "(Incorrect size)"
        );
        intern::fail = true;
        return val;
    }

    linalg::resize(&val, rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            val[i][j] = ((int *)(intern::data[id - 1]))[i * cols + j];
        }
    }

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Read from shared memory object #", id, ": Value: ", oupt
    );

    intern::fail = false;
    return val;
}

template <>
linalg::fmatrix get<linalg::fmatrix> (int id, int rows, int cols) {
    std::size_t len;
    linalg::fmatrix val;
    std::string oupt;

    len = rows * cols * sizeof(double);

    logging::inf(intern::mod,
        "Reading from shared memory object #", id, ": ",
        "Type: Real matrix, Rows: ", rows, ", Cols: ", cols, ", Size: ", len
    );

    if (!(id > 0 && id <= intern::data.size())) {
        logging::err(intern::mod,
            "Failed to read from shared memory object #", id, " ",
            "(Shared memory object not found)"
        );
        intern::fail = true;
        return val;
    }

    if (!(rows > 0 && cols > 0)) {
        logging::err(intern::mod,
            "Failed to read from shared memory object #", id, " ",
            "(Invalid dimensions)"
        );
        intern::fail = true;
        return val;
    }

    if (!(intern::len[id - 1] == len)) {
        logging::err(intern::mod,
            "Failed to read from shared memory object #", id, " ",
            "(Incorrect size)"
        );
        intern::fail = true;
        return val;
    }

    linalg::resize(&val, rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            val[i][j] = ((double *)(intern::data[id - 1]))[i * cols + j];
        }
    }

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Read from shared memory object #", id, ": Value: ", oupt
    );

    intern::fail = false;
    return val;
}

}

namespace sharedmem :: intern {

std::string conv_val_to_oupt (bool val) {
    std::string oupt;
    if (val) {
        oupt = "True";
    } else {
        oupt = "False";
    }
    return oupt;
}

std::string conv_val_to_oupt (int val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << std::showpos << val;
    oupt = ostr.str();

    return oupt;
}

std::string conv_val_to_oupt (double val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << std::showpos << std::scientific << std::setprecision(2) << val;
    oupt = ostr.str();

    return oupt;
}

std::string conv_val_to_oupt (linalg::ivector val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << "[";
    for (int i = 0; i < linalg::dim(val); i++) {
        ostr << std::showpos << val[i];
        if (i < linalg::dim(val) - 1) {
            ostr << ", ";
        }
    }
    ostr << "]";
    oupt = ostr.str();

    return oupt;
}

std::string conv_val_to_oupt (linalg::fvector val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << "[";
    for (int i = 0; i < linalg::dim(val); i++) {
        ostr << std::showpos << std::scientific << std::setprecision(2)
             << val[i];
        if (i < linalg::dim(val) - 1) {
            ostr << ", ";
        }
    }
    ostr << "]";
    oupt = ostr.str();

    return oupt;
}

std::string conv_val_to_oupt (linalg::imatrix val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << "[";
    for (int i = 0; i < linalg::rows(val); i++) {
        ostr << "[";
        for (int j = 0; j < linalg::cols(val); j++) {
            ostr << std::showpos << val[i][j];
            if (j < linalg::cols(val) - 1) {
                ostr << ", ";
            }
        }
        ostr << "]";
        if (i < linalg::rows(val) - 1) {
            ostr << ", ";
        }
    }
    ostr << "]";
    oupt = ostr.str();

    return oupt;
}

std::string conv_val_to_oupt (linalg::fmatrix val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << "[";
    for (int i = 0; i < linalg::rows(val); i++) {
        ostr << "[";
        for (int j = 0; j < linalg::cols(val); j++) {
            ostr << std::showpos << std::scientific << std::setprecision(2)
                 << val[i][j];
            if (j < linalg::cols(val) - 1) {
                ostr << ", ";
            }
        }
        ostr << "]";
        if (i < linalg::rows(val) - 1) {
            ostr << ", ";
        }
    }
    ostr << "]";
    oupt = ostr.str();

    return oupt;
}

}
