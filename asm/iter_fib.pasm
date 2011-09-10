######################################
# A fibonacci number (iterative) calculator
######################################
# Variables
100	DATA$ONE	1
101	DATA$ACC	0
102	DATA$START	0
103	DATA$END	23
104	DATA$C0	0
105	DATA$C1	1
106	DATA$SUM	0
200	DATA$RESULTS	0
4095	DATA$TERMINATE	0
# Start execution
# - sets the start condition i = 0
0	LDM	$START
1	STR	$ACC
# - loads the current i value
2	LDM	$ACC
3	ADD	$ONE
4	STR	$ACC
# - check the condition i < END
5	LDM	$END
6	SUB	$ACC
7	JMZ	$RESULTS
# - if there's anything to do, do it here
8	LDI	0
9	ADD	$C0
10	ADD	$C1
11	STR	$SUM
12	LDM	$C1
13	STR	$C0
14	LDM	$SUM
15	STR	$C1
# - loop!
16	JMP	2
# - show fib
200	LDM	$C1
201	JMP	$TERMINATE
END
