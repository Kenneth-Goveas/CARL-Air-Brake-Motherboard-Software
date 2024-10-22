#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <math/linalg.hpp>

#include <sys/logging.hpp>

#include <ipc/sema.hpp>
#include <ipc/shmem.hpp>
#include <ipc/channel.hpp>

namespace channel :: intern {

std::string mod = "channel";

bool fail;
bool blank;

bool req_ctrl, req_stat, req_estm, req_sens, req_updt;

int id_acces;
int id_ctrl_drdy, id_stat_drdy, id_estm_drdy, id_sens_drdy, id_updt_drdy;
int id_ctrl_data, id_stat_data, id_estm_data, id_sens_data, id_updt_data;

std::string conv_val_to_oupt (bool req);
std::string conv_val_to_oupt (double val);
std::string conv_val_to_oupt (linalg::fvector val);

}

namespace channel {

bool fail (void) {
    return intern::fail;
}

bool blank (void) {
    return intern::blank;
}

void init (
    bool req_ctrl, bool req_stat, bool req_estm, bool req_sens, bool req_updt
) {
    std::string oupt;

    oupt = intern::conv_val_to_oupt(req_ctrl);
    logging::inf(intern::mod,
        "Setting channel requirement: Type: Control, Req: ", oupt
    );

    intern::req_ctrl = req_ctrl;

    oupt = intern::conv_val_to_oupt(req_stat);
    logging::inf(intern::mod,
        "Setting channel requirement: Type: State, Req: ", oupt
    );

    intern::req_stat = req_stat;

    oupt = intern::conv_val_to_oupt(req_estm);
    logging::inf(intern::mod,
        "Setting channel requirement: Type: Estimate, Req: ", oupt
    );

    intern::req_estm = req_estm;

    oupt = intern::conv_val_to_oupt(req_sens);
    logging::inf(intern::mod,
        "Setting channel requirement: Type: Sensing, Req: ", oupt
    );

    intern::req_sens = req_sens;

    oupt = intern::conv_val_to_oupt(req_updt);
    logging::inf(intern::mod,
        "Setting channel requirement: Type: Update, Req: ", oupt
    );

    intern::req_updt = req_updt;
}

bool exists (void) {
    bool flag;

    logging::inf(intern::mod,
        "Checking if selected channel resources exist"
    );

    logging::inf(intern::mod,
        "Checking if access control semaphore exists"
    );

    flag = sema::exists("acces");
    if (sema::fail()) {
        logging::err(intern::mod,
            "Failed to check if access control semaphore exists"
        );
        logging::err(intern::mod,
            "Failed to check if selected channel resources exist"
        );
        intern::fail = true;
        return flag;
    }

    if (!flag) {
        logging::inf(intern::mod,
            "Access control semaphore does not exist"
        );
        logging::inf(intern::mod,
            "Selected channel resources do not exist"
        );
        flag = false;
        intern::fail = false;
        return flag;
    }

    logging::inf(intern::mod,
        "Access control semaphore exists"
    );

    if (intern::req_ctrl) {
        logging::inf(intern::mod,
            "Checking if channel status shared memory object exists: ",
            "Type: Control"
        );

        flag = shmem::exists("ctrl-drdy");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to check if channel status shared memory object exists"
            );
            logging::err(intern::mod,
                "Failed to check if selected channel resources exist"
            );
            intern::fail = true;
            return flag;
        }

        if (!flag) {
            logging::inf(intern::mod,
                "Channel status shared memory object does not exist"
            );
            logging::inf(intern::mod,
                "Selected channel resources do not exist"
            );
            flag = false;
            intern::fail = false;
            return flag;
        }

        logging::inf(intern::mod,
            "Channel status shared memory object exists"
        );

        logging::inf(intern::mod,
            "Checking size of channel status shared memory object: ",
            "Type: Control"
        );

