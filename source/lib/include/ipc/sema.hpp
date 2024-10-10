#ifndef __IPC_SEMA_HPP__
#define __IPC_SEMA_HPP__

#include <string>

namespace sema {

bool fail (void);

bool exists (std::string name);

void create (std::string name);
void remove (std::string name);

int open (std::string name);
void close (void);

void acquire (int id);
void release (int id);

}

#endif
