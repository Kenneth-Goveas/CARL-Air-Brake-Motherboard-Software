#include <string>
#include <vector>

#include <sys/logging.hpp>
#include <sys/option.hpp>

#include <ipc/channel.hpp>

namespace logging :: local {
    std::string path = "data/system.log";
    std::string unit = "memory";
    std::string mod = "main";
}

namespace option :: local {
    std::vector<std::string> args, names;
}

namespace channel :: local {
    std::string name_oper_create = "create", name_oper_remove = "remove";
    bool oper_create, oper_remove;
    int count_oper;

    std::string name_want_ctrl = "control", name_want_stat = "state", name_want_estm = "estimate", name_want_sens = "sensing";
    bool want_ctrl, want_stat, want_estm, want_sens;
    int count_want;
}

int main (int argc, char ** argv) {
    logging::init(logging::local::path, logging::local::unit);
    if (logging::fail()) {
        return 1;
    }

    logging::inf(logging::local::mod,
        "Starting"
    );

    for (int i = 1; i < argc; i++) {
        option::local::args.push_back(argv[i]);
    }

    option::local::names = {
        channel::local::name_oper_create, channel::local::name_oper_remove,
        channel::local::name_want_ctrl, channel::local::name_want_stat, channel::local::name_want_estm, channel::local::name_want_sens
    };

    option::init(option::local::args, option::local::names);
    if (option::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    channel::local::oper_create = option::get(channel::local::name_oper_create);
    if (option::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    channel::local::oper_remove = option::get(channel::local::name_oper_remove);
    if (option::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    channel::local::want_ctrl = option::get(channel::local::name_want_ctrl);
    if (option::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    channel::local::want_stat = option::get(channel::local::name_want_stat);
    if (option::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    channel::local::want_estm = option::get(channel::local::name_want_estm);
    if (option::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    channel::local::want_sens = option::get(channel::local::name_want_sens);
    if (option::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    channel::local::count_oper = 0;
    for (auto oper : {channel::local::oper_create, channel::local::oper_remove}) {
        if (oper) {
            channel::local::count_oper++;
        }
    }

    channel::local::count_want = 0;
    for (auto want : {channel::local::want_ctrl, channel::local::want_stat, channel::local::want_estm, channel::local::want_sens}) {
        if (want) {
            channel::local::count_want++;
        }
    }

    if (channel::local::count_oper != 1) {
        logging::err(logging::local::mod,
            "Exactly one operation must be selected"
        );
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    if (channel::local::count_want == 0) {
        logging::err(logging::local::mod,
            "At least one channel must be selected"
        );
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    channel::init(channel::local::want_ctrl, channel::local::want_stat, channel::local::want_estm, channel::local::want_sens);

    if (channel::local::oper_create) {
        channel::create();
        if (channel::fail()) {
            logging::wrn(logging::local::mod,
                "Exiting"
            );
            logging::deinit();
            return 1;
        }

        channel::open();
        if (channel::fail()) {
            channel::remove();
            logging::wrn(logging::local::mod,
                "Exiting"
            );
            logging::deinit();
            return 1;
        }

        channel::clear();
        if (channel::fail()) {
            channel::close();
            channel::remove();
            logging::wrn(logging::local::mod,
                "Exiting"
            );
            logging::deinit();
            return 1;
        }

        channel::close();
        if (channel::fail()) {
            channel::remove();
            logging::wrn(logging::local::mod,
                "Exiting"
            );
            logging::deinit();
            return 1;
        }
    }

    if (channel::local::oper_remove) {
        channel::remove();
        if (channel::fail()) {
            logging::wrn(logging::local::mod,
                "Exiting"
            );
            logging::deinit();
            return 1;
        }
    }

    logging::inf(logging::local::mod,
        "Exiting"
    );

    logging::deinit();

    return 0;
}