        flag = shmem::fits<bool>("ctrl-drdy");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to check size of channel status shared memory object"
            );
            logging::err(intern::mod,
                "Failed to check if selected channel resources exist"
            );
            intern::fail = true;
            return flag;
        }

        if (!flag) {
            logging::inf(intern::mod,
                "Size of channel status shared memory object does not match"
            );
            logging::inf(intern::mod,
                "Selected channel resources do not exist"
            );
            flag = false;
            intern::fail = false;
            return flag;
        }

        logging::inf(intern::mod,
            "Size of channel status shared memory object matches"
        );

        logging::inf(intern::mod,
            "Checking if channel data shared memory object exists: ",
            "Type: Control"
        );

        flag = shmem::exists("ctrl-data");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to check if channel data shared memory object exists"
            );
            logging::err(intern::mod,
                "Failed to check if selected channel resources exist"
            );
            intern::fail = true;
            return flag;
        }

        if (!flag) {
            logging::inf(intern::mod,
                "Channel data shared memory object does not exist"
            );
            logging::inf(intern::mod,
                "Selected channel resources do not exist"
            );
            flag = false;
            intern::fail = false;
            return flag;
        }

        logging::inf(intern::mod,
            "Channel data shared memory object exists"
        );

        logging::inf(intern::mod,
            "Checking size of channel data shared memory object: ",
            "Type: Control"
        );

        flag = shmem::fits<double>("ctrl-data");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to check size of channel data shared memory object"
            );
            logging::err(intern::mod,
                "Failed to check if selected channel resources exist"
            );
            intern::fail = true;
            return flag;
        }

        if (!flag) {
            logging::inf(intern::mod,
                "Size of channel data shared memory object does not match"
            );
            logging::inf(intern::mod,
                "Selected channel resources do not exist"
            );
            flag = false;
            intern::fail = false;
            return flag;
        }

        logging::inf(intern::mod,
            "Size of channel data shared memory object matches"
        );
    }

    if (intern::req_stat) {
        logging::inf(intern::mod,
            "Checking if channel status shared memory object exists: ",
            "Type: State"
        );

        flag = shmem::exists("stat-drdy");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to check if channel status shared memory object exists"
            );
            logging::err(intern::mod,
                "Failed to check if selected channel resources exist"
            );
            intern::fail = true;
            return flag;
        }

        if (!flag) {
            logging::inf(intern::mod,
                "Channel status shared memory object does not exist"
            );
            logging::inf(intern::mod,
                "Selected channel resources do not exist"
            );
            flag = false;
            intern::fail = false;
            return flag;
        }

        logging::inf(intern::mod,
            "Channel status shared memory object exists"
        );

        logging::inf(intern::mod,
            "Checking size of channel status shared memory object: ",
            "Type: State"
        );

        flag = shmem::fits<bool>("stat-drdy");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to check size of channel status shared memory object"
            );
            logging::err(intern::mod,
                "Failed to check if selected channel resources exist"
            );
            intern::fail = true;
            return flag;
        }

        if (!flag) {
            logging::inf(intern::mod,
                "Size of channel status shared memory object does not match"
            );
            logging::inf(intern::mod,
                "Selected channel resources do not exist"
            );
            flag = false;
            intern::fail = false;
            return flag;
        }

        logging::inf(intern::mod,
            "Size of channel status shared memory object matches"
        );

        logging::inf(intern::mod,
            "Checking if channel data shared memory object exists: ",
            "Type: State"
        );

        flag = shmem::exists("stat-data");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to check if channel data shared memory object exists"
            );
            logging::err(intern::mod,
                "Failed to check if selected channel resources exist"
            );
            intern::fail = true;
            return flag;
        }

        if (!flag) {
            logging::inf(intern::mod,
                "Channel data shared memory object does not exist"
            );
            logging::inf(intern::mod,
                "Selected channel resources do not exist"
            );
            flag = false;
            intern::fail = false;
            return flag;
        }

        logging::inf(intern::mod,
            "Channel data shared memory object exists"
        );

        logging::inf(intern::mod,
            "Checking size of channel data shared memory object: ",
            "Type: State"
        );

        flag = shmem::fits<linalg::fvector>("stat-data", 7);
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to check size of channel data shared memory object"
            );
            logging::err(intern::mod,
                "Failed to check if selected channel resources exist"
            );
            intern::fail = true;
            return flag;
        }

        if (!flag) {
            logging::inf(intern::mod,
                "Size of channel data shared memory object does not match"
            );
            logging::inf(intern::mod,
                "Selected channel resources do not exist"
            );
            flag = false;
            intern::fail = false;
            return flag;
        }

        logging::inf(intern::mod,
            "Size of channel data shared memory object matches"
        );
    }

    if (intern::req_estm) {
        logging::inf(intern::mod,
            "Checking if channel status shared memory object exists: ",
            "Type: Estimate"
        );

        flag = shmem::exists("estm-drdy");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to check if channel status shared memory object exists"
            );
            logging::err(intern::mod,
                "Failed to check if selected channel resources exist"
            );
            intern::fail = true;
            return flag;
        }

        if (!flag) {
            logging::inf(intern::mod,
                "Channel status shared memory object does not exist"
            );
            logging::inf(intern::mod,
                "Selected channel resources do not exist"
            );
            flag = false;
            intern::fail = false;
            return flag;
        }

        logging::inf(intern::mod,
            "Channel status shared memory object exists"
        );

        logging::inf(intern::mod,
            "Checking size of channel status shared memory object: ",
            "Type: Estimate"
        );

        flag = shmem::fits<bool>("estm-drdy");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to check size of channel status shared memory object"
            );
            logging::err(intern::mod,
                "Failed to check if selected channel resources exist"
            );
            intern::fail = true;
            return flag;
        }

        if (!flag) {
            logging::inf(intern::mod,
                "Size of channel status shared memory object does not match"
            );
            logging::inf(intern::mod,
                "Selected channel resources do not exist"
            );
            flag = false;
            intern::fail = false;
            return flag;
        }

        logging::inf(intern::mod,
            "Size of channel status shared memory object matches"
        );

        logging::inf(intern::mod,
            "Checking if channel data shared memory object exists: ",
            "Type: Estimate"
        );

        flag = shmem::exists("estm-data");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to check if channel data shared memory object exists"
            );
            logging::err(intern::mod,
                "Failed to check if selected channel resources exist"
            );
            intern::fail = true;
            return flag;
        }

        if (!flag) {
            logging::inf(intern::mod,
                "Channel data shared memory object does not exist"
            );
            logging::inf(intern::mod,
                "Selected channel resources do not exist"
            );
            flag = false;
            intern::fail = false;
            return flag;
        }

        logging::inf(intern::mod,
            "Channel data shared memory object exists"
        );

        logging::inf(intern::mod,
            "Checking size of channel data shared memory object: ",
            "Type: Estimate"
        );

        flag = shmem::fits<linalg::fvector>("estm-data", 4);
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to check size of channel data shared memory object"
            );
            logging::err(intern::mod,
                "Failed to check if selected channel resources exist"
            );
            intern::fail = true;
            return flag;
        }

        if (!flag) {
            logging::inf(intern::mod,
                "Size of channel data shared memory object does not match"
            );
            logging::inf(intern::mod,
                "Selected channel resources do not exist"
            );
            flag = false;
            intern::fail = false;
            return flag;
        }

        logging::inf(intern::mod,
            "Size of channel data shared memory object matches"
        );
    }

    if (intern::req_sens) {
        logging::inf(intern::mod,
            "Checking if channel status shared memory object exists: ",
            "Type: Sensing"
        );

        flag = shmem::exists("sens-drdy");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to check if channel status shared memory object exists"
            );
            logging::err(intern::mod,
                "Failed to check if selected channel resources exist"
            );
            intern::fail = true;
            return flag;
        }

        if (!flag) {
            logging::inf(intern::mod,
                "Channel status shared memory object does not exist"
            );
            logging::inf(intern::mod,
                "Selected channel resources do not exist"
            );
            flag = false;
            intern::fail = false;
            return flag;
        }

        logging::inf(intern::mod,
            "Channel status shared memory object exists"
        );

        logging::inf(intern::mod,
            "Checking size of channel status shared memory object: ",
            "Type: Sensing"
        );

        flag = shmem::fits<bool>("sens-drdy");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to check size of channel status shared memory object"
            );
            logging::err(intern::mod,
                "Failed to check if selected channel resources exist"
            );
            intern::fail = true;
            return flag;
        }

        if (!flag) {
            logging::inf(intern::mod,
                "Size of channel status shared memory object does not match"
            );
            logging::inf(intern::mod,
                "Selected channel resources do not exist"
            );
            flag = false;
            intern::fail = false;
            return flag;
        }

        logging::inf(intern::mod,
            "Size of channel status shared memory object matches"
        );

        logging::inf(intern::mod,
            "Checking if channel data shared memory object exists: ",
            "Type: Sensing"
        );

        flag = shmem::exists("sens-data");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to check if channel data shared memory object exists"
            );
            logging::err(intern::mod,
                "Failed to check if selected channel resources exist"
            );
            intern::fail = true;
            return flag;
        }

        if (!flag) {
            logging::inf(intern::mod,
                "Channel data shared memory object does not exist"
            );
            logging::inf(intern::mod,
                "Selected channel resources do not exist"
            );
            flag = false;
            intern::fail = false;
            return flag;
        }

        logging::inf(intern::mod,
            "Channel data shared memory object exists"
        );

        logging::inf(intern::mod,
            "Checking size of channel data shared memory object: ",
            "Type: Sensing"
        );

        flag = shmem::fits<linalg::fvector>("sens-data", 11);
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to check size of channel data shared memory object"
            );
            logging::err(intern::mod,
                "Failed to check if selected channel resources exist"
            );
            intern::fail = true;
            return flag;
        }

        if (!flag) {
            logging::inf(intern::mod,
                "Size of channel data shared memory object does not match"
            );
            logging::inf(intern::mod,
                "Selected channel resources do not exist"
            );
            flag = false;
            intern::fail = false;
            return flag;
        }

        logging::inf(intern::mod,
            "Size of channel data shared memory object matches"
        );
    }

    if (intern::req_updt) {
        logging::inf(intern::mod,
            "Checking if channel status shared memory object exists: ",
            "Type: Update"
        );

        flag = shmem::exists("updt-drdy");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to check if channel status shared memory object exists"
            );
            logging::err(intern::mod,
                "Failed to check if selected channel resources exist"
            );
            intern::fail = true;
            return flag;
        }

        if (!flag) {
            logging::inf(intern::mod,
                "Channel status shared memory object does not exist"
            );
            logging::inf(intern::mod,
                "Selected channel resources do not exist"
            );
            flag = false;
            intern::fail = false;
            return flag;
        }

        logging::inf(intern::mod,
            "Channel status shared memory object exists"
        );

        logging::inf(intern::mod,
            "Checking size of channel status shared memory object: ",
            "Type: Update"
        );

        flag = shmem::fits<bool>("updt-drdy");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to check size of channel status shared memory object"
            );
            logging::err(intern::mod,
                "Failed to check if selected channel resources exist"
            );
            intern::fail = true;
            return flag;
        }

        if (!flag) {
            logging::inf(intern::mod,
                "Size of channel status shared memory object does not match"
            );
            logging::inf(intern::mod,
                "Selected channel resources do not exist"
            );
            flag = false;
            intern::fail = false;
            return flag;
        }

        logging::inf(intern::mod,
            "Size of channel status shared memory object matches"
        );

        logging::inf(intern::mod,
            "Checking if channel data shared memory object exists: ",
            "Type: Update"
        );

        flag = shmem::exists("updt-data");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to check if channel data shared memory object exists"
            );
            logging::err(intern::mod,
                "Failed to check if selected channel resources exist"
            );
            intern::fail = true;
            return flag;
        }

        if (!flag) {
            logging::inf(intern::mod,
                "Channel data shared memory object does not exist"
            );
            logging::inf(intern::mod,
                "Selected channel resources do not exist"
            );
            flag = false;
            intern::fail = false;
            return flag;
        }

        logging::inf(intern::mod,
            "Channel data shared memory object exists"
        );

        logging::inf(intern::mod,
            "Checking size of channel data shared memory object: ",
            "Type: Update"
        );

        flag = shmem::fits<linalg::fvector>("updt-data", 4);
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to check size of channel data shared memory object"
            );
            logging::err(intern::mod,
                "Failed to check if selected channel resources exist"
            );
            intern::fail = true;
            return flag;
        }

        if (!flag) {
            logging::inf(intern::mod,
                "Size of channel data shared memory object does not match"
            );
            logging::inf(intern::mod,
                "Selected channel resources do not exist"
            );
            flag = false;
            intern::fail = false;
            return flag;
        }

        logging::inf(intern::mod,
            "Size of channel data shared memory object matches"
        );
    }

    logging::inf(intern::mod,
        "Selected channel resources exist"
    );

    flag = true;

    intern::fail = false;
    return flag;
}

