#ifndef __DAT_CSVWRITE_HPP__
#define __DAT_CSVWRITE_HPP__

#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <sys/logging.hpp>

namespace csvwrite {

class tracker {
    protected:
        static int count;
        static int instantiate (void);
};

class instance : private tracker {
    public:
        instance (const std::string & path);
        ~instance (void);

        void put_head (const std::vector<std::string> & head);
        void put_body (const std::vector<double> & body);

        enum class except_ctor {fail};
        enum class except_put_head {fail};
        enum class except_put_body {fail};

    private:
        logging::instance logging;
        bool init;
        int id;

        std::ofstream file;
        int dim;
};

}

#endif
