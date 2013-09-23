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
#include "../pigeon/pigeon.h"
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

/*
 * Place an entry into the symbol table.
 */
void place_entry(char* varname, int address, ENTRY* entries, int entry_num) {
	entries[entry_num].varname = varname;
	entries[entry_num].value = address;
}

/*
 * Gets an entry from the symbol table.
 */
int get_entry(char* varname, ENTRY* entries, int entry_size) {
	int i = 0;
	for (i = 0; i < entry_size; i++) {
		if (strcmp(varname, entries[i].varname) == 0) {
			return entries[i].value;
		}
	}
	return -1;
}

/*
 * Parses a single line of Pigeon Assembly.
 */
void parse(char* line, STATE* state) {

	/* Skip comment lines and blank lines */
	if (line[0] == '#' || line[0] == '\r' || line[0] == '\n') {
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
		fprintf(stderr, "Unknown opcode: %s\n", opcode);
		return;
	}

	state->mem[address] = (opcode_value << 12) + value;
}

/*
 * Reads a line from the FILE handle.
 * Based on an answer by Marlon (http://stackoverflow.com/users/282474/marlon) on a Stack Overflow question:
 * - http://stackoverflow.com/questions/5597513/line-by-line-reading-in-c-and-c/5597814#5597814
 */
char* _getline(FILE* in) {
	int capacity = 80;
	int length = 0;
	char* line = malloc(capacity * sizeof(char));

	while (!feof(in)) {
		fgets(&line[length], capacity - length, in);

		length = strlen(line);
		if (!feof(in) && line[length - 1] != '\n') {
			capacity = capacity * 2;
			line = realloc(line, capacity * sizeof(char));
		} else {
			return line;
		}
	}
}

/*
 * Runs the routine to load Pigeon Assembly instructions from the given FILE
 * handle into the Pigeon VM's memory.
 */
void load_memory(FILE* in, STATE* state, int verbose) {

	int linenum = 0;
	int f_break = 0;

	while (!feof(in)) {
		linenum++;
		char* line = _getline(in);

		if (verbose) printf("LINE(%4i): %s", linenum, line);

		if (strncmp(line, "END", 3) == 0) {
			f_break = 1;
		}

		if (!f_break) parse(line, state);

		free(line);
		if (f_break) break;
	}
}
