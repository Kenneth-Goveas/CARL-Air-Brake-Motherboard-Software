#ifndef __IPC_SHAREDMEM_HPP__
#define __IPC_SHAREDMEM_HPP__

#include <cstddef>
#include <cstdint>

#include <string>

#include <math/linalg.hpp>
#include <sys/logging.hpp>

namespace sharedmem {

enum class mode {none, create, remove, both};

class tracker {
    protected:
        static int count;
        static int instantiate (void);
};

template <typename type>
class instance;

template <>
class instance<std::uint8_t> : private tracker {
    public:
        instance (const std::string & name, mode mode = mode::none);
        ~instance (void);

        void put (std::uint8_t val);
        std::uint8_t get (void);

        enum class except_ctor {fail};
        enum class except_put {fail};
        enum class except_get {fail};

    private:
        logging::instance logging;
        bool init;
        int id;

        std::uint8_t * data;
        std::string name;
        std::size_t len;
        bool create, remove;

        class except_intern {};
};

template <>
class instance<bool> : private tracker {
    public:
        instance (const std::string & name, mode mode = mode::none);
        ~instance (void);

        void put (bool val);
        bool get (void);

        enum class except_ctor {fail};
        enum class except_put {fail};
        enum class except_get {fail};

    private:
        logging::instance logging;
        bool init;
        int id;

        bool * data;
        std::string name;
        std::size_t len;
        bool create, remove;

        class except_intern {};
};

template <>
class instance<int> : private tracker {
    public:
        instance (const std::string & name, mode mode = mode::none);
        ~instance (void);

        void put (int val);
        int get (void);

        enum class except_ctor {fail};
        enum class except_put {fail};
        enum class except_get {fail};

    private:
        logging::instance logging;
        bool init;
        int id;

        int * data;
        std::string name;
        std::size_t len;
        bool create, remove;

        class except_intern {};
};

template <>
class instance<double> : private tracker {
    public:
        instance (const std::string & name, mode mode = mode::none);
        ~instance (void);

        void put (double val);
        double get (void);

        enum class except_ctor {fail};
        enum class except_put {fail};
        enum class except_get {fail};

    private:
        logging::instance logging;
        bool init;
        int id;

        double * data;
        std::string name;
        std::size_t len;
        bool create, remove;

        class except_intern {};
};

template <>
class instance<linalg::ivector> : private tracker {
    public:
        instance (const std::string & name, int dim, mode mode = mode::none);
        ~instance (void);

        void put (const linalg::ivector & val);
        linalg::ivector get (void);

        enum class except_ctor {fail};
        enum class except_put {fail};
        enum class except_get {fail};

    private:
        logging::instance logging;
        bool init;
        int id;

        int * data;
        std::string name;
        std::size_t len;
        int dim;
        bool create, remove;

        class except_intern {};
};

template <>
class instance<linalg::fvector> : private tracker {
    public:
        instance (const std::string & name, int dim, mode mode = mode::none);
        ~instance (void);

        void put (const linalg::fvector & val);
        linalg::fvector get (void);

        enum class except_ctor {fail};
        enum class except_put {fail};
        enum class except_get {fail};

    private:
        logging::instance logging;
        bool init;
        int id;

        double * data;
        std::string name;
        std::size_t len;
        int dim;
        bool create, remove;

        class except_intern {};
};

template <>
class instance<linalg::imatrix> : private tracker {
    public:
        instance (const std::string & name, int rows, int cols, mode mode = mode::none);
        ~instance (void);

        void put (const linalg::imatrix & val);
        linalg::imatrix get (void);

        enum class except_ctor {fail};
        enum class except_put {fail};
        enum class except_get {fail};

    private:
        logging::instance logging;
        bool init;
        int id;

        int * data;
        std::string name;
        std::size_t len;
        int rows, cols;
        bool create, remove;

        class except_intern {};
};

template <>
class instance<linalg::fmatrix> : private tracker {
    public:
        instance (const std::string & name, int rows, int cols, mode mode = mode::none);
        ~instance (void);

        void put (const linalg::fmatrix & val);
        linalg::fmatrix get (void);

        enum class except_ctor {fail};
        enum class except_put {fail};
        enum class except_get {fail};

    private:
        logging::instance logging;
        bool init;
        int id;

        double * data;
        std::string name;
        std::size_t len;
        int rows, cols;
        bool create, remove;

        class except_intern {};
};

}

#endif
