#include "cpu/exec/helper.h"

uint32_t pio_write(ioaddr_t, size_t, uint32_t);

make_helper(out)
{
    int index;
    index = instr_fetch(cpu.eip, 1) & 0xf;
    if (index < 8)
    {
        if (index == 6)
        {
            pio_write(instr_fetch(cpu.eip + 1, 1), 1, reg_b(0));
        }
        else
        {
            if (ops_decoded.is_operand_size_16)
            {
                pio_write(instr_fetch(cpu.eip + 1, 1), 2, reg_w(0));
            }
            else
            {
                pio_write(instr_fetch(cpu.eip + 1, 1), 4, reg_l(0));
            }
        }
        return 2;
    }
    else
    {
        if (index == 0xe)
        {
            pio_write(reg_w(R_DX), 1, reg_b(0));
        }
        else
        {
            if (ops_decoded.is_operand_size_16)
            {
                pio_write(reg_w(R_DX), 2, reg_w(0));
            }
            else
            {
                pio_write(reg_w(R_DX), 4, reg_l(0));
            }
        }
        return 1;
    }
}