void create (void) {
    logging::inf(intern::mod,
        "Creating selected channel resources"
    );

    logging::inf(intern::mod,
        "Creating access control semaphore"
    );

    sema::create("acces");
    if (sema::fail()) {
        logging::err(intern::mod,
            "Failed to create access control semaphore"
        );
        logging::err(intern::mod,
            "Failed to create selected channel resources"
        );
        intern::fail = true;
        return;
    }

    if (intern::req_ctrl) {
        logging::inf(intern::mod,
            "Creating channel status shared memory object: ",
            "Type: Control"
        );

        shmem::create<bool>("ctrl-drdy");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to create channel status shared memory object"
            );
            logging::wrn(intern::mod,
                "Removing access control semaphore"
            );
            sema::remove("acces");
            if (sema::fail()) {
                logging::err(intern::mod,
                    "Failed to remove access control semaphore"
                );
            }
            logging::err(intern::mod,
                "Failed to create selected channel resources"
            );
            intern::fail = true;
            return;
        }

        logging::inf(intern::mod,
            "Creating channel data shared memory object: ",
            "Type: Control"
        );

        shmem::create<double>("ctrl-data");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to create channel data shared memory object"
            );
            logging::wrn(intern::mod,
                "Removing access control semaphore"
            );
            sema::remove("acces");
            if (sema::fail()) {
                logging::err(intern::mod,
                    "Failed to remove access control semaphore"
                );
            }
            logging::wrn(intern::mod,
                "Removing channel status shared memory object: ",
                "Type: Control"
            );
            shmem::remove("ctrl-drdy");
            if (shmem::fail()) {
                logging::err(intern::mod,
                    "Failed to remove channel status shared memory object"
                );
            }
            logging::err(intern::mod,
                "Failed to create selected channel resources"
            );
            intern::fail = true;
            return;
        }
    }

    if (intern::req_stat) {
        logging::inf(intern::mod,
            "Creating channel status shared memory object: ",
            "Type: State"
        );

        shmem::create<bool>("stat-drdy");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to create channel status shared memory object"
            );
            logging::wrn(intern::mod,
                "Removing access control semaphore"
            );
            sema::remove("acces");
            if (sema::fail()) {
                logging::err(intern::mod,
                    "Failed to remove access control semaphore"
                );
            }
            if (intern::req_ctrl) {
                logging::wrn(intern::mod,
                    "Removing channel status shared memory object: ",
                    "Type: Control"
                );
                shmem::remove("ctrl-drdy");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel status shared memory object"
                    );
                }
                logging::wrn(intern::mod,
                    "Removing channel data shared memory object: ",
                    "Type: Control"
                );
                shmem::remove("ctrl-data");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel data shared memory object"
                    );
                }
            }
            logging::err(intern::mod,
                "Failed to create selected channel resources"
            );
            intern::fail = true;
            return;
        }

        logging::inf(intern::mod,
            "Creating channel data shared memory object: ",
            "Type: State"
        );

        shmem::create<linalg::fvector>("stat-data", 7);
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to create channel data shared memory object"
            );
            logging::wrn(intern::mod,
                "Removing access control semaphore"
            );
            sema::remove("acces");
            if (sema::fail()) {
                logging::err(intern::mod,
                    "Failed to remove access control semaphore"
                );
            }
            if (intern::req_ctrl) {
                logging::wrn(intern::mod,
                    "Removing channel status shared memory object: ",
                    "Type: Control"
                );
                shmem::remove("ctrl-drdy");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel status shared memory object"
                    );
                }
                logging::wrn(intern::mod,
                    "Removing channel data shared memory object: ",
                    "Type: Control"
                );
                shmem::remove("ctrl-data");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel data shared memory object"
                    );
                }
            }
            logging::wrn(intern::mod,
                "Removing channel status shared memory object: ",
                "Type: State"
            );
            shmem::remove("stat-drdy");
            if (shmem::fail()) {
                logging::err(intern::mod,
                    "Failed to remove channel status shared memory object"
                );
            }
            logging::err(intern::mod,
                "Failed to create selected channel resources"
            );
            intern::fail = true;
            return;
        }
    }

    if (intern::req_estm) {
        logging::inf(intern::mod,
            "Creating channel status shared memory object: ",
            "Type: Estimate"
        );

        shmem::create<bool>("estm-drdy");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to create channel status shared memory object"
            );
            logging::wrn(intern::mod,
                "Removing access control semaphore"
            );
            sema::remove("acces");
            if (sema::fail()) {
                logging::err(intern::mod,
                    "Failed to remove access control semaphore"
                );
            }
            if (intern::req_ctrl) {
                logging::wrn(intern::mod,
                    "Removing channel status shared memory object: ",
                    "Type: Control"
                );
                shmem::remove("ctrl-drdy");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel status shared memory object"
                    );
                }
                logging::wrn(intern::mod,
                    "Removing channel data shared memory object: ",
                    "Type: Control"
                );
                shmem::remove("ctrl-data");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel data shared memory object"
                    );
                }
            }
            if (intern::req_stat) {
                logging::wrn(intern::mod,
                    "Removing channel status shared memory object: ",
                    "Type: State"
                );
                shmem::remove("stat-drdy");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel status shared memory object"
                    );
                }
                logging::wrn(intern::mod,
                    "Removing channel data shared memory object: ",
                    "Type: State"
                );
                shmem::remove("stat-data");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel data shared memory object"
                    );
                }
            }
            logging::err(intern::mod,
                "Failed to create selected channel resources"
            );
            intern::fail = true;
            return;
        }

        logging::inf(intern::mod,
            "Creating channel data shared memory object: ",
            "Type: Estimate"
        );

        shmem::create<linalg::fvector>("estm-data", 4);
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to create channel data shared memory object"
            );
            logging::wrn(intern::mod,
                "Removing access control semaphore"
            );
            sema::remove("acces");
            if (sema::fail()) {
                logging::err(intern::mod,
                    "Failed to remove access control semaphore"
                );
            }
            if (intern::req_ctrl) {
                logging::wrn(intern::mod,
                    "Removing channel status shared memory object: ",
                    "Type: Control"
                );
                shmem::remove("ctrl-drdy");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel status shared memory object"
                    );
                }
                logging::wrn(intern::mod,
                    "Removing channel data shared memory object: ",
                    "Type: Control"
                );
                shmem::remove("ctrl-data");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel data shared memory object"
                    );
                }
            }
            if (intern::req_stat) {
                logging::wrn(intern::mod,
                    "Removing channel status shared memory object: ",
                    "Type: State"
                );
                shmem::remove("stat-drdy");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel status shared memory object"
                    );
                }
                logging::wrn(intern::mod,
                    "Removing channel data shared memory object: ",
                    "Type: State"
                );
                shmem::remove("stat-data");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel data shared memory object"
                    );
                }
            }
            logging::wrn(intern::mod,
                "Removing channel status shared memory object: ",
                "Type: Estimate"
            );
            shmem::remove("estm-drdy");
            if (shmem::fail()) {
                logging::err(intern::mod,
                    "Failed to remove channel status shared memory object"
                );
            }
            logging::err(intern::mod,
                "Failed to create selected channel resources"
            );
            intern::fail = true;
            return;
        }
    }

    if (intern::req_sens) {
        logging::inf(intern::mod,
            "Creating channel status shared memory object: ",
            "Type: Sensing"
        );

        shmem::create<bool>("sens-drdy");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to create channel status shared memory object"
            );
            logging::wrn(intern::mod,
                "Removing access control semaphore"
            );
            sema::remove("acces");
            if (sema::fail()) {
                logging::err(intern::mod,
                    "Failed to remove access control semaphore"
                );
            }
            if (intern::req_ctrl) {
                logging::wrn(intern::mod,
                    "Removing channel status shared memory object: ",
                    "Type: Control"
                );
                shmem::remove("ctrl-drdy");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel status shared memory object"
                    );
                }
                logging::wrn(intern::mod,
                    "Removing channel data shared memory object: ",
                    "Type: Control"
                );
                shmem::remove("ctrl-data");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel data shared memory object"
                    );
                }
            }
            if (intern::req_stat) {
                logging::wrn(intern::mod,
                    "Removing channel status shared memory object: ",
                    "Type: State"
                );
                shmem::remove("stat-drdy");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel status shared memory object"
                    );
                }
                logging::wrn(intern::mod,
                    "Removing channel data shared memory object: ",
                    "Type: State"
                );
                shmem::remove("stat-data");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel data shared memory object"
                    );
                }
            }
            if (intern::req_estm) {
                logging::wrn(intern::mod,
                    "Removing channel status shared memory object: ",
                    "Type: Estimate"
                );
                shmem::remove("estm-drdy");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel status shared memory object"
                    );
                }
                logging::wrn(intern::mod,
                    "Removing channel data shared memory object: ",
                    "Type: Estimate"
                );
                shmem::remove("estm-data");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel data shared memory object"
                    );
                }
            }
            logging::err(intern::mod,
                "Failed to create selected channel resources"
            );
            intern::fail = true;
            return;
        }

        logging::inf(intern::mod,
            "Creating channel data shared memory object: ",
            "Type: Sensing"
        );

        shmem::create<linalg::fvector>("sens-data", 11);
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to create channel data shared memory object"
            );
            logging::wrn(intern::mod,
                "Removing access control semaphore"
            );
            sema::remove("acces");
            if (sema::fail()) {
                logging::err(intern::mod,
                    "Failed to remove access control semaphore"
                );
            }
            if (intern::req_ctrl) {
                logging::wrn(intern::mod,
                    "Removing channel status shared memory object: ",
                    "Type: Control"
                );
                shmem::remove("ctrl-drdy");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel status shared memory object"
                    );
                }
                logging::wrn(intern::mod,
                    "Removing channel data shared memory object: ",
                    "Type: Control"
                );
                shmem::remove("ctrl-data");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel data shared memory object"
                    );
                }
            }
            if (intern::req_stat) {
                logging::wrn(intern::mod,
                    "Removing channel status shared memory object: ",
                    "Type: State"
                );
                shmem::remove("stat-drdy");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel status shared memory object"
                    );
                }
                logging::wrn(intern::mod,
                    "Removing channel data shared memory object: ",
                    "Type: State"
                );
                shmem::remove("stat-data");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel data shared memory object"
                    );
                }
            }
            if (intern::req_estm) {
                logging::wrn(intern::mod,
                    "Removing channel status shared memory object: ",
                    "Type: Estimate"
                );
                shmem::remove("estm-drdy");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel status shared memory object"
                    );
                }
                logging::wrn(intern::mod,
                    "Removing channel data shared memory object: ",
                    "Type: Estimate"
                );
                shmem::remove("estm-data");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel data shared memory object"
                    );
                }
            }
            logging::wrn(intern::mod,
                "Removing channel status shared memory object: ",
                "Type: Sensing"
            );
            shmem::remove("sens-drdy");
            if (shmem::fail()) {
                logging::err(intern::mod,
                    "Failed to remove channel status shared memory object"
                );
            }
            logging::err(intern::mod,
                "Failed to create selected channel resources"
            );
            intern::fail = true;
            return;
        }
    }

    if (intern::req_updt) {
        logging::inf(intern::mod,
            "Creating channel status shared memory object: ",
            "Type: Update"
        );

        shmem::create<bool>("updt-drdy");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to create channel status shared memory object"
            );
            logging::wrn(intern::mod,
                "Removing access control semaphore"
            );
            sema::remove("acces");
            if (sema::fail()) {
                logging::err(intern::mod,
                    "Failed to remove access control semaphore"
                );
            }
            if (intern::req_ctrl) {
                logging::wrn(intern::mod,
                    "Removing channel status shared memory object: ",
                    "Type: Control"
                );
                shmem::remove("ctrl-drdy");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel status shared memory object"
                    );
                }
                logging::wrn(intern::mod,
                    "Removing channel data shared memory object: ",
                    "Type: Control"
                );
                shmem::remove("ctrl-data");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel data shared memory object"
                    );
                }
            }
            if (intern::req_stat) {
                logging::wrn(intern::mod,
                    "Removing channel status shared memory object: ",
                    "Type: State"
                );
                shmem::remove("stat-drdy");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel status shared memory object"
                    );
                }
                logging::wrn(intern::mod,
                    "Removing channel data shared memory object: ",
                    "Type: State"
                );
                shmem::remove("stat-data");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel data shared memory object"
                    );
                }
            }
            if (intern::req_estm) {
                logging::wrn(intern::mod,
                    "Removing channel status shared memory object: ",
                    "Type: Estimate"
                );
                shmem::remove("estm-drdy");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel status shared memory object"
                    );
                }
                logging::wrn(intern::mod,
                    "Removing channel data shared memory object: ",
                    "Type: Estimate"
                );
                shmem::remove("estm-data");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel data shared memory object"
                    );
                }
            }
            if (intern::req_sens) {
                logging::wrn(intern::mod,
                    "Removing channel status shared memory object: ",
                    "Type: Sensing"
                );
                shmem::remove("sens-drdy");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel status shared memory object"
                    );
                }
                logging::wrn(intern::mod,
                    "Removing channel data shared memory object: ",
                    "Type: Sensing"
                );
                shmem::remove("sens-data");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel data shared memory object"
                    );
                }
            }
            logging::err(intern::mod,
                "Failed to create selected channel resources"
            );
            intern::fail = true;
            return;
        }

        logging::inf(intern::mod,
            "Creating channel data shared memory object: ",
            "Type: Update"
        );

        shmem::create<linalg::fvector>("updt-data", 4);
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to create channel data shared memory object"
            );
            logging::wrn(intern::mod,
                "Removing access control semaphore"
            );
            sema::remove("acces");
            if (sema::fail()) {
                logging::err(intern::mod,
                    "Failed to remove access control semaphore"
                );
            }
            if (intern::req_ctrl) {
                logging::wrn(intern::mod,
                    "Removing channel status shared memory object: ",
                    "Type: Control"
                );
                shmem::remove("ctrl-drdy");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel status shared memory object"
                    );
                }
                logging::wrn(intern::mod,
                    "Removing channel data shared memory object: ",
                    "Type: Control"
                );
                shmem::remove("ctrl-data");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel data shared memory object"
                    );
                }
            }
            if (intern::req_stat) {
                logging::wrn(intern::mod,
                    "Removing channel status shared memory object: ",
                    "Type: State"
                );
                shmem::remove("stat-drdy");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel status shared memory object"
                    );
                }
                logging::wrn(intern::mod,
                    "Removing channel data shared memory object: ",
                    "Type: State"
                );
                shmem::remove("stat-data");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel data shared memory object"
                    );
                }
            }
            if (intern::req_estm) {
                logging::wrn(intern::mod,
                    "Removing channel status shared memory object: ",
                    "Type: Estimate"
                );
                shmem::remove("estm-drdy");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel status shared memory object"
                    );
                }
                logging::wrn(intern::mod,
                    "Removing channel data shared memory object: ",
                    "Type: Estimate"
                );
                shmem::remove("estm-data");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel data shared memory object"
                    );
                }
            }
            if (intern::req_sens) {
                logging::wrn(intern::mod,
                    "Removing channel status shared memory object: ",
                    "Type: Sensing"
                );
                shmem::remove("sens-drdy");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel status shared memory object"
                    );
                }
                logging::wrn(intern::mod,
                    "Removing channel data shared memory object: ",
                    "Type: Sensing"
                );
                shmem::remove("sens-data");
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to remove channel data shared memory object"
                    );
                }
            }
            logging::wrn(intern::mod,
                "Removing channel status shared memory object: ",
                "Type: Update"
            );
            shmem::remove("updt-drdy");
            if (shmem::fail()) {
                logging::err(intern::mod,
                    "Failed to remove channel status shared memory object"
                );
            }
            logging::err(intern::mod,
                "Failed to create selected channel resources"
            );
            intern::fail = true;
            return;
        }
    }

    intern::fail = false;
}

