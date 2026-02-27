/*
 * Forth Primitive Operations Implementation
 *
 * Implements built-in Forth words: arithmetic operations, I/O, and
 * stack manipulation. Each operation pops operands, performs computation,
 * and may push results. All operations handle reference counting and may
 * trigger garbage collection.
 */

#include <stdio.h>
#include <stdlib.h>

#include "ops.h"
#include "stack.h"
#include "mem.h"


/*
 * operationAdd() implementation
 *
 * ( a b -- a+b )
 *
 * Pops two values from the stack. If both are integers, computes their
 * sum, pushes the result, and cleans up operands via reference counting.
 * No operation occurs if either operand is not an integer.
 */
void operationAdd(tfcontext *context) {
    tfobj *b = stackPop(context);
    tfobj *a = stackPop(context);

    if (a->type == TF_OBJ_INT && b->type == TF_OBJ_INT) {           
        tfobj *result = createIntegerObject(a->number + b->number);
        stackPush(context, result);
        /* Release local reference: the stack now owns it */
        decrementReferenceCount(result);
    }

    decrementReferenceCount(a);
    decrementReferenceCount(b);
}

/*
 * operationSub() implementation
 *
 * ( a b -- a-b )
 *
 * Pops two values from the stack. If both are integers, computes a minus b,
 * pushes the result, and cleans up operands. No operation if not both integers.
 */
void operationSub(tfcontext *context) {
    tfobj *b = stackPop(context);
    tfobj *a = stackPop(context);

    if (a->type == TF_OBJ_INT && b->type == TF_OBJ_INT) {
        tfobj *result = createIntegerObject(a->number - b->number);
        stackPush(context, result);
        decrementReferenceCount(result);
    }

    decrementReferenceCount(a);
    decrementReferenceCount(b);
}

/*
 * operationMul() implementation
 *
 * ( a b -- a*b )
 *
 * Pops two values from the stack. If both are integers, computes their
 * product, pushes the result, and cleans up operands.
 */
void operationMul(tfcontext *context) {
    tfobj *b = stackPop(context);
    tfobj *a = stackPop(context);

    if (a->type == TF_OBJ_INT && b->type == TF_OBJ_INT) {
        tfobj *result = createIntegerObject(a->number * b->number);
        stackPush(context, result);
        decrementReferenceCount(result);
    }

    decrementReferenceCount(a);
    decrementReferenceCount(b);
}

/*
 * operationDiv() implementation
 *
 * ( a b -- a/b )
 *
 * Pops two integer values and computes integer division (a/b).
 * Terminates with error on division by zero.
 * Cleans up operands via reference counting.
 */
void operationDiv(tfcontext *context) {
    tfobj *b = stackPop(context);
    tfobj *a = stackPop(context);

    if (a->type == TF_OBJ_INT && b->type == TF_OBJ_INT) {
        if (b->number == 0) {                                      
            fprintf(stderr, "Division by zero error.\n");
            exit(EXIT_FAILURE);
        }
        tfobj *result = createIntegerObject(a->number / b->number);
        stackPush(context, result);
        decrementReferenceCount(result);
    }

    decrementReferenceCount(a);
    decrementReferenceCount(b);
}


/*
 * operationPrint() implementation
 *
 * ( a -- )
 *
 * Pops the top of the stack and prints it in a human-readable format.
 * Output is type-dependent:
 *   - Integer: Decimal followed by space
 *   - String:  String contents followed by space
 *   - Boolean: "TRUE" or "FALSE" followed by space
 *
 * The popped value is destroyed by reference counting.
 */
void operationPrint(tfcontext *context) {
    tfobj *object = stackPop(context);

    if (object->type == TF_OBJ_INT) {
        printf("%d ", object->number);
    } else if (object->type == TF_OBJ_STR) {
        printf("%s ", object->str_obj.str);
    } else if (object->type == TF_OBJ_BOOL) {
        printf("%s ", object->number ? "TRUE" : "FALSE");
    }

    decrementReferenceCount(object);
}


/*
 * operationDup() implementation
 *
 * ( a -- a a )
 *
 * If the stack is non-empty, pops the top element and pushes it twice,
 * resulting in duplication. Safe no-op if the stack is empty.
 */
void operationDup(tfcontext *context) {
    if (context->stack->list_obj.len == 0) return;
    
    tfobj *object = stackPop(context);
    
    stackPush(context, object);
    stackPush(context, object);
    
    decrementReferenceCount(object);
}

/*
 * operationDrop() implementation
 *
 * ( a -- )
 *
 * Pops and immediately destroys the top element of the stack.
 */
void operationDrop(tfcontext *context) {
    tfobj *object = stackPop(context);
    
    decrementReferenceCount(object);
}

/*
 * operationSwap() implementation
 *
 * ( a b -- b a )
 *
 * Pops two elements and pushes them back in reverse order,
 * effectively swapping the top two stack elements.
 */
void operationSwap(tfcontext *context) {
    tfobj *b = stackPop(context);                                       
    tfobj *a = stackPop(context);                                       

    stackPush(context, b);                                              
    stackPush(context, a);

    decrementReferenceCount(a);                                  
    decrementReferenceCount(b);
}