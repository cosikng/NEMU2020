#include "cpu/exec/helper.h"

make_helper(exec);

make_helper(rep) {
	int len = 0;
	int count = 0;
	if(instr_fetch(eip + 1, 1) == 0xc3) {
		/* repz ret */
		exec(eip + 1);
		len = 0;
	}
	else {
		if(instr_fetch(eip + 1, 1) == 0x66) len = 2;
		else len = 1;
		while(cpu.ecx) {
			exec(eip + 1);
			count ++;
			cpu.ecx --;
			/*assert(ops_decoded.opcode == 0xa4	// movsb
				|| ops_decoded.opcode == 0xa5	// movsw
				|| ops_decoded.opcode == 0xaa	// stosb
				|| ops_decoded.opcode == 0xab	// stosw
				|| ops_decoded.opcode == 0xa6	// cmpsb
				|| ops_decoded.opcode == 0xa7	// cmpsw
				|| ops_decoded.opcode == 0xae	// scasb
				|| ops_decoded.opcode == 0xaf	// scasw
				);*/
			if((ops_decoded.opcode == 0xa6
				|| ops_decoded.opcode == 0xa7
				|| ops_decoded.opcode == 0xae
				|| ops_decoded.opcode == 0xaf) && cpu.eflags.ZF == 0) break;

			/* TODO: Jump out of the while loop if necessary. */

		}
	}

#ifdef DEBUG
	char temp[80];
	if(count != 0) sprintf(temp, "rep %s", assembly);
	else sprintf(temp, "rep");
	sprintf(assembly, "%s[cnt = %d]", temp, count);
#endif
	
	return len + 1;
}

make_helper(repnz) {
	int count = 0;
	int len = 0;
	if(instr_fetch(eip + 1, 1) == 0x66) len = 2;
	else len = 1;
	while(cpu.ecx) {
		exec(eip + 1);
		count ++;
		cpu.ecx --;
		if((ops_decoded.opcode == 0xa6
				|| ops_decoded.opcode == 0xa7
				|| ops_decoded.opcode == 0xae
				|| ops_decoded.opcode == 0xaf) && cpu.eflags.ZF == 1) break;


		/* TODO: Jump out of the while loop if necessary. */

	}

#ifdef DEBUG
	char temp[80];
	if(count != 0) sprintf(temp, "rep %s", assembly);
	else sprintf(temp, "rep");
	sprintf(assembly, "%s[cnt = %d]", temp, count);
#endif

	return len + 1;
}