void remove (void) {
    intern::fail = false;

    logging::inf(intern::mod,
        "Removing selected channel resources"
    );

    logging::inf(intern::mod,
        "Removing access control semaphore"
    );

    sema::remove("acces");
    if (sema::fail()) {
        logging::err(intern::mod,
            "Failed to remove access control semaphore"
        );
        intern::fail = true;
    }

    if (intern::req_ctrl) {
        logging::inf(intern::mod,
            "Removing channel status shared memory object: ",
            "Type: Control"
        );
        shmem::remove("ctrl-drdy");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to remove channel status shared memory object"
            );
            intern::fail = true;
        }
        logging::inf(intern::mod,
            "Removing channel data shared memory object: ",
            "Type: Control"
        );
        shmem::remove("ctrl-data");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to remove channel data shared memory object"
            );
            intern::fail = true;
        }
    }

    if (intern::req_stat) {
        logging::inf(intern::mod,
            "Removing channel status shared memory object: ",
            "Type: State"
        );
        shmem::remove("stat-drdy");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to remove channel status shared memory object"
            );
            intern::fail = true;
        }
        logging::inf(intern::mod,
            "Removing channel data shared memory object: ",
            "Type: State"
        );
        shmem::remove("stat-data");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to remove channel data shared memory object"
            );
            intern::fail = true;
        }
    }

    if (intern::req_estm) {
        logging::inf(intern::mod,
            "Removing channel status shared memory object: ",
            "Type: Estimate"
        );
        shmem::remove("estm-drdy");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to remove channel status shared memory object"
            );
            intern::fail = true;
        }
        logging::inf(intern::mod,
            "Removing channel data shared memory object: ",
            "Type: Estimate"
        );
        shmem::remove("estm-data");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to remove channel data shared memory object"
            );
            intern::fail = true;
        }
    }

    if (intern::req_sens) {
        logging::inf(intern::mod,
            "Removing channel status shared memory object: ",
            "Type: Sensing"
        );
        shmem::remove("sens-drdy");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to remove channel status shared memory object"
            );
            intern::fail = true;
        }
        logging::inf(intern::mod,
            "Removing channel data shared memory object: ",
            "Type: Sensing"
        );
        shmem::remove("sens-data");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to remove channel data shared memory object"
            );
            intern::fail = true;
        }
    }

    if (intern::req_updt) {
        logging::inf(intern::mod,
            "Removing channel status shared memory object: ",
            "Type: Update"
        );
        shmem::remove("updt-drdy");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to remove channel status shared memory object"
            );
            intern::fail = true;
        }
        logging::inf(intern::mod,
            "Removing channel data shared memory object: ",
            "Type: Update"
        );
        shmem::remove("updt-data");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to remove channel data shared memory object"
            );
            intern::fail = true;
        }
    }

    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to remove selected channel resources"
        );
    }
}

