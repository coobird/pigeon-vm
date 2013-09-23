#include <stdio.h>
#include <stdlib.h>
#include "pigeon/pigeon.h"
#include "asm/asm.h"

void output_state(STATE* state) {
	char opcode[5];
	get_mnemonic(state->opcode, opcode);

	/* JSON
	printf("{\"pc\":%i,\"opcode\":\"%s\",\"operand\":%i,\"acc\":%i,\"csp\":%i,\"dsp\":%i}\n",
		state->pc, opcode, state->operand, state->acc, state->csp, state->dsp);
	*/

	printf("pc: %4i  opcode: %s operand: %4i  acc: %5i  csp: %2i  dsp: %2i\n",
		state->pc, opcode, state->operand, state->acc, state->csp, state->dsp);
}

int main() {
	
	STATE state;
	
	printf("Initializing VM state.\n");
	init_vm(&state);

	printf("Loading instructions from stdin.\n");
	load_memory(&state);

	printf("Beginning execution.\n");
	run(&state, output_state);

	printf("Finished execution.\n");
	
	return 0;
}
