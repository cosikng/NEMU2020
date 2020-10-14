#include "cpu/helper.h"
#ifndef __CALL_H__
#define __CALL_H__

//make_helper(call_i_d);
int call_i_d(swaddr_t eip){
    printf("Hello\n");
    return 1;
}

#endif