void open (void) {
    logging::inf(intern::mod,
        "Opening selected channel resources"
    );

    logging::inf(intern::mod,
        "Opening access control semaphore"
    );

    intern::id_acces = sema::open("acces");
    if (sema::fail()) {
        logging::err(intern::mod,
            "Failed to open access control semaphore"
        );
        logging::err(intern::mod,
            "Failed to open selected channel resources"
        );
        intern::fail = true;
        return;
    }

    if (intern::req_ctrl) {
        logging::inf(intern::mod,
            "Opening channel status shared memory object: ",
            "Type: Control"
        );

        intern::id_ctrl_drdy = shmem::open("ctrl-drdy");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to open channel status shared memory object"
            );
            logging::wrn(intern::mod,
                "Closing opened semaphores"
            );
            sema::close();
            if (sema::fail()) {
                logging::err(intern::mod,
                    "Failed to close opened semaphores"
                );
            }
            logging::err(intern::mod,
                "Failed to open selected channel resources"
            );
            intern::fail = true;
            return;
        }

        logging::inf(intern::mod,
            "Opening channel data shared memory object: ",
            "Type: Control"
        );

        intern::id_ctrl_data = shmem::open("ctrl-data");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to open channel data shared memory object"
            );
            logging::wrn(intern::mod,
                "Closing opened semaphores"
            );
            sema::close();
            if (sema::fail()) {
                logging::err(intern::mod,
                    "Failed to close opened semaphores"
                );
            }
            logging::wrn(intern::mod,
                "Closing opened shared memory objects"
            );
            shmem::close();
            if (shmem::fail()) {
                logging::err(intern::mod,
                    "Failed to close opened shared memory objects"
                );
            }
            logging::err(intern::mod,
                "Failed to open selected channel resources"
            );
            intern::fail = true;
            return;
        }
    }

    if (intern::req_stat) {
        logging::inf(intern::mod,
            "Opening channel status shared memory object: ",
            "Type: State"
        );

        intern::id_stat_drdy = shmem::open("stat-drdy");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to open channel status shared memory object"
            );
            logging::wrn(intern::mod,
                "Closing opened semaphores"
            );
            sema::close();
            if (sema::fail()) {
                logging::err(intern::mod,
                    "Failed to close opened semaphores"
                );
            }
            if (intern::req_ctrl) {
                logging::wrn(intern::mod,
                    "Closing opened shared memory objects"
                );
                shmem::close();
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to close opened shared memory objects"
                    );
                }
            }
            logging::err(intern::mod,
                "Failed to open selected channel resources"
            );
            intern::fail = true;
            return;
        }

        logging::inf(intern::mod,
            "Opening channel data shared memory object: ",
            "Type: State"
        );

        intern::id_stat_data = shmem::open("stat-data");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to open channel data shared memory object"
            );
            logging::wrn(intern::mod,
                "Closing opened semaphores"
            );
            sema::close();
            if (sema::fail()) {
                logging::err(intern::mod,
                    "Failed to close opened semaphores"
                );
            }
            logging::wrn(intern::mod,
                "Closing opened shared memory objects"
            );
            shmem::close();
            if (shmem::fail()) {
                logging::err(intern::mod,
                    "Failed to close opened shared memory objects"
                );
            }
            logging::err(intern::mod,
                "Failed to open selected channel resources"
            );
            intern::fail = true;
            return;
        }
    }

    if (intern::req_estm) {
        logging::inf(intern::mod,
            "Opening channel status shared memory object: ",
            "Type: Estimate"
        );

        intern::id_estm_drdy = shmem::open("estm-drdy");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to open channel status shared memory object"
            );
            logging::wrn(intern::mod,
                "Closing opened semaphores"
            );
            sema::close();
            if (sema::fail()) {
                logging::err(intern::mod,
                    "Failed to close opened semaphores"
                );
            }
            if (intern::req_ctrl || intern::req_stat) {
                logging::wrn(intern::mod,
                    "Closing opened shared memory objects"
                );
                shmem::close();
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to close opened shared memory objects"
                    );
                }
            }
            logging::err(intern::mod,
                "Failed to open selected channel resources"
            );
            intern::fail = true;
            return;
        }

        logging::inf(intern::mod,
            "Opening channel data shared memory object: ",
            "Type: Estimate"
        );

        intern::id_estm_data = shmem::open("estm-data");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to open channel data shared memory object"
            );
            logging::wrn(intern::mod,
                "Closing opened semaphores"
            );
            sema::close();
            if (sema::fail()) {
                logging::err(intern::mod,
                    "Failed to close opened semaphores"
                );
            }
            logging::wrn(intern::mod,
                "Closing opened shared memory objects"
            );
            shmem::close();
            if (shmem::fail()) {
                logging::err(intern::mod,
                    "Failed to close opened shared memory objects"
                );
            }
            logging::err(intern::mod,
                "Failed to open selected channel resources"
            );
            intern::fail = true;
            return;
        }
    }

    if (intern::req_sens) {
        logging::inf(intern::mod,
            "Opening channel status shared memory object: ",
            "Type: Sensing"
        );

        intern::id_sens_drdy = shmem::open("sens-drdy");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to open channel status shared memory object"
            );
            logging::wrn(intern::mod,
                "Closing opened semaphores"
            );
            sema::close();
            if (sema::fail()) {
                logging::err(intern::mod,
                    "Failed to close opened semaphores"
                );
            }
            if (intern::req_ctrl || intern::req_stat || intern::req_estm) {
                logging::wrn(intern::mod,
                    "Closing opened shared memory objects"
                );
                shmem::close();
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to close opened shared memory objects"
                    );
                }
            }
            logging::err(intern::mod,
                "Failed to open selected channel resources"
            );
            intern::fail = true;
            return;
        }

        logging::inf(intern::mod,
            "Opening channel data shared memory object: ",
            "Type: Sensing"
        );

        intern::id_sens_data = shmem::open("sens-data");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to open channel data shared memory object"
            );
            logging::wrn(intern::mod,
                "Closing opened semaphores"
            );
            sema::close();
            if (sema::fail()) {
                logging::err(intern::mod,
                    "Failed to close opened semaphores"
                );
            }
            logging::wrn(intern::mod,
                "Closing opened shared memory objects"
            );
            shmem::close();
            if (shmem::fail()) {
                logging::err(intern::mod,
                    "Failed to close opened shared memory objects"
                );
            }
            logging::err(intern::mod,
                "Failed to open selected channel resources"
            );
            intern::fail = true;
            return;
        }
    }

    if (intern::req_updt) {
        logging::inf(intern::mod,
            "Opening channel status shared memory object: ",
            "Type: Update"
        );

        intern::id_updt_drdy = shmem::open("updt-drdy");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to open channel status shared memory object"
            );
            logging::wrn(intern::mod,
                "Closing opened semaphores"
            );
            sema::close();
            if (sema::fail()) {
                logging::err(intern::mod,
                    "Failed to close opened semaphores"
                );
            }
            if (
                   intern::req_ctrl || intern::req_stat
                || intern::req_estm || intern::req_sens
            ) {
                logging::wrn(intern::mod,
                    "Closing opened shared memory objects"
                );
                shmem::close();
                if (shmem::fail()) {
                    logging::err(intern::mod,
                        "Failed to close opened shared memory objects"
                    );
                }
            }
            logging::err(intern::mod,
                "Failed to open selected channel resources"
            );
            intern::fail = true;
            return;
        }

        logging::inf(intern::mod,
            "Opening channel data shared memory object: ",
            "Type: Update"
        );

        intern::id_updt_data = shmem::open("updt-data");
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to open channel data shared memory object"
            );
            logging::wrn(intern::mod,
                "Closing opened semaphores"
            );
            sema::close();
            if (sema::fail()) {
                logging::err(intern::mod,
                    "Failed to close opened semaphores"
                );
            }
            logging::wrn(intern::mod,
                "Closing opened shared memory objects"
            );
            shmem::close();
            if (shmem::fail()) {
                logging::err(intern::mod,
                    "Failed to close opened shared memory objects"
                );
            }
            logging::err(intern::mod,
                "Failed to open selected channel resources"
            );
            intern::fail = true;
            return;
        }
    }

    intern::fail = false;
}

