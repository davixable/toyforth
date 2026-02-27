/*
 * Execution Engine Module
 *
 * Implements the core interpreter loop that executes a compiled program
 * list on a ToyForth virtual machine.
 */

#ifndef ENGINE_H
#define ENGINE_H

#include "tforth.h"


/*
 * execute() - Executes a compiled program list on the virtual machine
 *
 * Iterates through each object in the compiled program list and executes it:
 *   - TF_OBJ_INT/BOOL: Pushed onto the data stack
 *   - TF_OBJ_SYMBOL: Looked up in the operation dictionary and executed
 *
 * Unknown operations cause immediate program termination with error message.
 *
 * Args:
 *   program_list - Compiled program (TF_OBJ_LIST of executable objects)
 *   context      - VM execution context (contains the data stack)
 */
void execute(tfobj *program_list, tfcontext *context);


#endif  