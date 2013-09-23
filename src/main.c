#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pigeon/pigeon.h"
#include "asm/asm.h"

void output_state_json(STATE* state) {
	char opcode[5];
	get_mnemonic(state->opcode, opcode);

	printf("{\"pc\":%i,\"opcode\":\"%s\",\"operand\":%i,\"acc\":%i,\"csp\":%i,\"dsp\":%i}\n",
		state->pc, opcode, state->operand, state->acc, state->csp, state->dsp);
}

void output_state_text(STATE* state) {
	char opcode[5];
	get_mnemonic(state->opcode, opcode);

	printf("pc: %4i  opcode: %s operand: %4i  acc: %5i  csp: %2i  dsp: %2i\n",
		state->pc, opcode, state->operand, state->acc, state->csp, state->dsp);
}

void output_state_quiet(STATE* state) {
}

int main(int argc, char** argv) {
	void* callback = &output_state_text;

	int f_verbose = 0, f_json = 0, f_text = 0, f_quiet = 0, f_output = 0;
	int cnt_outflags = 0;

	int opt;
	while ((opt = getopt(argc, argv, "ovqjt")) != -1) {
		switch (opt) {
			case 'v':
				f_verbose = 1;
				break;
			case 'o':
				f_output = 1;
				break;
			case 'q':
				f_quiet = 1;
				cnt_outflags++;
				break;
			case 'j':
				f_json = 1;
				cnt_outflags++;
				break;
			case 't':
				f_text = 1;
				cnt_outflags++;
				break;
			default:
				return -1;
				;
		}
	}

	if (cnt_outflags > 1) {
		printf("Can only speficy one of -j, -t, or -q.\n");
		return -1;
	}

	if (f_json) callback = &output_state_json;
	if (f_quiet) callback = &output_state_quiet;
	
	STATE state;
	
	if (f_verbose) printf("Initializing VM state.\n");
	init_vm(&state);

	if (f_verbose) printf("Loading instructions from stdin.\n");
	load_memory(&state, f_verbose);

	if (f_verbose) printf("Beginning execution.\n");
	run(&state, callback);

	if (f_verbose) printf("Finished execution.\n");
	
	if (f_output) printf("%i\n", state.acc);

	return 0;
}