void close (void) {
    intern::fail = false;

    logging::inf(intern::mod,
        "Closing selected channel resources"
    );

    logging::inf(intern::mod,
        "Closing opened semaphores"
    );

    sema::close();
    if (sema::fail()) {
        logging::err(intern::mod,
            "Failed to close opened semaphores"
        );
        intern::fail = true;
    }

    if (
           intern::req_ctrl || intern::req_stat || intern::req_estm
        || intern::req_sens || intern::req_updt
    ) {
        logging::inf(intern::mod,
            "Closing opened shared memory objects"
        );
        shmem::close();
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to close opened shared memory objects"
            );
            intern::fail = true;
        }
    }

    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to close selected channel resources"
        );
    }
}

void put_ctrl (double ctrl) {
    std::string oupt;

    oupt = intern::conv_val_to_oupt(ctrl);
    logging::inf(intern::mod,
        "Writing to channel: Type: Control, Value: ", oupt
    );

    logging::inf(intern::mod,
        "Writing to channel data shared memory object"
    );

    shmem::put(intern::id_ctrl_data, ctrl);
    if (shmem::fail()) {
        logging::err(intern::mod,
            "Failed to write to channel data shared memory object"
        );
        logging::err(intern::mod,
            "Failed to write to channel"
        );
        intern::fail = true;
        return;
    }

    logging::inf(intern::mod,
        "Writing to channel status shared memory object"
    );

    shmem::put(intern::id_ctrl_drdy, true);
    if (shmem::fail()) {
        logging::err(intern::mod,
            "Failed to write to channel status shared memory object"
        );
        logging::err(intern::mod,
            "Failed to write to channel"
        );
        intern::fail = true;
        return;
    }

    intern::fail = false;
    return;
}

