#include <string>
#include <vector>

#include <sys/logging.hpp>
#include <sys/option.hpp>

#include <ipc/channel.hpp>

namespace logging::local {
    std::string path = "log/system.log";
    std::string app = "memory";
    std::string lib = "main";
}

namespace option::local {
    std::vector<std::string> names;
}

namespace channel::local {
    std::string mode_create_name = "create", mode_remove_name = "remove";
    bool mode_create, mode_remove;
    int mode_count = 0;
    channel::mode mode;
}

namespace local {
    class except {};
}

int main (int argc, char ** argv) {
    if (!logging::instance::start(logging::local::path, logging::local::app)) {
        return 1;
    }

    logging::instance logging(logging::local::lib);
    logging.inf("Starting");

    try {
        option::local::names = {channel::local::mode_create_name, channel::local::mode_remove_name};
        option::instance option(argc, argv, option::local::names);

        channel::local::mode_create = option.get(channel::local::mode_create_name);
        channel::local::mode_remove = option.get(channel::local::mode_remove_name);

        for (auto mode : {channel::local::mode_create, channel::local::mode_remove}) {
            if (mode) {
                channel::local::mode_count++;
            }
        }

        if (channel::local::mode_count != 1) {
            logging.err("Exactly one mode must be selected");
            throw local::except{};
        }

        if (channel::local::mode_create) {
            channel::local::mode = channel::mode::create;
        }

        if (channel::local::mode_remove) {
            channel::local::mode = channel::mode::remove;
        }

        channel::instance channel(channel::local::mode);

        channel.acquire();
        channel.clear();
        channel.release();
    } catch (...) {
        logging.wrn("Exiting");
        logging::instance::stop();
        return 1;
    }

    logging.inf("Exiting");
    logging::instance::stop();

    return 0;
}
