#ifndef PIGEON_H_
#define PIGEON_H_

/*
 * Stack size for the call and data stacks.
 * This is an arbituary size.
 */
#define STACK_SIZE 8

/*
 * Memory size.
 * The Pigeon VM is a 16-bit VM which can address 12-bit memory addresses.
 * Therefore, we can have 4096 bytes of memory.
 */
#define MEM_SIZE 4096

/*
 * Opcodes.
 */
#define ADD		0
#define SUB		1
#define	AND		2
#define	OR		3
#define	XOR		4
#define	NOT		5
#define	LDM		6
#define	LDI		7
#define	STR		8
#define	JMP		9
#define	JMZ		10
#define	JMN		11
#define	CALL	12
#define	RET		13
#define PUSH	14
#define POP		15

/*
 * Mnemonics for the opcodes.
 */
#define S_ADD	"ADD"
#define S_SUB	"SUB"
#define S_AND	"AND"
#define S_OR	"OR"
#define S_XOR	"XOR"
#define S_NOT	"NOT"
#define S_LDM	"LDM"
#define S_LDI	"LDI"
#define S_STR	"STR"
#define S_JMP	"JMP"
#define S_JMZ	"JMZ"
#define S_JMN	"JMN"
#define S_CALL	"CALL"
#define S_RET	"RET"
#define S_PUSH	"PUSH"
#define S_POP	"POP"

/*
 * A struct which describes the VM's state.
 */
typedef struct state {

	/*
	 * Opcode and Operand.
	 * - Opcode:   4 bits
	 * - Operand: 12 bits
	 *
	 * See the above "define" macros for the opcodes.
	 */
	unsigned short opcode: 4;
	unsigned short operand: 12;
	
	/* Accumulator */
	signed short acc;

	/* Program counter */
	unsigned short pc;
	
	/* Call stack */
	unsigned short cstack[STACK_SIZE];

	/* Data stack */
	unsigned short dstack[STACK_SIZE];

	/* Call stack pointer */
	unsigned char csp;

	/* Data stack pointer */
	unsigned char dsp;
	
	/* Memory */
	signed short mem[MEM_SIZE];

} STATE;

void init_vm(STATE* state);
void fetch(STATE* state);
void execute(STATE* state);
void run(STATE* state, void (*callback)(STATE*));

#endif /*PIGEON_H_*/
