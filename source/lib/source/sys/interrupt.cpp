#include <cstring>
#include <cerrno>
#include <csignal>

#include <string>

#include <sys/logging.hpp>
#include <sys/interrupt.hpp>

namespace interrupt :: intern {

std::string mod = "interrupt";

bool fail;
volatile bool caught;

void handler (int signum);

}

namespace interrupt {

bool fail (void) {
    return intern::fail;
}

bool caught (void) {
    return intern::caught;
}

void init (void) {
    logging::inf(intern::mod,
        "Configuring interrupt signal handler"
    );

    intern::caught = false;

    if (std::signal(SIGINT, intern::handler) == SIG_ERR) {
        logging::err(intern::mod,
            "Failed to configure interrupt signal handler ",
            "(", std::strerror(errno), ")"
        );
        intern::fail = true;
        return;
    }

    intern::fail = false;
}

}

namespace interrupt :: intern {

void handler (int signum) {
    if (signum == SIGINT) {
        logging::inf(mod,
            "Caught interrupt signal"
        );
        caught = true;
    }
}

}
