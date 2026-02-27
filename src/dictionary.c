/*
 * Operation Dictionary Implementation
 *
 * Implements a static lookup table of all built-in Forth primitives.
 * The dictionary is defined at compile-time and provides O(n) lookup
 * where n is the number of primitive operations.
 *
 * NOTE: All operation names are case-sensitive. Comparison uses strcmp().
 */

#include <string.h>

#include "dictionary.h"
#include "ops.h"



/* 
 * OperationEntry - Maps a Forth operation to its C implementation
 *
 * This structure pairs a string operation with a function pointer to the
 * corresponding operation implementation.
 */
typedef struct {
    const char *operation;  /* Forth operation name (NUL-terminated) */
    Operation op;      /* Function pointer to the C implementation */
} OperationEntry;



/*
 * operations[] - Static dictionary of all built-in Forth primitives
 *
 * This table contains all primitive operations available in ToyForth.
 * The table is searched linearly by lookupOperation().
 *
 * NOTE: Operation names are lowercase and case-sensitive.
 *       "dup" is found; "DUP" or "Dup" will fail to resolve.
 */
static const OperationEntry operations[] = {
    {"+", operationAdd},
    {"-", operationSub},
    {"*", operationMul},
    {"/", operationDiv},
    {".", operationPrint},
    {"dup", operationDup},
    {"drop", operationDrop},
    {"swap", operationSwap}
};


/* Calculate the number of operations in the dictionary */
#define OP_COUNT (sizeof(operations) / sizeof(operations[0]))


/*
 * lookupOperation() implementation
 *
 * Searches the operation dictionary for an operation name and returns the
 * corresponding function pointer. Uses linear search with strcmp().
 */
Operation lookupOperation(const char *operation) {
    for (size_t i = 0; i < OP_COUNT; i++) {
        if (strcmp(operation, operations[i].operation) == 0) {
            return operations[i].op;
        }
    }

    return NULL;
}