/*
 * Memory Management Module
 *
 * Provides automatic memory management through reference counting. All tfobj
 * instances use reference counting: when refcount reaches 0, the object is
 * automatically deallocated.
 *
 * OWNERSHIP RULES:
 *   - Creating a function returns an object with refcount=1 (creator owns it)
 *   - Passing an object to a function does NOT transfer ownership
 *   - The caller remains responsible for decrementing when done
 *   - Exceptions: operations that modify reference counts (stackPush, etc.)
 */

#ifndef MEM_H
#define MEM_H

#include <stddef.h>
#include "tforth.h"


/*
 * wmalloc() - Safe malloc wrapper with automatic OOM error handling
 *
 * Allocates memory with guaranteed non-NULL return or program exit.
 * Treats zero-size requests as one byte to simplify logic.
 *
 * Args:
 *   size - Number of bytes to allocate (0 is promoted to 1)
 *
 * Returns:
 *   Non-NULL pointer to allocated memory (never NULL)
 */
void *wmalloc(size_t size);

/*
 * wrealloc() - Safe realloc wrapper with automatic OOM error handling
 *
 * Resizes previously allocated memory with guaranteed non-NULL return
 * or program exit. Treats zero-size requests as one byte.
 *
 * Args:
 *   ptr  - Previous allocation (may be NULL)
 *   size - New size in bytes (0 is promoted to 1)
 *
 * Returns:
 *   Non-NULL pointer to resized memory (never NULL)
 */
void *wrealloc(void *ptr, size_t size);


/*
 * createObject() - Base constructor for all ToyForth objects
 *
 * Allocates a new tfobj instance, initializes type and sets refcount to 1.
 * The caller takes ownership of this reference.
 *
 * Args:
 *   type - The object type (determines active union member)
 *
 * Returns:
 *   Newly allocated tfobj with refcount=1
 */
tfobj *createObject(TF_OBJ_TYPE type);

/*
 * createStringObject() - Constructs a string object
 *
 * Creates a TF_OBJ_STR instance with a deep copy of the input string.
 * The input string may be freed immediately after this call.
 *
 * Args:
 *   str - Source string (does not need to be NUL-terminated)
 *   len - Length of source string in bytes
 *
 * Returns:
 *   New TF_OBJ_STR object with refcount=1 and deep-copied string data
 */
tfobj *createStringObject(const char *str, size_t len);

/*
 * createIntegerObject() - Constructs an integer object
 *
 * Creates a TF_OBJ_INT instance containing a 32-bit signed integer.
 *
 * Args:
 *   number - The integer value to store
 *
 * Returns:
 *   New TF_OBJ_INT object with refcount=1
 */
tfobj *createIntegerObject(int number);

/*
 * createBooleanObject() - Constructs a boolean object
 *
 * Creates a TF_OBJ_BOOL instance (internally stored as int: 0=false, 1=true).
 *
 * Args:
 *   number - The boolean value (0 or non-zero)
 *
 * Returns:
 *   New TF_OBJ_BOOL object with refcount=1
 */
tfobj *createBooleanObject(int number);

/*
 * createSymbolObject() - Constructs a symbol/word name object
 *
 * Creates a TF_OBJ_SYMBOL instance with a deep copy of the input string.
 * Symbols are Forth word names that are resolved at execution time.
 *
 * Args:
 *   str - Source symbol string (does not need to be NUL-terminated)
 *   len - Length of source string in bytes
 *
 * Returns:
 *   New TF_OBJ_SYMBOL object with refcount=1 and deep-copied string data
 */
tfobj *createSymbolObject(const char *str, size_t len);

/*
 * createListObject() - Constructs an empty list/array object
 *
 * Allocates a new TF_OBJ_LIST with initial capacity. Used for the main
 * data stack and compiled program representation.
 *
 * Returns:
 *   New empty TF_OBJ_LIST object with refcount=1 and capacity=INITIAL_STACK_CAPACITY
 */
tfobj *createListObject();


/*
 * createContext() - Allocates and initializes a ToyForth execution context
 *
 * Creates a new tfcontext with an empty data stack ready for execution.
 * Should be freed with freeContext() when no longer needed.
 *
 * Returns:
 *   New tfcontext with initialized stack and refcount=1
 */
tfcontext *createContext();

/*
 * freeContext() - Deallocates an execution context and its resources
 *
 * Releases all resources associated with the context, including any objects
 * remaining on the stack. The stack is freed through reference counting.
 *
 * Args:
 *   context - Context to deallocate (may be NULL; no-op if so)
 */
void freeContext(tfcontext *context);


/*
 * incrementReferenceCount() - Increases an object's reference count
 *
 * Called when a new reference to an object is created (e.g., adding to a
 * container). A safe no-op if passed a NULL pointer.
 *
 * Args:
 *   object - Object whose reference count should be incremented (may be NULL)
 */
void incrementReferenceCount(tfobj *object);

/*
 * decrementReferenceCount() - Decreases an object's reference count
 *
 * Called when relinquishing ownership of a reference. When count reaches 0,
 * the object is automatically freed via freeObject(). Safe no-op if passed
 * a NULL pointer.
 *
 * Args:
 *   object - Object whose reference count should be decremented (may be NULL)
 */
void decrementReferenceCount(tfobj *object);

/*
 * freeObject() - Immediately deallocates an object and its resources
 *
 * Called automatically when reference count reaches 0. Does not check
 * reference counts; use only after confirming the object has no active
 * references. Handles recursive deallocation for container types.
 *
 * Args:
 *   object - Object to deallocate and destroy (may be NULL)
 */
void freeObject(tfobj *object);


#endif