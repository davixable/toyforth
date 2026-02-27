/*
 * Stack Module - Data Stack Operations
 *
 * Provides push/pop operations for the ToyForth data stack (implemented as
 * a dynamically resizing list). Handles reference counting automatically.
 */

#ifndef STACK_H
#define STACK_H

#include "tforth.h"

/*
 * stackPush() - Pushes an object onto the data stack
 *
 * Appends the object to the context's stack list. The stack takes a new
 * reference to the object and increments its refcount. The caller retains
 * their original reference and must decrement it when done (or transfer
 * ownership if appropriate).
 *
 * Args:
 *   context - Execution context
 *   object  - Object to push onto the stack
 */
void stackPush(tfcontext *context, tfobj *object);

/*
 * stackPop() - Pops the topmost object from the data stack
 *
 * Removes the top element and returns a reference to it. The stack releases
 * its reference to the object (does NOT decrement refcount). The caller
 * acquires ownership and must decrement the refcount when done.
 *
 * Terminates with "Stack underflow error" if the stack is empty.
 *
 * Args:
 *   context - Execution context
 *
 * Returns:
 *   The popped object (caller takes ownership of the reference)
 */
tfobj *stackPop(tfcontext *context);

#endif 