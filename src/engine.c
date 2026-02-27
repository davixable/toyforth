/*
 * Execution Engine Implementation
 *
 * Implements the core interpreter loop that executes a pre-compiled program
 * list on a ToyForth virtual machine. The engine maintains VM state through
 * an execution context and processes each compiled object in sequence.
 */

#include <stdio.h>
#include <stdlib.h>

#include "engine.h"
#include "dictionary.h"
#include "stack.h"


void execute(tfobj *program_list, tfcontext *context) {
    if (program_list == NULL || context == NULL) return;
     
    for (size_t i = 0; i < program_list->list_obj.len; i++) {
        tfobj *object = program_list->list_obj.element[i];

        if (object->type == TF_OBJ_INT || object->type == TF_OBJ_BOOL) {
            stackPush(context, object);
        } 
        else if (object->type == TF_OBJ_SYMBOL) {
            Operation op = lookupOperation(object->str_obj.str);
            
            if (op != NULL) {
                op(context);
            } else {
                fprintf(stderr, "Unknown word: %s\n", object->str_obj.str);
                exit(EXIT_FAILURE);
            }
        }
        else {
            fprintf(stderr, "Found an unexecutable object during execution.\n");
            exit(EXIT_FAILURE);
        }
    }
}