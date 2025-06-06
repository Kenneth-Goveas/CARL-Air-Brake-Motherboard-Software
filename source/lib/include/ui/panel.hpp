#ifndef __UI_PANEL_HPP__
#define __UI_PANEL_HPP__

#include <cstdint>

#include <string>
#include <vector>

#include <math/linalg.hpp>
#include <sys/logging.hpp>
#include <ui/terminal.hpp>
#include <ui/input.hpp>
#include <ui/output.hpp>

namespace panel {

class tracker {
    protected:
        static int count;
        static int instantiate (void);
};

class instance : private tracker {
    public:
        instance (
            terminal::instance & terminal, input::instance & input, output::instance & output,
            terminal::color color, int row, int col, int hgt, int wdt
        );

        void put (terminal::color color, int row, int col, const char * val);

        void put (terminal::color color, int row, int col, const std::string & val);
        void put (terminal::color color, int row, int col, std::uint8_t val);
        void put (terminal::color color, int row, int col, bool val);
        void put (terminal::color color, int row, int col, int val);
        void put (terminal::color color, int row, int col, double val);
        void put (terminal::color color, int row, int col, const linalg::ivector & val);
        void put (terminal::color color, int row, int col, const linalg::fvector & val);
        void put (terminal::color color, int row, int col, const linalg::imatrix & val);
        void put (terminal::color color, int row, int col, const linalg::fmatrix & val);

        void put (terminal::color color, int row, int col, const std::vector<std::string> & val);
        void put (terminal::color color, int row, int col, const std::vector<std::uint8_t> & val);
        void put (terminal::color color, int row, int col, const std::vector<bool> & val);
        void put (terminal::color color, int row, int col, const std::vector<int> & val);
        void put (terminal::color color, int row, int col, const std::vector<double> & val);
        void put (terminal::color color, int row, int col, const std::vector<linalg::ivector> & val);
        void put (terminal::color color, int row, int col, const std::vector<linalg::fvector> & val);
        void put (terminal::color color, int row, int col, const std::vector<linalg::imatrix> & val);
        void put (terminal::color color, int row, int col, const std::vector<linalg::fmatrix> & val);

        template <typename type>
        type get (terminal::color color, int row, int col);

        void redraw (void);

        enum class except_ctor {fail};
        enum class except_put {fail};
        enum class except_get {fail, eof};
        enum class except_redraw {fail};

    private:
        logging::instance logging;
        bool init;
        int id;

        terminal::instance * terminal;
        input::instance * input;
        output::instance * output;

        terminal::color color;
        int row, col, hgt, wdt;
};

template <>
std::string instance::get<std::string> (terminal::color color, int row, int col);

template <>
std::uint8_t instance::get<std::uint8_t> (terminal::color color, int row, int col);

template <>
bool instance::get<bool> (terminal::color color, int row, int col);

template <>
int instance::get<int> (terminal::color color, int row, int col);

template <>
double instance::get<double> (terminal::color color, int row, int col);

template <>
linalg::ivector instance::get<linalg::ivector> (terminal::color color, int row, int col);

template <>
linalg::fvector instance::get<linalg::fvector> (terminal::color color, int row, int col);

template <>
linalg::imatrix instance::get<linalg::imatrix> (terminal::color color, int row, int col);

template <>
linalg::fmatrix instance::get<linalg::fmatrix> (terminal::color color, int row, int col);

template <>
std::vector<std::string> instance::get<std::vector<std::string>> (terminal::color color, int row, int col);

template <>
std::vector<std::uint8_t> instance::get<std::vector<std::uint8_t>> (terminal::color color, int row, int col);

template <>
std::vector<bool> instance::get<std::vector<bool>> (terminal::color color, int row, int col);

template <>
std::vector<int> instance::get<std::vector<int>> (terminal::color color, int row, int col);

template <>
std::vector<double> instance::get<std::vector<double>> (terminal::color color, int row, int col);

template <>
std::vector<linalg::ivector> instance::get<std::vector<linalg::ivector>> (terminal::color color, int row, int col);

template <>
std::vector<linalg::fvector> instance::get<std::vector<linalg::fvector>> (terminal::color color, int row, int col);

template <>
std::vector<linalg::imatrix> instance::get<std::vector<linalg::imatrix>> (terminal::color color, int row, int col);

template <>
std::vector<linalg::fmatrix> instance::get<std::vector<linalg::fmatrix>> (terminal::color color, int row, int col);

}

#endif
