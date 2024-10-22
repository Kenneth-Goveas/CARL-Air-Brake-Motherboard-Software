#ifndef __IPC_CHANNEL_HPP__
#define __IPC_CHANNEL_HPP__

#include <math/linalg.hpp>

namespace channel {

bool fail (void);
bool blank (void);

void init (
    bool req_ctrl, bool req_stat, bool req_estm, bool req_sens, bool req_updt
);

bool exists (void);

void create (void);
void remove (void);

void open (void);
void close (void);

void put_ctrl (double ctrl);
void put_stat (linalg::fvector stat);
void put_estm (linalg::fvector estm);
void put_sens (linalg::fvector sens);
void put_updt (linalg::fvector updt);

double get_ctrl (void);
linalg::fvector get_stat (void);
linalg::fvector get_estm (void);
linalg::fvector get_sens (void);
linalg::fvector get_updt (void);

void clear (void);

void acquire_acces (void);
void release_acces (void);

}

#endif
