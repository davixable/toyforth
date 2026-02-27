/*
 * Operation Dictionary Module
 *
 * Implements a static lookup table mapping Forth word names (strings) to
 * their corresponding C implementations. The dictionary is built at compile
 * time and provides O(n) lookup where n is the number of primitives.
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "tforth.h"


/*
 * Operation - Function pointer type for built-in Forth operations
 *
 * All Forth primitives have the same signature: they take a context pointer
 * and modify the VM state (typically by popping operands, computing, and
 * pushing results onto the stack).
 */
typedef void (*Operation)(tfcontext *context);

/*
 * lookupOperation() - Resolves a Forth operation to its implementation
 *
 * Searches the built-in primitive dictionary for the given operation.
 * Comparison is case-sensitive ("dup" ≠ "DUP").
 *
 * Args:
 *   operation - Operation to look up (NUL-terminated string)
 *
 * Returns:
 *   Function pointer to the operation if found, or NULL if unknown
 */
Operation lookupOperation(const char *operation);


#endif 