######################################
# A for-loop program.
######################################
# Variables
100	DATA$ONE	1
101	DATA$ACC	0
102	DATA$START	0
103	DATA$END	5
4095	DATA$TERMINATE	0
# Start execution
# for (i = 0; i < 5; i++) {
#   accumulator = 777
# }
# - sets the start condition i = 0
0	LDM	$START
1	STR	$ACC
# - loads the current i value
2	LDM	$ACC
3	ADD	$ONE
4	STR	$ACC
# - check the condition i < 5
5	LDM	$END
6	SUB	$ACC
7	JMZ	$TERMINATE
# - if there's anything to do, do it here
8	LDI	777
# - loop!
9	JMP	2
END
