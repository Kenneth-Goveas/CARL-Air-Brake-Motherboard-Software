#ifndef __DAT_CSVREAD_HPP__
#define __DAT_CSVREAD_HPP__

#include <string>
#include <vector>

#include <sys/logging.hpp>

namespace csvread {

class tracker {
    protected:
        static int count;
        static int instantiate (void);
};

class instance : private tracker {
    public:
        instance (const std::string & path);

        std::vector<std::string> get_head (void);
        std::vector<double> get_body (void);

        enum class except_ctor {fail};
        enum class except_get_head {fail};
        enum class except_get_body {fail, eof};

    private:
        logging::instance logging;
        bool init;
        int id;

        std::vector<std::string> head;
        std::vector<std::vector<double>> body;
        int idx;

        void split (const std::string & str, std::vector<std::string> & arr);
        void strip (std::string & str);

        void parse_head (const std::string & str, std::vector<std::string> & head);
        void parse_body (const std::string & str, std::vector<double> & body);

        enum class except_parse_head {fail};
        enum class except_parse_body {fail};

        class except_intern {};
};

}

#endif