void put_stat (linalg::fvector stat) {
    std::string oupt;

    oupt = intern::conv_val_to_oupt(stat);
    logging::inf(intern::mod,
        "Writing to channel: Type: State, Value: ", oupt
    );

    logging::inf(intern::mod,
        "Writing to channel data shared memory object"
    );

    shmem::put(intern::id_stat_data, stat);
    if (shmem::fail()) {
        logging::err(intern::mod,
            "Failed to write to channel data shared memory object"
        );
        logging::err(intern::mod,
            "Failed to write to channel"
        );
        intern::fail = true;
        return;
    }

    logging::inf(intern::mod,
        "Writing to channel status shared memory object"
    );

    shmem::put(intern::id_stat_drdy, true);
    if (shmem::fail()) {
        logging::err(intern::mod,
            "Failed to write to channel status shared memory object"
        );
        logging::err(intern::mod,
            "Failed to write to channel"
        );
        intern::fail = true;
        return;
    }

    intern::fail = false;
    return;
}

void put_estm (linalg::fvector estm) {
    std::string oupt;

    oupt = intern::conv_val_to_oupt(estm);
    logging::inf(intern::mod,
        "Writing to channel: Type: Estimate, Value: ", oupt
    );

    logging::inf(intern::mod,
        "Writing to channel data shared memory object"
    );

    shmem::put(intern::id_estm_data, estm);
    if (shmem::fail()) {
        logging::err(intern::mod,
            "Failed to write to channel data shared memory object"
        );
        logging::err(intern::mod,
            "Failed to write to channel"
        );
        intern::fail = true;
        return;
    }

    logging::inf(intern::mod,
        "Writing to channel status shared memory object"
    );

    shmem::put(intern::id_estm_drdy, true);
    if (shmem::fail()) {
        logging::err(intern::mod,
            "Failed to write to channel status shared memory object"
        );
        logging::err(intern::mod,
            "Failed to write to channel"
        );
        intern::fail = true;
        return;
    }

    intern::fail = false;
    return;
}

void put_sens (linalg::fvector sens) {
    std::string oupt;

    oupt = intern::conv_val_to_oupt(sens);
    logging::inf(intern::mod,
        "Writing to channel: Type: Sensing, Value: ", oupt
    );

    logging::inf(intern::mod,
        "Writing to channel data shared memory object"
    );

    shmem::put(intern::id_sens_data, sens);
    if (shmem::fail()) {
        logging::err(intern::mod,
            "Failed to write to channel data shared memory object"
        );
        logging::err(intern::mod,
            "Failed to write to channel"
        );
        intern::fail = true;
        return;
    }

    logging::inf(intern::mod,
        "Writing to channel status shared memory object"
    );

    shmem::put(intern::id_sens_drdy, true);
    if (shmem::fail()) {
        logging::err(intern::mod,
            "Failed to write to channel status shared memory object"
        );
        logging::err(intern::mod,
            "Failed to write to channel"
        );
        intern::fail = true;
        return;
    }

    intern::fail = false;
    return;
}

void put_updt (linalg::fvector updt) {
    std::string oupt;

    oupt = intern::conv_val_to_oupt(updt);
    logging::inf(intern::mod,
        "Writing to channel: Type: Update, Value: ", oupt
    );

    logging::inf(intern::mod,
        "Writing to channel data shared memory object"
    );

    shmem::put(intern::id_updt_data, updt);
    if (shmem::fail()) {
        logging::err(intern::mod,
            "Failed to write to channel data shared memory object"
        );
        logging::err(intern::mod,
            "Failed to write to channel"
        );
        intern::fail = true;
        return;
    }

    logging::inf(intern::mod,
        "Writing to channel status shared memory object"
    );

    shmem::put(intern::id_updt_drdy, true);
    if (shmem::fail()) {
        logging::err(intern::mod,
            "Failed to write to channel status shared memory object"
        );
        logging::err(intern::mod,
            "Failed to write to channel"
        );
        intern::fail = true;
        return;
    }

    intern::fail = false;
    return;
}

double get_ctrl (void) {
    bool flag;
    double ctrl;
    std::string oupt;

    logging::inf(intern::mod,
        "Reading from channel: Type: Control"
    );

    logging::inf(intern::mod,
        "Reading from channel status shared memory object"
    );

    flag = shmem::get<bool>(intern::id_ctrl_drdy);
    if (shmem::fail()) {
        logging::err(intern::mod,
            "Failed to read from channel status shared memory object"
        );
        logging::err(intern::mod,
            "Failed to read from channel"
        );
        intern::fail = true;
        intern::blank = false;
        return ctrl;
    }

    oupt = intern::conv_val_to_oupt(flag);
    logging::inf(intern::mod,
        "Read from channel status shared memory object: Value: ", oupt
    );

    if (!flag) {
        logging::wrn(intern::mod,
            "Channel content is blank"
        );
        intern::fail = false;
        intern::blank = true;
        return ctrl;
    }

    logging::inf(intern::mod,
        "Reading from channel data shared memory object"
    );

    ctrl = shmem::get<double>(intern::id_ctrl_data);
    if (shmem::fail()) {
        logging::err(intern::mod,
            "Failed to read from channel data shared memory object"
        );
        logging::err(intern::mod,
            "Failed to read from channel"
        );
        intern::fail = true;
        intern::blank = false;
        return ctrl;
    }

    oupt = intern::conv_val_to_oupt(ctrl);
    logging::inf(intern::mod,
        "Read from channel data shared memory object: Value: ", oupt
    );

    intern::fail = false;
    intern::blank = false;
    return ctrl;
}

linalg::fvector get_stat (void) {
    bool flag;
    linalg::fvector stat;
    std::string oupt;

    logging::inf(intern::mod,
        "Reading from channel: Type: State"
    );

    logging::inf(intern::mod,
        "Reading from channel status shared memory object"
    );

    flag = shmem::get<bool>(intern::id_stat_drdy);
    if (shmem::fail()) {
        logging::err(intern::mod,
            "Failed to read from channel status shared memory object"
        );
        logging::err(intern::mod,
            "Failed to read from channel"
        );
        intern::fail = true;
        intern::blank = false;
        return stat;
    }

    oupt = intern::conv_val_to_oupt(flag);
    logging::inf(intern::mod,
        "Read from channel status shared memory object: Value: ", oupt
    );

    if (!flag) {
        logging::wrn(intern::mod,
            "Channel content is blank"
        );
        intern::fail = false;
        intern::blank = true;
        return stat;
    }

    logging::inf(intern::mod,
        "Reading from channel data shared memory object"
    );

    stat = shmem::get<linalg::fvector>(intern::id_stat_data, 7);
    if (shmem::fail()) {
        logging::err(intern::mod,
            "Failed to read from channel data shared memory object"
        );
        logging::err(intern::mod,
            "Failed to read from channel"
        );
        intern::fail = true;
        intern::blank = false;
        return stat;
    }

    oupt = intern::conv_val_to_oupt(stat);
    logging::inf(intern::mod,
        "Read from channel data shared memory object: Value: ", oupt
    );

    intern::fail = false;
    intern::blank = false;
    return stat;
}

