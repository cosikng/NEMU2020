#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "push-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */
make_helper(push_v_l)
{
    int byte;
    if(ops_decoded.is_operand_size_16) byte = 2;
    else byte = 4;
    int index = swaddr_read(cpu.eip, 1) - 0x50;
	cpu.esp -= byte;
	swaddr_write(cpu.esp, byte, cpu.gpr[index]._32);
	print_asm_template1();
	return 1;
}

make_helper_v(push_rm)
make_helper_v(push_i)

