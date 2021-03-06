##############################################################################
# A for-loop program using a stack
# ----------------------------------
# This program uses the stack to store the loop index.
# 
##############################################################################

# Variables
100	DATA$ONE	1
101	DATA$START	0
102	DATA$END	5
20	DATA$CLEANUP	0
4095	DATA$TERMINATE	0

# Equivalent C code
# -------------------
# for (i = 0; i < 5; i++) {
#   accumulator = 777;
# }
# - sets the start condition i = 0

# Start execution
0	LDM	$START
1	PUSH

# - loads the current i value to the stack
2	POP
3	ADD	$ONE
4	PUSH

# - check the condition i < 5, if it is not, jump
5	SUB	$END
6	JMZ	$CLEANUP

# - if there's anything to do, do it here
7	LDI	777

# - loop!
8	JMP	2

# Clean up the stack and accumulator
20	POP
21	LDI	0
22	JMP	$TERMINATE
