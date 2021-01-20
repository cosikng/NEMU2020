#include "cpu/exec/helper.h"

uint32_t pio_read(ioaddr_t, size_t);

make_helper(in)
{
    int index;
    index = instr_fetch(cpu.eip, 1) & 0xf;
    if (index < 6)
    {
        if (index == 4)
        {
            reg_b(0) = pio_read(instr_fetch(cpu.eip + 1, 1), 1);
        }
        else
        {
            if (ops_decoded.is_operand_size_16)
            {
                reg_w(0) = pio_read(instr_fetch(cpu.eip + 1, 1), 2);
            }
            else
            {
                reg_l(0) = pio_read(instr_fetch(cpu.eip + 1, 1), 4);
            }
        }
        return 2;
    }
    else
    {
        if (index == 0xc)
        {
            reg_b(0) = pio_read(reg_w(R_DX), 1);
        }
        else
        {
            if (ops_decoded.is_operand_size_16)
            {
                reg_w(0) = pio_read(reg_w(R_DX), 2);
            }
            else
            {
                reg_l(0) = pio_read(reg_w(R_DX), 4);
            }
        }
        return 1;
    }
}