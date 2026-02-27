/*
 * ToyForth - A minimal Forth interpreter implementation in C
 *
 * This header defines the core data structures and types for the ToyForth VM.
 * The implementation uses reference counting for automatic memory management
 * of dynamically allocated objects.
 */

#ifndef TFORTH_H
#define TFORTH_H

#include <stddef.h>

/* Initial capacity for newly allocated stacks and lists */
#define INITIAL_STACK_CAPACITY 16


/*
 * TF_OBJ_TYPE - Enumeration of all object types in the ToyForth system
 *
 * TF_OBJ_INT:    Integer value (32-bit signed integer)
 * TF_OBJ_STR:    String value (NUL-terminated)
 * TF_OBJ_BOOL:   Boolean value (true/false)
 * TF_OBJ_LIST:   List/array container containing pointers to other tfobj instances
 * TF_OBJ_SYMBOL: Forth word/operation name (stored as string, resolved at execution)
 */
typedef enum {
    TF_OBJ_INT,
    TF_OBJ_STR,
    TF_OBJ_BOOL,
    TF_OBJ_LIST,
    TF_OBJ_SYMBOL
} TF_OBJ_TYPE;

/*
 * tfobj - The primary value type for the ToyForth system
 *
 * This structure represents any value in the language: integers, strings,
 * symbols, booleans, and lists. Uses a tagged union pattern for type safety.
 *
 * Memory Management:
 *   - All tfobj instances use automatic reference counting
 *   - type:    Determines which union member is active
 *   - refcount: Number of active references; object is freed when refcount == 0
 *   - union:   Tagged union containing the actual value data
 */
typedef struct tfobj {
    TF_OBJ_TYPE type;               /* Distinguishes which union member is active */
    int refcount;                   /* Reference count for automatic memory deallocation */
    union {
        int number;                 /* For TF_OBJ_INT and TF_OBJ_BOOL */
        struct {
            char *str;              /* NUL-terminated string data (heap-allocated) */
            size_t len;             /* Length of string (excluding NUL terminator) */
        } str_obj;                  /* For TF_OBJ_STR and TF_OBJ_SYMBOL */
        struct {
            struct tfobj **element; /* Array of pointers to other tfobj instances */
            size_t len;             /* Current number of elements in the list */
            size_t capacity;        /* Allocated space for elements (>= len) */
        } list_obj;                 /* For TF_OBJ_LIST */
    };
} tfobj;

/*
 * tfparser - Parser state for tokenizing and compiling program text
 *
 * Maintains position information (line, column) for accurate error reporting
 * during the compilation phase.
 */
typedef struct {
    char *program;                  /* Pointer to current position in program text */
    int line;                       /* 1-based line number for error diagnostics */
    int column;                     /* 1-based column number for error diagnostics */
} tfparser;

/*
 * tfcontext - Execution context for the ToyForth virtual machine
 *
 * Encapsulates the runtime state of a ToyForth program. Currently contains
 * only the data stack, but extensible for future features (return stack,
 * locals, etc.).
 */
typedef struct {
    tfobj *stack;                   /* The primary data stack (implemented as TF_OBJ_LIST) */
} tfcontext;

#endif  