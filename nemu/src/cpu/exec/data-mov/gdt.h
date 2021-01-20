#ifndef __GDT_H__
#define __GDT_H__

#include "cpu/exec/helper.h"

make_helper(lgdt);
make_helper(lidt);

#endif
