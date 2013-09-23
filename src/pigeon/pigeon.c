#include <stdio.h>
#include <string.h>
#include "pigeon.h"

/*
 * Initializes the Pigeon VM's states.
 */
void init_vm(STATE* state) {
	/* Initialize the states */
	state->acc = 0;
	state->pc = 0;
	/* Call stack pointer */
	state->csp = 0;
	/* Data stack pointer */
	state->dsp = 0;
	
	int i = 0;
	
	/* Zero out the memory */
	for (i = 0; i < MEM_SIZE; i++)
		state->mem[i] = 0;
	
	/* Zero out the call and data stacks */
	for (i = 0; i < STACK_SIZE; i++) {
		state->cstack[i] = 0;
		state->dstack[i] = 0;
	}
}

/*
 * Fetches an instruction from the current memory location and loads it into
 * the `opcode` and `operand` registers.
 */
void fetch(STATE* state) {
	unsigned short instr = state->mem[state->pc];
	
	state->opcode = (instr & 0xF000) >> 12;
	state->operand = instr & 0x0FFF;
}

/*
 * Executes a single instruction.
 */
void execute(STATE* state) {

	unsigned short opcode = state->opcode;
	unsigned short operand = state->operand;
	unsigned short value = state->mem[operand]; 

	switch (opcode) {

		case ADD:
			state->acc += value;
			(state->pc)++;
			break;

		case SUB:
			state->acc -= value;
			(state->pc)++;
			break;

		case AND:
			state->acc &= value;
			(state->pc)++;
			break;

		case OR:
			state->acc |= value;
			(state->pc)++;
			break;

		case XOR:
			state->acc ^= value;
			(state->pc)++;
			break;

		case NOT:
			state->acc = ~state->acc;
			(state->pc)++;
			break;

		case LDM:
			state->acc = value;
			(state->pc)++;
			break;

		case LDI:
			state->acc = operand;
			(state->pc)++;
			break;

		case STR:
			state->mem[operand] = state->acc;
			(state->pc)++;
			break;

		case JMP:
			state->pc = operand;
			break;

		case JMZ:
			if (state->acc == 0)
				state->pc = operand;
			else
				(state->pc)++;
			break;

		case JMN:
			if (state->acc < 0)
				state->pc = operand;
			else
				(state->pc)++;
			break;

		case CALL:
			state->cstack[state->csp] = ++(state->pc);
			(state->csp)++;
			state->pc = operand;
			break;

		case RET:
			state->pc = state->cstack[--(state->csp)];
			break;

		case PUSH:
			state->dstack[(state->dsp)++] = state->acc;
			(state->pc)++;
			break;

		case POP:
			state->acc = state->dstack[--(state->dsp)];
			(state->pc)++;
			break;
	}
}

/*
 * Gets the mnemonic for the given opcode.
 */
void get_mnemonic(unsigned short opcode, char mnemonic[5]) {

	if (opcode == ADD)
		strcpy(mnemonic, S_ADD);

	else if (opcode == SUB)
		strcpy(mnemonic, S_SUB);

	else if (opcode == AND)
		strcpy(mnemonic, S_AND);

	else if (opcode == OR)
		strcpy(mnemonic, S_OR);

	else if (opcode == XOR)
		strcpy(mnemonic, S_XOR);

	else if (opcode == NOT)
		strcpy(mnemonic, S_NOT);

	else if (opcode == LDM)
		strcpy(mnemonic, S_LDM);

	else if (opcode == LDI)
		strcpy(mnemonic, S_LDI);

	else if (opcode == STR)
		strcpy(mnemonic, S_STR);

	else if (opcode == JMP)
		strcpy(mnemonic, S_JMP);

	else if (opcode == JMZ)
		strcpy(mnemonic, S_JMZ);

	else if (opcode == JMN)
		strcpy(mnemonic, S_JMN);

	else if (opcode == CALL)
		strcpy(mnemonic, S_CALL);

	else if (opcode == RET)
		strcpy(mnemonic, S_RET);

	else if (opcode == PUSH)
		strcpy(mnemonic, S_PUSH);

	else if (opcode == POP)
		strcpy(mnemonic, S_POP);
}

void run(STATE* state, void (*callback)(STATE*)) {

	int instruction_count = 0;
	const int MAX_INST_COUNT = 500;

	/*
	 * Run the VM.
	 * - Stops when the PC reaches 4096.
	 * - Stops when more than 500 instructions have been executed to stop out-of-control programs
	 */
	while (state->pc < MEM_SIZE && instruction_count < MAX_INST_COUNT) {
		instruction_count++;
		fetch(state);
		callback(state);
		execute(state);
	}
}
