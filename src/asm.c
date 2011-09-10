/*
 * asm.c
 *
 * Reads Pigeon VM assembly code from standard input, and loads that up into
 * the VM memory.
 *
 *  Created on: 2010/10/31
 *      Author: coobird
 */

#include <stdio.h>
#include "pigeon.h"
#include <string.h>
#include <stdlib.h>

/*
 * An entry on the symbol table which maps the variable name to its value.
 */
typedef struct {
	char* varname;
	int value;
} ENTRY;

/*
 * Symbol table and current symbol table size.
 */
static ENTRY entries[100];
static int entry_size = 0;

void place_entry(char* varname, int address, ENTRY* entries, int entry_num) {
	entries[entry_num].varname = varname;
	entries[entry_num].value = address;
}

int get_entry(char* varname, ENTRY* entries, int entry_size) {
	int i = 0;
	for (i = 0; i < entry_size; i++) {
		if (strcmp(varname, entries[i].varname) == 0) {
			return entries[i].value;
		}
	}
	return -1;
}

void parse(char* line, STATE* state) {

	/* skip a comment line */
	if (line[0] == '#') {
		return;
	}

	int address;
	char opcode[16];
	char value_buffer[16];
	int value;

	sscanf(line, "%i\t%s\t%s", &address, opcode, value_buffer);

	/* Do variable substitution.
	 *
	 * The *address* of the variable will be substituted into a variable,
	 * not the actual value. Therefore, this will not work on LDI, but will
	 * work with LDM.
	 */
	if (value_buffer[0] == '$') {
		value = get_entry(value_buffer, entries, entry_size);

	} else {
		value = atoi(value_buffer);
	}


	/* If the line is data, handle it. If it's a variable handle it.
	 *
	 * A variable declaration should be in the form:
	 * <address>	DATA$<var_name>	<value>
	 */
	int opcode_value;
	if (strncmp("DATA", opcode, 4) == 0) {

		if (opcode[4] == '$') {
			char* varname = malloc(12);
			int i = 0;
			for (i = 0; opcode[i + 4] != '\0' || i > 12; i++) {
				varname[i] = opcode[i + 4];
			}
			varname[i] = '\0';
			place_entry(varname, address, entries, entry_size++);
		}

		state->mem[address] = value;
		return;
	}

	else if (strcmp(S_ADD, opcode) == 0)
		opcode_value = ADD;

	else if (strcmp(S_SUB, opcode) == 0)
		opcode_value = SUB;

	else if (strcmp(S_AND, opcode) == 0)
		opcode_value = AND;

	else if (strcmp(S_OR, opcode) == 0)
		opcode_value = OR;

	else if (strcmp(S_XOR, opcode) == 0)
		opcode_value = XOR;

	else if (strcmp(S_NOT, opcode) == 0)
		opcode_value = NOT;

	else if (strcmp(S_LDM, opcode) == 0)
		opcode_value = LDM;

	else if (strcmp(S_LDI, opcode) == 0)
		opcode_value = LDI;

	else if (strcmp(S_STR, opcode) == 0)
		opcode_value = STR;

	else if (strcmp(S_JMP, opcode) == 0)
		opcode_value = JMP;

	else if (strcmp(S_JMZ, opcode) == 0)
		opcode_value = JMZ;

	else if (strcmp(S_JMN, opcode) == 0)
		opcode_value = JMN;

	else if (strcmp(S_CALL, opcode) == 0)
		opcode_value = CALL;

	else if (strcmp(S_RET, opcode) == 0)
		opcode_value = RET;

	else if (strcmp(S_PUSH, opcode) == 0)
		opcode_value = PUSH;

	else if (strcmp(S_POP, opcode) == 0)
		opcode_value = POP;

	else {
		printf("Unknown opcode: %s\n", opcode);
		return;
	}

	state->mem[address] = (opcode_value << 12) + value;
}

void load_memory(STATE* state) {
	char line[80];

	int linenum = 0;

	while (!feof(stdin)) {
		memset(line, 0, 80);
		fgets(line, 79, stdin);

		printf("LINE(%4i): %s", ++linenum, line);

		if (strncmp(line, "END", 3) == 0) {
			break;
		}
		parse(line, state);
	}
}