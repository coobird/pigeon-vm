/*
 ============================================================================
 Name        : Pigeon Virtual Machine
 Author      : 
 Version     :
 Copyright   : 
 Description : 
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "pigeon/pigeon.h"
#include "asm/asm.h"

int main() {
	
	STATE state;
	
	printf("Initializing VM state.\n");
	init_vm(&state);

	printf("Loading instructions from stdin.\n");
	load_memory(&state);

	printf("Beginning execution.\n");
	run(&state);

	printf("Finished execution.\n");
	
	return 0;
}
