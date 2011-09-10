#include <stdio.h>
#include <string.h>
#include "pigeon.h"

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

void fetch(STATE* state) {
	unsigned short instr = state->mem[state->pc];
	
	state->opcode = (instr & 0xF000) >> 12;
	state->operand = instr & 0x0FFF;
}

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

void run(STATE* state) {
	int i = 0;
	char opcode[5];
	unsigned short op;

	/*
	 * Run the VM.
	 * - Stops when the PC reaches 4096.
	 * - Stops when more than 500 instructions have been executed to stop out-of-control programs
	 */
	while (state->pc < MEM_SIZE && i < 500) {
		i++;	
		fetch(state);

		op = state->opcode;
		if (op == ADD)
			strcpy(opcode, S_ADD);

		else if (op == SUB)
			strcpy(opcode, S_SUB);

		else if (op == AND)
			strcpy(opcode, S_AND);

		else if (op == OR)
			strcpy(opcode, S_OR);

		else if (op == XOR)
			strcpy(opcode, S_XOR);

		else if (op == NOT)
			strcpy(opcode, S_NOT);

		else if (op == LDM)
			strcpy(opcode, S_LDM);

		else if (op == LDI)
			strcpy(opcode, S_LDI);

		else if (op == STR)
			strcpy(opcode, S_STR);

		else if (op == JMP)
			strcpy(opcode, S_JMP);

		else if (op == JMZ)
			strcpy(opcode, S_JMZ);

		else if (op == JMN)
			strcpy(opcode, S_JMN);

		else if (op == CALL)
			strcpy(opcode, S_CALL);

		else if (op == RET)
			strcpy(opcode, S_RET);

		else if (op == PUSH)
			strcpy(opcode, S_PUSH);

		else if (op == POP)
			strcpy(opcode, S_POP);

		/* JSON
		printf("{\"pc\":%i,\"opcode\":\"%s\",\"operand\":%i,\"acc\":%i,\"csp\":%i,\"dsp\":%i}\n", state->pc, opcode, state->operand, state->acc, state->csp, state->dsp);
		*/

		printf("pc: %4i  opcode: %s  operand: %4i  acc: %5i  csp: %2i  dsp: %2i\n", state->pc, opcode, state->operand, state->acc, state->csp, state->dsp);
		execute(state);
	}
}
