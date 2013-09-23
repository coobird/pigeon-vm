Pigeon, an Accumulator-based Virtual Machine
============================================

Pigeon is an accumulator-based (1-operand) virtual machine.

The virtual machine consists of:

 * accumulator register
 * program counter
 * data stack
 * call stack
 * memory


Accumulator
-----------

The accumulator register is where results for arithmetric operations
to be stored.

It consists of a single 16-bit signed integer. The accumulator will wrap
around when it overflows or underflows.


Program Counter
---------------

The program counter indicates the current position of execution in the
memory space.

The virtual machine will fetch the instruction from the location indicated by
the program counter, and will execute that instruction.

The program counter increments by 1 the instruction from the indicated
location is executed, except in cases of jumps, calls and conditional jump
instructions.

When the program counter reaches the last memory address, the execution of the
virtual machine will terminate.


Data and Call stacks
--------------------

There are two 8-level call stacks: the data stack and call stack.

The call stack contains the return addresses of the caller, allowing
implementation of subroutine calls.

The data stack can contain arbituary data which can be pushed from, and popped
onto the accumulator.


Memory
------

The memory contains both the instruction and data in the same address space.

The Pigeon is a virtual machine with 12 bits addressible memory space. Each
addressible memory location containing a 12-bit value, resulting in 4096
memory locations.

Program execution for the Pigeon virtual machine begins at address 0.


Instruction Set
---------------

Each Pigeon virtual machine instruction consists of a 16-bit instruction,
where the upper 4-bits is used for the instruction (opcode) and the lower
12-bits used for the memory address (operand).

The instuction set consists of the following opcodes:

 * Arithmetic operations
  * `ADD` and `SUB`
 * Logical operations
  * `AND`, `OR`, `XOR` and `NOT`
 * Memory operations
  * `LDM`, `LDI`, and `STR`
 * Jump operations
  * `JMP`, `JMZ`, `JMN`
 * Function call operations
  * `CALL` and `RET`
 * Stack operations
  * `PUSH` and `POP`

```
ADD	memory-address
Adds the value contained in memory-address to the accumulator.

SUB	memory-address
Subtracts the value contained in memory-address from the accumulator.

AND	memory-address
Performs a logical AND operation between the value in memory-address and the
accumulator, then stores the result in the accumulator.

OR	memory-address
Performs a logical OR operation between the value in memory-address and the
accumulator, then stores the result in the accumulator.

XOR	memory-address
Performs a logical XOR operation between the value in memory-address and the
accumulator, then stores the result in the accumulator.

NOT
Performs a logical NOT operation of the value contained in the accumulator
and stores the value back to the accumulator.

LDM	memory-address
Loads a value from memory-address into the accumulator.

LDI	operand
Loads a value of operand into the accumulator.

STR	memory-address
Stores the value in the accumulator to the memory-address.

JMP	memory-address
Jumps to memory-address and begins execution.

JMZ	memory-address
If the accumulator contains zero (0), then jump to memory-address and begins
execution.

JMN	memory-address
If the accumulator contains a negative value, then jump to memory-address and
begins execution.

CALL	memory-address
Calls a subroutine at memory-address, stores the return address to the call
stack. The return address is the current address + 1.

RETURN
Returns to the memory address at the top of the call stack.

PUSH
Pushes the value of the accumulator to the stack.

POP
Pops the value off the stack onto the accumulator.
```
