/*
 * Data Stack Implementation
 *
 * Provides push/pop operations for the ToyForth data stack, which is
 * implemented as a dynamically resizing list. Handles reference counting
 * for proper memory management.
 *
 * OWNERSHIP SEMANTICS:
 *   - stackPush(): Stack acquires a new reference (increments refcount)
 *   - stackPop():  Caller acquires the reference (stack does NOT decrement)
 */

#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "list.h"
#include "mem.h"


/*
 * stackPush() implementation
 *
 * Adds an object to the top of the stack. The stack acquires a new
 * reference (increments refcount), so listAppendObject() is used directly.
 */
void stackPush(tfcontext *context, tfobj *object) {
    /* Reuse list utility for stack operations; it handles resizing and refcounting */
    listAppendObject(context->stack, object);
}


/*
 * stackPop() implementation
 *
 * Removes and returns the top of the stack. The caller acquires ownership
 * of the returned reference and must decrement it when done. The stack does
 * NOT decrement the refcount here.
 *
 * Terminates with error message if the stack is empty (underflow).
 */
tfobj *stackPop(tfcontext *context) {
    if (context->stack->list_obj.len == 0) {
        fprintf(stderr, "Stack underflow error.\n");
        exit(EXIT_FAILURE);
    }

    size_t top_index = context->stack->list_obj.len - 1;
    tfobj *object = context->stack->list_obj.element[top_index];
    context->stack->list_obj.len--;

    /* 
     * IMPORTANT: We do NOT decrement the reference count here.
     * The list no longer owns this reference, but the caller now does.
     * The caller is responsible for decrementing when done.
     */
    return object;
}