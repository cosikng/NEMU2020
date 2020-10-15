#include "cpu/helper.h"
#ifndef __POP_H__
#define __POP_H__

make_helper(pop_rm_w);
make_helper(pop_rm_l);
make_helper(pop_rm_v);

make_helper(pop_r16);
make_helper(pop_r32);

/*make_helper(pop_ds);
make_helper(pop_es);
make_helper(pop_ss);
make_helper(pop_fs);
make_helper(pop_gs);*/

#endif
