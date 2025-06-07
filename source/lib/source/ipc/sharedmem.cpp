#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cerrno>

#include <string>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <math/linalg.hpp>
#include <sys/logging.hpp>
#include <ipc/sharedmem.hpp>

namespace sharedmem {

int tracker::count = 0;

int tracker::instantiate (void) {
    tracker::count++;
    return tracker::count;
}

instance<std::uint8_t>::instance (const std::string & name, mode mode)
  : logging("sharedmem"), init(false), id(instance<std::uint8_t>::instantiate()), name(name), len(sizeof(std::uint8_t)),
    create(mode == mode::create || mode == mode::both), remove(mode == mode::remove || mode == mode::both) {

    bool obj_open = false, obj_map = false;
    int ret, desc;

    this->logging.inf("Initializing instance #", this->id, ": Type: Byte, Name: ", this->name);

    try {
        this->logging.inf("Opening shared memory object #", this->id);
        if (this->create) {
            desc = shm_open(this->name.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
        } else {
            desc = shm_open(this->name.c_str(), O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
        }
        if (desc < 0) {
            this->logging.err("Failed to open shared memory object #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }

        obj_open = true;

        this->logging.inf("Resizing shared memory object #", this->id);
        ret = ftruncate(desc, this->len);
        if (ret < 0) {
            this->logging.err("Failed to resize shared memory object #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }

        this->logging.inf("Mapping shared memory region #", this->id);
        this->data = (std::uint8_t *)mmap(NULL, this->len, PROT_READ | PROT_WRITE, MAP_SHARED, desc, 0);
        if (this->data == MAP_FAILED) {
            this->logging.err("Failed to map shared memory region #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }

        obj_map = true;

        obj_open = false;

        this->logging.inf("Closing shared memory object #", this->id);
        ret = close(desc);
        if (ret < 0) {
            this->logging.err("Failed to close shared memory object #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }
    } catch (except_intern) {
        if (obj_open) {
            this->logging.wrn("Closing shared memory object #", this->id);
            ret = close(desc);
            if (ret < 0) {
                this->logging.err("Failed to close shared memory object #", this->id, " (", std::strerror(errno), ")");
            }
        }

        if (obj_map) {
            this->logging.wrn("Unmapping shared memory region #", this->id);
            ret = munmap(this->data, this->len);
            if (ret < 0) {
                this->logging.err("Failed to unmap shared memory region #", this->id, " (", std::strerror(errno), ")");
            }
        }

        this->logging.err("Failed to initialize instance #", this->id);
        throw except_ctor::fail;
    }

    this->init = true;
}

instance<bool>::instance (const std::string & name, mode mode)
  : logging("sharedmem"), init(false), id(instance<bool>::instantiate()), name(name), len(sizeof(bool)),
    create(mode == mode::create || mode == mode::both), remove(mode == mode::remove || mode == mode::both) {

    bool obj_open = false, obj_map = false;
    int ret, desc;

    this->logging.inf("Initializing instance #", this->id, ": Type: Boolean, Name: ", this->name);

    try {
        this->logging.inf("Opening shared memory object #", this->id);
        if (this->create) {
            desc = shm_open(this->name.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
        } else {
            desc = shm_open(this->name.c_str(), O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
        }
        if (desc < 0) {
            this->logging.err("Failed to open shared memory object #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }

        obj_open = true;

        this->logging.inf("Resizing shared memory object #", this->id);
        ret = ftruncate(desc, this->len);
        if (ret < 0) {
            this->logging.err("Failed to resize shared memory object #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }

        this->logging.inf("Mapping shared memory region #", this->id);
        this->data = (bool *)mmap(NULL, this->len, PROT_READ | PROT_WRITE, MAP_SHARED, desc, 0);
        if (this->data == MAP_FAILED) {
            this->logging.err("Failed to map shared memory region #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }

        obj_map = true;

        obj_open = false;

        this->logging.inf("Closing shared memory object #", this->id);
        ret = close(desc);
        if (ret < 0) {
            this->logging.err("Failed to close shared memory object #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }
    } catch (except_intern) {
        if (obj_open) {
            this->logging.wrn("Closing shared memory object #", this->id);
            ret = close(desc);
            if (ret < 0) {
                this->logging.err("Failed to close shared memory object #", this->id, " (", std::strerror(errno), ")");
            }
        }

        if (obj_map) {
            this->logging.wrn("Unmapping shared memory region #", this->id);
            ret = munmap(this->data, this->len);
            if (ret < 0) {
                this->logging.err("Failed to unmap shared memory region #", this->id, " (", std::strerror(errno), ")");
            }
        }

        this->logging.err("Failed to initialize instance #", this->id);
        throw except_ctor::fail;
    }

    this->init = true;
}

instance<int>::instance (const std::string & name, mode mode)
  : logging("sharedmem"), init(false), id(instance<int>::instantiate()), name(name), len(sizeof(int)),
    create(mode == mode::create || mode == mode::both), remove(mode == mode::remove || mode == mode::both) {

    bool obj_open = false, obj_map = false;
    int ret, desc;

    this->logging.inf("Initializing instance #", this->id, ": Type: Integer scalar, Name: ", this->name);

    try {
        this->logging.inf("Opening shared memory object #", this->id);
        if (this->create) {
            desc = shm_open(this->name.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
        } else {
            desc = shm_open(this->name.c_str(), O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
        }
        if (desc < 0) {
            this->logging.err("Failed to open shared memory object #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }

        obj_open = true;

        this->logging.inf("Resizing shared memory object #", this->id);
        ret = ftruncate(desc, this->len);
        if (ret < 0) {
            this->logging.err("Failed to resize shared memory object #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }

        this->logging.inf("Mapping shared memory region #", this->id);
        this->data = (int *)mmap(NULL, this->len, PROT_READ | PROT_WRITE, MAP_SHARED, desc, 0);
        if (this->data == MAP_FAILED) {
            this->logging.err("Failed to map shared memory region #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }

        obj_map = true;

        obj_open = false;

        this->logging.inf("Closing shared memory object #", this->id);
        ret = close(desc);
        if (ret < 0) {
            this->logging.err("Failed to close shared memory object #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }
    } catch (except_intern) {
        if (obj_open) {
            this->logging.wrn("Closing shared memory object #", this->id);
            ret = close(desc);
            if (ret < 0) {
                this->logging.err("Failed to close shared memory object #", this->id, " (", std::strerror(errno), ")");
            }
        }

        if (obj_map) {
            this->logging.wrn("Unmapping shared memory region #", this->id);
            ret = munmap(this->data, this->len);
            if (ret < 0) {
                this->logging.err("Failed to unmap shared memory region #", this->id, " (", std::strerror(errno), ")");
            }
        }

        this->logging.err("Failed to initialize instance #", this->id);
        throw except_ctor::fail;
    }

    this->init = true;
}

instance<double>::instance (const std::string & name, mode mode)
  : logging("sharedmem"), init(false), id(instance<double>::instantiate()), name(name), len(sizeof(double)),
    create(mode == mode::create || mode == mode::both), remove(mode == mode::remove || mode == mode::both) {

    bool obj_open = false, obj_map = false;
    int ret, desc;

    this->logging.inf("Initializing instance #", this->id, ": Type: Real scalar, Name: ", this->name);

    try {
        this->logging.inf("Opening shared memory object #", this->id);
        if (this->create) {
            desc = shm_open(this->name.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
        } else {
            desc = shm_open(this->name.c_str(), O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
        }
        if (desc < 0) {
            this->logging.err("Failed to open shared memory object #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }

        obj_open = true;

        this->logging.inf("Resizing shared memory object #", this->id);
        ret = ftruncate(desc, this->len);
        if (ret < 0) {
            this->logging.err("Failed to resize shared memory object #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }

        this->logging.inf("Mapping shared memory region #", this->id);
        this->data = (double *)mmap(NULL, this->len, PROT_READ | PROT_WRITE, MAP_SHARED, desc, 0);
        if (this->data == MAP_FAILED) {
            this->logging.err("Failed to map shared memory region #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }

        obj_map = true;

        obj_open = false;

        this->logging.inf("Closing shared memory object #", this->id);
        ret = close(desc);
        if (ret < 0) {
            this->logging.err("Failed to close shared memory object #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }
    } catch (except_intern) {
        if (obj_open) {
            this->logging.wrn("Closing shared memory object #", this->id);
            ret = close(desc);
            if (ret < 0) {
                this->logging.err("Failed to close shared memory object #", this->id, " (", std::strerror(errno), ")");
            }
        }

        if (obj_map) {
            this->logging.wrn("Unmapping shared memory region #", this->id);
            ret = munmap(this->data, this->len);
            if (ret < 0) {
                this->logging.err("Failed to unmap shared memory region #", this->id, " (", std::strerror(errno), ")");
            }
        }

        this->logging.err("Failed to initialize instance #", this->id);
        throw except_ctor::fail;
    }

    this->init = true;
}

instance<linalg::ivector>::instance (const std::string & name, int dim, mode mode)
  : logging("sharedmem"), init(false), id(instance<linalg::ivector>::instantiate()), name(name), len(dim * sizeof(int)), dim(dim),
    create(mode == mode::create || mode == mode::both), remove(mode == mode::remove || mode == mode::both) {

    bool obj_open = false, obj_map = false;
    int ret, desc;

    this->logging.inf("Initializing instance #", this->id, ": Type: Integer vector, Dim: ", this->dim, ", Name: ", this->name);

    if (this->dim <= 0) {
        this->logging.err("Failed to initialize instance #", this->id, " (Invalid dimension)");
        throw except_ctor::fail;
    }

    try {
        this->logging.inf("Opening shared memory object #", this->id);
        if (this->create) {
            desc = shm_open(this->name.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
        } else {
            desc = shm_open(this->name.c_str(), O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
        }
        if (desc < 0) {
            this->logging.err("Failed to open shared memory object #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }

        obj_open = true;

        this->logging.inf("Resizing shared memory object #", this->id);
        ret = ftruncate(desc, this->len);
        if (ret < 0) {
            this->logging.err("Failed to resize shared memory object #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }

        this->logging.inf("Mapping shared memory region #", this->id);
        this->data = (int *)mmap(NULL, this->len, PROT_READ | PROT_WRITE, MAP_SHARED, desc, 0);
        if (this->data == MAP_FAILED) {
            this->logging.err("Failed to map shared memory region #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }

        obj_map = true;

        obj_open = false;

        this->logging.inf("Closing shared memory object #", this->id);
        ret = close(desc);
        if (ret < 0) {
            this->logging.err("Failed to close shared memory object #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }
    } catch (except_intern) {
        if (obj_open) {
            this->logging.wrn("Closing shared memory object #", this->id);
            ret = close(desc);
            if (ret < 0) {
                this->logging.err("Failed to close shared memory object #", this->id, " (", std::strerror(errno), ")");
            }
        }

        if (obj_map) {
            this->logging.wrn("Unmapping shared memory region #", this->id);
            ret = munmap(this->data, this->len);
            if (ret < 0) {
                this->logging.err("Failed to unmap shared memory region #", this->id, " (", std::strerror(errno), ")");
            }
        }

        this->logging.err("Failed to initialize instance #", this->id);
        throw except_ctor::fail;
    }

    this->init = true;
}

instance<linalg::fvector>::instance (const std::string & name, int dim, mode mode)
  : logging("sharedmem"), init(false), id(instance<linalg::fvector>::instantiate()), name(name), len(dim * sizeof(double)), dim(dim),
    create(mode == mode::create || mode == mode::both), remove(mode == mode::remove || mode == mode::both) {

    bool obj_open = false, obj_map = false;
    int ret, desc;

    this->logging.inf("Initializing instance #", this->id, ": Type: Real vector, Dim: ", this->dim, ", Name: ", this->name);

    if (this->dim <= 0) {
        this->logging.err("Failed to initialize instance #", this->id, " (Invalid dimension)");
        throw except_ctor::fail;
    }

    try {
        this->logging.inf("Opening shared memory object #", this->id);
        if (this->create) {
            desc = shm_open(this->name.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
        } else {
            desc = shm_open(this->name.c_str(), O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
        }
        if (desc < 0) {
            this->logging.err("Failed to open shared memory object #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }

        obj_open = true;

        this->logging.inf("Resizing shared memory object #", this->id);
        ret = ftruncate(desc, this->len);
        if (ret < 0) {
            this->logging.err("Failed to resize shared memory object #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }

        this->logging.inf("Mapping shared memory region #", this->id);
        this->data = (double *)mmap(NULL, this->len, PROT_READ | PROT_WRITE, MAP_SHARED, desc, 0);
        if (this->data == MAP_FAILED) {
            this->logging.err("Failed to map shared memory region #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }

        obj_map = true;

        obj_open = false;

        this->logging.inf("Closing shared memory object #", this->id);
        ret = close(desc);
        if (ret < 0) {
            this->logging.err("Failed to close shared memory object #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }
    } catch (except_intern) {
        if (obj_open) {
            this->logging.wrn("Closing shared memory object #", this->id);
            ret = close(desc);
            if (ret < 0) {
                this->logging.err("Failed to close shared memory object #", this->id, " (", std::strerror(errno), ")");
            }
        }

        if (obj_map) {
            this->logging.wrn("Unmapping shared memory region #", this->id);
            ret = munmap(this->data, this->len);
            if (ret < 0) {
                this->logging.err("Failed to unmap shared memory region #", this->id, " (", std::strerror(errno), ")");
            }
        }

        this->logging.err("Failed to initialize instance #", this->id);
        throw except_ctor::fail;
    }

    this->init = true;
}

instance<linalg::imatrix>::instance (const std::string & name, int rows, int cols, mode mode)
  : logging("sharedmem"), init(false), id(instance<linalg::imatrix>::instantiate()),
    name(name), len(rows * cols * sizeof(int)), rows(rows), cols(cols),
    create(mode == mode::create || mode == mode::both), remove(mode == mode::remove || mode == mode::both) {

    bool obj_open = false, obj_map = false;
    int ret, desc;

    this->logging.inf(
        "Initializing instance #", this->id, ": Type: Integer matrix, Rows: ", this->rows, ", Cols: ", this->cols, ", Name: ", this->name
    );

    if (this->rows <= 0 || this->cols <= 0) {
        this->logging.err("Failed to initialize instance #", this->id, " (Invalid dimensions)");
        throw except_ctor::fail;
    }

    try {
        this->logging.inf("Opening shared memory object #", this->id);
        if (this->create) {
            desc = shm_open(this->name.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
        } else {
            desc = shm_open(this->name.c_str(), O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
        }
        if (desc < 0) {
            this->logging.err("Failed to open shared memory object #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }

        obj_open = true;

        this->logging.inf("Resizing shared memory object #", this->id);
        ret = ftruncate(desc, this->len);
        if (ret < 0) {
            this->logging.err("Failed to resize shared memory object #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }

        this->logging.inf("Mapping shared memory region #", this->id);
        this->data = (int *)mmap(NULL, this->len, PROT_READ | PROT_WRITE, MAP_SHARED, desc, 0);
        if (this->data == MAP_FAILED) {
            this->logging.err("Failed to map shared memory region #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }

        obj_map = true;

        obj_open = false;

        this->logging.inf("Closing shared memory object #", this->id);
        ret = close(desc);
        if (ret < 0) {
            this->logging.err("Failed to close shared memory object #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }
    } catch (except_intern) {
        if (obj_open) {
            this->logging.wrn("Closing shared memory object #", this->id);
            ret = close(desc);
            if (ret < 0) {
                this->logging.err("Failed to close shared memory object #", this->id, " (", std::strerror(errno), ")");
            }
        }

        if (obj_map) {
            this->logging.wrn("Unmapping shared memory region #", this->id);
            ret = munmap(this->data, this->len);
            if (ret < 0) {
                this->logging.err("Failed to unmap shared memory region #", this->id, " (", std::strerror(errno), ")");
            }
        }

        this->logging.err("Failed to initialize instance #", this->id);
        throw except_ctor::fail;
    }

    this->init = true;
}

instance<linalg::fmatrix>::instance (const std::string & name, int rows, int cols, mode mode)
  : logging("sharedmem"), init(false), id(instance<linalg::fmatrix>::instantiate()),
    name(name), len(rows * cols * sizeof(double)), rows(rows), cols(cols),
    create(mode == mode::create || mode == mode::both), remove(mode == mode::remove || mode == mode::both) {

    bool obj_open = false, obj_map = false;
    int ret, desc;

    this->logging.inf(
        "Initializing instance #", this->id, ": Type: Real matrix, Rows: ", this->rows, ", Cols: ", this->cols, ", Name: ", this->name
    );

    if (this->rows <= 0 || this->cols <= 0) {
        this->logging.err("Failed to initialize instance #", this->id, " (Invalid dimensions)");
        throw except_ctor::fail;
    }

    try {
        this->logging.inf("Opening shared memory object #", this->id);
        if (this->create) {
            desc = shm_open(this->name.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
        } else {
            desc = shm_open(this->name.c_str(), O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
        }
        if (desc < 0) {
            this->logging.err("Failed to open shared memory object #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }

        obj_open = true;

        this->logging.inf("Resizing shared memory object #", this->id);
        ret = ftruncate(desc, this->len);
        if (ret < 0) {
            this->logging.err("Failed to resize shared memory object #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }

        this->logging.inf("Mapping shared memory region #", this->id);
        this->data = (double *)mmap(NULL, this->len, PROT_READ | PROT_WRITE, MAP_SHARED, desc, 0);
        if (this->data == MAP_FAILED) {
            this->logging.err("Failed to map shared memory region #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }

        obj_map = true;

        obj_open = false;

        this->logging.inf("Closing shared memory object #", this->id);
        ret = close(desc);
        if (ret < 0) {
            this->logging.err("Failed to close shared memory object #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }
    } catch (except_intern) {
        if (obj_open) {
            this->logging.wrn("Closing shared memory object #", this->id);
            ret = close(desc);
            if (ret < 0) {
                this->logging.err("Failed to close shared memory object #", this->id, " (", std::strerror(errno), ")");
            }
        }

        if (obj_map) {
            this->logging.wrn("Unmapping shared memory region #", this->id);
            ret = munmap(this->data, this->len);
            if (ret < 0) {
                this->logging.err("Failed to unmap shared memory region #", this->id, " (", std::strerror(errno), ")");
            }
        }

        this->logging.err("Failed to initialize instance #", this->id);
        throw except_ctor::fail;
    }

    this->init = true;
}

instance<std::uint8_t>::~instance (void) {
    int ret;

    if (this->init) {
        this->logging.inf("Unmapping shared memory region #", this->id);
        ret = munmap(this->data, this->len);
        if (ret < 0) {
            this->logging.err("Failed to unmap shared memory region #", this->id, " (", std::strerror(errno), ")");
        }

        if (this->remove) {
            this->logging.inf("Unlinking shared memory object #", this->id);
            ret = shm_unlink(this->name.c_str());
            if (ret < 0) {
                this->logging.err("Failed to unlink shared memory object #", this->id, " (", std::strerror(errno), ")");
            }
        }
    }
}

instance<bool>::~instance (void) {
    int ret;

    if (this->init) {
        this->logging.inf("Unmapping shared memory region #", this->id);
        ret = munmap(this->data, this->len);
        if (ret < 0) {
            this->logging.err("Failed to unmap shared memory region #", this->id, " (", std::strerror(errno), ")");
        }

        if (this->remove) {
            this->logging.inf("Unlinking shared memory object #", this->id);
            ret = shm_unlink(this->name.c_str());
            if (ret < 0) {
                this->logging.err("Failed to unlink shared memory object #", this->id, " (", std::strerror(errno), ")");
            }
        }
    }
}

instance<int>::~instance (void) {
    int ret;

    if (this->init) {
        this->logging.inf("Unmapping shared memory region #", this->id);
        ret = munmap(this->data, this->len);
        if (ret < 0) {
            this->logging.err("Failed to unmap shared memory region #", this->id, " (", std::strerror(errno), ")");
        }

        if (this->remove) {
            this->logging.inf("Unlinking shared memory object #", this->id);
            ret = shm_unlink(this->name.c_str());
            if (ret < 0) {
                this->logging.err("Failed to unlink shared memory object #", this->id, " (", std::strerror(errno), ")");
            }
        }
    }
}

instance<double>::~instance (void) {
    int ret;

    if (this->init) {
        this->logging.inf("Unmapping shared memory region #", this->id);
        ret = munmap(this->data, this->len);
        if (ret < 0) {
            this->logging.err("Failed to unmap shared memory region #", this->id, " (", std::strerror(errno), ")");
        }

        if (this->remove) {
            this->logging.inf("Unlinking shared memory object #", this->id);
            ret = shm_unlink(this->name.c_str());
            if (ret < 0) {
                this->logging.err("Failed to unlink shared memory object #", this->id, " (", std::strerror(errno), ")");
            }
        }
    }
}

instance<linalg::ivector>::~instance (void) {
    int ret;

    if (this->init) {
        this->logging.inf("Unmapping shared memory region #", this->id);
        ret = munmap(this->data, this->len);
        if (ret < 0) {
            this->logging.err("Failed to unmap shared memory region #", this->id, " (", std::strerror(errno), ")");
        }

        if (this->remove) {
            this->logging.inf("Unlinking shared memory object #", this->id);
            ret = shm_unlink(this->name.c_str());
            if (ret < 0) {
                this->logging.err("Failed to unlink shared memory object #", this->id, " (", std::strerror(errno), ")");
            }
        }
    }
}

instance<linalg::fvector>::~instance (void) {
    int ret;

    if (this->init) {
        this->logging.inf("Unmapping shared memory region #", this->id);
        ret = munmap(this->data, this->len);
        if (ret < 0) {
            this->logging.err("Failed to unmap shared memory region #", this->id, " (", std::strerror(errno), ")");
        }

        if (this->remove) {
            this->logging.inf("Unlinking shared memory object #", this->id);
            ret = shm_unlink(this->name.c_str());
            if (ret < 0) {
                this->logging.err("Failed to unlink shared memory object #", this->id, " (", std::strerror(errno), ")");
            }
        }
    }
}

instance<linalg::imatrix>::~instance (void) {
    int ret;

    if (this->init) {
        this->logging.inf("Unmapping shared memory region #", this->id);
        ret = munmap(this->data, this->len);
        if (ret < 0) {
            this->logging.err("Failed to unmap shared memory region #", this->id, " (", std::strerror(errno), ")");
        }

        if (this->remove) {
            this->logging.inf("Unlinking shared memory object #", this->id);
            ret = shm_unlink(this->name.c_str());
            if (ret < 0) {
                this->logging.err("Failed to unlink shared memory object #", this->id, " (", std::strerror(errno), ")");
            }
        }
    }
}

instance<linalg::fmatrix>::~instance (void) {
    int ret;

    if (this->init) {
        this->logging.inf("Unmapping shared memory region #", this->id);
        ret = munmap(this->data, this->len);
        if (ret < 0) {
            this->logging.err("Failed to unmap shared memory region #", this->id, " (", std::strerror(errno), ")");
        }

        if (this->remove) {
            this->logging.inf("Unlinking shared memory object #", this->id);
            ret = shm_unlink(this->name.c_str());
            if (ret < 0) {
                this->logging.err("Failed to unlink shared memory object #", this->id, " (", std::strerror(errno), ")");
            }
        }
    }
}

void instance<std::uint8_t>::put (std::uint8_t val) {
    this->logging.inf("Writing data to shared memory region #", this->id, ": Type: Byte, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to write data to shared memory region #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    *(this->data) = val;
}

void instance<bool>::put (bool val) {
    this->logging.inf("Writing data to shared memory region #", this->id, ": Type: Boolean, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to write data to shared memory region #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    *(this->data) = val;
}

void instance<int>::put (int val) {
    this->logging.inf("Writing data to shared memory region #", this->id, ": Type: Integer scalar, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to write data to shared memory region #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    *(this->data) = val;
}

void instance<double>::put (double val) {
    this->logging.inf("Writing data to shared memory region #", this->id, ": Type: Real scalar, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to write data to shared memory region #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    *(this->data) = val;
}

void instance<linalg::ivector>::put (const linalg::ivector & val) {
    this->logging.inf("Writing data to shared memory region #", this->id, ": Type: Integer vector, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to write data to shared memory region #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    if (linalg::dim(val) != this->dim) {
        this->logging.err("Failed to write data to shared memory region #", this->id, " (Incorrect dimension)");
        throw except_put::fail;
    }

    for (int i = 0; i < this->dim; i++) {
        this->data[i] = val[i];
    }
}

void instance<linalg::fvector>::put (const linalg::fvector & val) {
    this->logging.inf("Writing data to shared memory region #", this->id, ": Type: Real vector, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to write data to shared memory region #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    if (linalg::dim(val) != this->dim) {
        this->logging.err("Failed to write data to shared memory region #", this->id, " (Incorrect dimension)");
        throw except_put::fail;
    }

    for (int i = 0; i < this->dim; i++) {
        this->data[i] = val[i];
    }
}

void instance<linalg::imatrix>::put (const linalg::imatrix & val) {
    this->logging.inf("Writing data to shared memory region #", this->id, ": Type: Integer matrix, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to write data to shared memory region #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    if (linalg::rows(val) != this->rows || linalg::cols(val) != this->cols) {
        this->logging.err("Failed to write data to shared memory region #", this->id, " (Incorrect dimensions)");
        throw except_put::fail;
    }

    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            this->data[i * this->cols + j] = val[i][j];
        }
    }
}

void instance<linalg::fmatrix>::put (const linalg::fmatrix & val) {
    this->logging.inf("Writing data to shared memory region #", this->id, ": Type: Real matrix, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to write data to shared memory region #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    if (linalg::rows(val) != this->rows || linalg::cols(val) != this->cols) {
        this->logging.err("Failed to write data to shared memory region #", this->id, " (Incorrect dimensions)");
        throw except_put::fail;
    }

    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            this->data[i * this->cols + j] = val[i][j];
        }
    }
}

std::uint8_t instance<std::uint8_t>::get (void) {
    std::uint8_t val;

    this->logging.inf("Reading data from shared memory region #", this->id, ": Type: Byte");

    if (!this->init) {
        this->logging.err("Failed to read data from shared memory region #", this->id, " (Instance not initialized)");
        throw except_get::fail;
    }

    val = *(this->data);
    this->logging.inf("Read data from shared memory region #", this->id, ": Value: ", val);

    return val;
}

bool instance<bool>::get (void) {
    bool val;

    this->logging.inf("Reading data from shared memory region #", this->id, ": Type: Boolean");

    if (!this->init) {
        this->logging.err("Failed to read data from shared memory region #", this->id, " (Instance not initialized)");
        throw except_get::fail;
    }

    val = *(this->data);
    this->logging.inf("Read data from shared memory region #", this->id, ": Value: ", val);

    return val;
}

int instance<int>::get (void) {
    int val;

    this->logging.inf("Reading data from shared memory region #", this->id, ": Type: Integer scalar");

    if (!this->init) {
        this->logging.err("Failed to read data from shared memory region #", this->id, " (Instance not initialized)");
        throw except_get::fail;
    }

    val = *(this->data);
    this->logging.inf("Read data from shared memory region #", this->id, ": Value: ", val);

    return val;
}

double instance<double>::get (void) {
    double val;

    this->logging.inf("Reading data from shared memory region #", this->id, ": Type: Real scalar");

    if (!this->init) {
        this->logging.err("Failed to read data from shared memory region #", this->id, " (Instance not initialized)");
        throw except_get::fail;
    }

    val = *(this->data);
    this->logging.inf("Read data from shared memory region #", this->id, ": Value: ", val);

    return val;
}

linalg::ivector instance<linalg::ivector>::get (void) {
    linalg::ivector val;

    this->logging.inf("Reading data from shared memory region #", this->id, ": Type: Integer vector, Dim: ", this->dim);

    if (!this->init) {
        this->logging.err("Failed to read data from shared memory region #", this->id, " (Instance not initialized)");
        throw except_get::fail;
    }

    linalg::resize(val, this->dim);
    for (int i = 0; i < this->dim; i++) {
        val[i] = this->data[i];
    }

    this->logging.inf("Read data from shared memory region #", this->id, ": Value: ", val);

    return val;
}

linalg::fvector instance<linalg::fvector>::get (void) {
    linalg::fvector val;

    this->logging.inf("Reading data from shared memory region #", this->id, ": Type: Real vector, Dim: ", this->dim);

    if (!this->init) {
        this->logging.err("Failed to read data from shared memory region #", this->id, " (Instance not initialized)");
        throw except_get::fail;
    }

    linalg::resize(val, this->dim);
    for (int i = 0; i < this->dim; i++) {
        val[i] = this->data[i];
    }

    this->logging.inf("Read data from shared memory region #", this->id, ": Value: ", val);

    return val;
}

linalg::imatrix instance<linalg::imatrix>::get (void) {
    linalg::imatrix val;

    this->logging.inf(
        "Reading data from shared memory region #", this->id, ": Type: Integer matrix, Rows: ", this->rows, ", Cols: ", this->cols
    );

    if (!this->init) {
        this->logging.err("Failed to read data from shared memory region #", this->id, " (Instance not initialized)");
        throw except_get::fail;
    }

    linalg::resize(val, this->rows, this->cols);
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            val[i][j] = this->data[i * this->cols + j];
        }
    }

    this->logging.inf("Read data from shared memory region #", this->id, ": Value: ", val);

    return val;
}

linalg::fmatrix instance<linalg::fmatrix>::get (void) {
    linalg::fmatrix val;

    this->logging.inf(
        "Reading data from shared memory region #", this->id, ": Type: Real matrix, Rows: ", this->rows, ", Cols: ", this->cols
    );

    if (!this->init) {
        this->logging.err("Failed to read data from shared memory region #", this->id, " (Instance not initialized)");
        throw except_get::fail;
    }

    linalg::resize(val, this->rows, this->cols);
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            val[i][j] = this->data[i * this->cols + j];
        }
    }

    this->logging.inf("Read data from shared memory region #", this->id, ": Value: ", val);

    return val;
}

}
