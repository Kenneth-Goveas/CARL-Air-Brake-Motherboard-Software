#ifndef __IPC_CHANNEL_HPP__
#define __IPC_CHANNEL_HPP__

#include <math/linalg.hpp>

namespace channel {

bool fail (void);
bool blank (void);

void init (bool want_ctrl, bool want_stat, bool want_estm, bool want_sens);

bool exists (void);

void create (void);
void remove (void);

void open (void);
void close (void);

void put_ctrl (double ctrl);
void put_stat (linalg::fvector stat);
void put_estm (linalg::fvector estm);
void put_sens (linalg::fvector sens);

double get_ctrl (void);
linalg::fvector get_stat (void);
linalg::fvector get_estm (void);
linalg::fvector get_sens (void);

void clear (void);

void acquire_acces (void);
void release_acces (void);

}

#endif
