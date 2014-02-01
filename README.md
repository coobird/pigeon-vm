Pigeon - an accumulator-based virtual machine
============================================

Pigeon is an accumulator-based (1-operand) virtual machine written in C.

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

When the program counter reaches the last memory address (address 4095), 
the virtual machine will terminate.


Data and Call Stacks
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

The following is a description of each instruction.

* `ADD	<memory-address>`
 * Adds the value contained in memory-address to the accumulator.
* `SUB	<memory-address>`
 * Subtracts the value contained in memory-address from the accumulator.
* `AND	<memory-address>`
 * Performs a logical AND operation between the value in memory-address and the
   accumulator, then stores the result in the accumulator.
* `OR	<memory-address>`
 * Performs a logical OR operation between the value in memory-address and the
   accumulator, then stores the result in the accumulator.
* `XOR	<memory-address>`
 * Performs a logical XOR operation between the value in memory-address and the
   accumulator, then stores the result in the accumulator.
* `NOT`
 * Performs a logical NOT operation of the value contained in the accumulator
    and stores the value back to the accumulator.
* `LDM	<memory-address>`
 * Loads a value from memory-address into the accumulator.* 
* `LDI	<operand>`
 * Loads a value of operand into the accumulator.
* `STR	<memory-address>`
 * Stores the value in the accumulator to the memory-address.
* `JMP	<memory-address>`
 * Jumps to memory-address and continues execution.
* `JMZ	<memory-address>`
 * If the accumulator contains zero (0), then jump to memory-address and begins
   execution.
* `JMN	<memory-address>`
 * If the accumulator contains a negative value, then jump to memory-address and
   begins execution.
* `CALL	<memory-address>`
 * Calls a subroutine at memory-address, stores the return address to the call
   stack. The return address is the current address + 1.
* `RETURN`
 * Returns to the memory address at the top of the call stack.
* `PUSH`
 * Pushes the value of the accumulator to the stack.
* `POP`
 * Pops the value off the stack onto the accumulator.


Programming Pigeon
------------------

Programming Pigeon is accomplished by writing Pigeon Assembly (pasm) code.

Each line consists of:

```
<memory-address> <tab> <opcode> <tab> [operand]
```

Each component must be delimited by a tab character.

The `memory-address` is a value from `0` to `4095` corresponding to the
address of the 12-bit memory space that the Pigeon has.

The `opcode` and `operand` is as described in the previous section.

In addition, "variables" can be mapped to specific memory addresses to
assist in writing Pigeon Assembly code.

A line that declares a variable is in the following format:

```
<memory-address> <tab> DATA$<variable-name> <tab> <init-value>
```

The variable name will be mapped to the specified memory address, and the
memory address will be initialized with the specified value.

An example of use of a variable can be seen in the following example:

```
# Varible declaration
100	DATA$VAR	42

# Program
0	LDM	$VAR
1	JMP	4095
END
```

The above program will load the value `42` from memory address `100` to the
accumulator by using the "load from memory" (`LDM`) instruction.

The program terminates after jumping to address 4095.

The `END` indicates that the end of the program has been reached, and the VM
can start execution.

Also note that comments can be written by having a `#` character as the first
character of a line.


Executing Pigeon
----------------

First, Pigeon can be built by simply running `make`:

```
$ make
gcc -o src/main.o -c src/main.c
gcc -o src/pigeon/pigeon.o -c src/pigeon/pigeon.c
gcc -o src/asm/asm.o -c src/asm/asm.c
gcc -o pigeon-vm src/main.o src/pigeon/pigeon.o src/asm/asm.o 
```

Running the VM is as simple as running `./pigeon-vm`.

At this point, the VM will wait for instructions to run from `STDIN`.

Try copy-and-pasting the example code from the previous section.

This should now show some output from Pigeon:

```
$ ./pigeon-vm 
# Varible declaration
100	DATA$VAR	42

# Program
0	LDM	$VAR
1	JMP	4095
END
pc:    0  opcode: LDM operand:  100  acc:     0  csp:  0  dsp:  0
pc:    1  opcode: JMP operand: 4095  acc:    42  csp:  0  dsp:  0
pc: 4095  opcode: JMP operand: 4095  acc:    42  csp:  0  dsp:  0
$
```

There are a few example programs present in the `asm` directory.
Those can be run by Pigeon by piping the program through to the VM:

```
$ cat asm/for_loop.pasm | ./pigeon-vm 
pc:    0  opcode: LDM operand:  102  acc:     0  csp:  0  dsp:  0
pc:    1  opcode: STR operand:  101  acc:     0  csp:  0  dsp:  0
pc:    2  opcode: LDM operand:  101  acc:     0  csp:  0  dsp:  0

  ... snip ...

pc:    6  opcode: SUB operand:  101  acc:     5  csp:  0  dsp:  0
pc:    7  opcode: JMZ operand: 4095  acc:     0  csp:  0  dsp:  0
pc: 4095  opcode: JMZ operand: 4095  acc:     0  csp:  0  dsp:  0
$
```