linalg::fvector get_estm (void) {
    bool flag;
    linalg::fvector estm;
    std::string oupt;

    logging::inf(intern::mod,
        "Reading from channel: Type: Estimate"
    );

    logging::inf(intern::mod,
        "Reading from channel status shared memory object"
    );

    flag = shmem::get<bool>(intern::id_estm_drdy);
    if (shmem::fail()) {
        logging::err(intern::mod,
            "Failed to read from channel status shared memory object"
        );
        logging::err(intern::mod,
            "Failed to read from channel"
        );
        intern::fail = true;
        intern::blank = false;
        return estm;
    }

    oupt = intern::conv_val_to_oupt(flag);
    logging::inf(intern::mod,
        "Read from channel status shared memory object: Value: ", oupt
    );

    if (!flag) {
        logging::wrn(intern::mod,
            "Channel content is blank"
        );
        intern::fail = false;
        intern::blank = true;
        return estm;
    }

    logging::inf(intern::mod,
        "Reading from channel data shared memory object"
    );

    estm = shmem::get<linalg::fvector>(intern::id_estm_data, 4);
    if (shmem::fail()) {
        logging::err(intern::mod,
            "Failed to read from channel data shared memory object"
        );
        logging::err(intern::mod,
            "Failed to read from channel"
        );
        intern::fail = true;
        intern::blank = false;
        return estm;
    }

    oupt = intern::conv_val_to_oupt(estm);
    logging::inf(intern::mod,
        "Read from channel data shared memory object: Value: ", oupt
    );

    intern::fail = false;
    intern::blank = false;
    return estm;
}

linalg::fvector get_sens (void) {
    bool flag;
    linalg::fvector sens;
    std::string oupt;

    logging::inf(intern::mod,
        "Reading from channel: Type: Sensing"
    );

    logging::inf(intern::mod,
        "Reading from channel status shared memory object"
    );

    flag = shmem::get<bool>(intern::id_sens_drdy);
    if (shmem::fail()) {
        logging::err(intern::mod,
            "Failed to read from channel status shared memory object"
        );
        logging::err(intern::mod,
            "Failed to read from channel"
        );
        intern::fail = true;
        intern::blank = false;
        return sens;
    }

    oupt = intern::conv_val_to_oupt(flag);
    logging::inf(intern::mod,
        "Read from channel status shared memory object: Value: ", oupt
    );

    if (!flag) {
        logging::wrn(intern::mod,
            "Channel content is blank"
        );
        intern::fail = false;
        intern::blank = true;
        return sens;
    }

    logging::inf(intern::mod,
        "Reading from channel data shared memory object"
    );

    sens = shmem::get<linalg::fvector>(intern::id_sens_data, 11);
    if (shmem::fail()) {
        logging::err(intern::mod,
            "Failed to read from channel data shared memory object"
        );
        logging::err(intern::mod,
            "Failed to read from channel"
        );
        intern::fail = true;
        intern::blank = false;
        return sens;
    }

    oupt = intern::conv_val_to_oupt(sens);
    logging::inf(intern::mod,
        "Read from channel data shared memory object: Value: ", oupt
    );

    intern::fail = false;
    intern::blank = false;
    return sens;
}

linalg::fvector get_updt (void) {
    bool flag;
    linalg::fvector updt;
    std::string oupt;

    logging::inf(intern::mod,
        "Reading from channel: Type: Update"
    );

    logging::inf(intern::mod,
        "Reading from channel status shared memory object"
    );

    flag = shmem::get<bool>(intern::id_updt_drdy);
    if (shmem::fail()) {
        logging::err(intern::mod,
            "Failed to read from channel status shared memory object"
        );
        logging::err(intern::mod,
            "Failed to read from channel"
        );
        intern::fail = true;
        intern::blank = false;
        return updt;
    }

    oupt = intern::conv_val_to_oupt(flag);
    logging::inf(intern::mod,
        "Read from channel status shared memory object: Value: ", oupt
    );

    if (!flag) {
        logging::wrn(intern::mod,
            "Channel content is blank"
        );
        intern::fail = false;
        intern::blank = true;
        return updt;
    }

    logging::inf(intern::mod,
        "Reading from channel data shared memory object"
    );

    updt = shmem::get<linalg::fvector>(intern::id_updt_data, 4);
    if (shmem::fail()) {
        logging::err(intern::mod,
            "Failed to read from channel data shared memory object"
        );
        logging::err(intern::mod,
            "Failed to read from channel"
        );
        intern::fail = true;
        intern::blank = false;
        return updt;
    }

    oupt = intern::conv_val_to_oupt(updt);
    logging::inf(intern::mod,
        "Read from channel data shared memory object: Value: ", oupt
    );

    intern::fail = false;
    intern::blank = false;
    return updt;
}

void clear (void) {
    intern::fail = false;

    logging::inf(intern::mod,
        "Clearing selected channels"
    );

    if (intern::req_ctrl) {
        logging::inf(intern::mod,
            "Writing to channel status shared memory object: Type: Control"
        );
        shmem::put(intern::id_ctrl_drdy, false);
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to write to channel status shared memory object"
            );
            intern::fail = true;
        }
    }

    if (intern::req_stat) {
        logging::inf(intern::mod,
            "Writing to channel status shared memory object: Type: State"
        );
        shmem::put(intern::id_stat_drdy, false);
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to write to channel status shared memory object"
            );
            intern::fail = true;
        }
    }

    if (intern::req_estm) {
        logging::inf(intern::mod,
            "Writing to channel status shared memory object: Type: Estimate"
        );
        shmem::put(intern::id_estm_drdy, false);
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to write to channel status shared memory object"
            );
            intern::fail = true;
        }
    }

    if (intern::req_sens) {
        logging::inf(intern::mod,
            "Writing to channel status shared memory object: Type: Sensing"
        );
        shmem::put(intern::id_sens_drdy, false);
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to write to channel status shared memory object"
            );
            intern::fail = true;
        }
    }

    if (intern::req_updt) {
        logging::inf(intern::mod,
            "Writing to channel status shared memory object: Type: Update"
        );
        shmem::put(intern::id_updt_drdy, false);
        if (shmem::fail()) {
            logging::err(intern::mod,
                "Failed to write to channel status shared memory object"
            );
            intern::fail = true;
        }
    }

    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to clear selected channels"
        );
    }
}

void acquire_acces (void) {
    logging::inf(intern::mod,
        "Acquiring access control semaphore"
    );

    sema::acquire(intern::id_acces);
    if (sema::fail()) {
        logging::err(intern::mod,
            "Failed to acquire access control semaphore"
        );
        intern::fail = true;
        return;
    }

    intern::fail = false;
}

void release_acces (void) {
    logging::inf(intern::mod,
        "Releasing access control semaphore"
    );

    sema::release(intern::id_acces);
    if (sema::fail()) {
        logging::err(intern::mod,
            "Failed to release access control semaphore"
        );
        intern::fail = true;
        return;
    }

    intern::fail = false;
}

}

namespace channel :: intern {

std::string conv_val_to_oupt (bool req) {
    std::string oupt;
    if (req) {
        oupt = "True";
    } else {
        oupt = "False";
    }
    return oupt;
}

std::string conv_val_to_oupt (double val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << std::showpos << std::scientific << std::setprecision(2) << val;
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

}
