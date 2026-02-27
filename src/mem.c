/*
 * Memory Management Implementation
 *
 * Implements reference-counted memory management for all ToyForth objects.
 * Uses a simple tracing garbage collector via reference counting: when
 * refcount reaches 0, the object is immediately freed.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mem.h"
#include "tforth.h"


/*
 * wmalloc() implementation
 *
 * Wrapper around malloc() that enforces non-NULL returns through explicit
 * error handling on allocation failure.
 */
void *wmalloc(size_t size) {
    if (size == 0) size = 1;                                       
    
    void *newptr = malloc(size);
    if (newptr == NULL) {
        fprintf(stderr, "OOM. Couldn't allocate %zu bytes.\n", size);
        exit(EXIT_FAILURE);
    }

    return newptr;
}


/*
 * wrealloc() implementation
 *
 * Wrapper around realloc() that enforces non-NULL returns through explicit
 * error handling on reallocation failure.
 */
void *wrealloc(void *ptr, size_t size) {
    if (size == 0) size = 1;             
    
    void *newptr = realloc(ptr, size);
    if (newptr == NULL) {
        fprintf(stderr, "OOM. Couldn't reallocate %zu bytes.\n", size);
        exit(EXIT_FAILURE);
    }
    
    return newptr;
}


/*
 * freeObject() implementation
 *
 * Deallocates a tfobj and all its internal heap-allocated data.
 * For container types (lists), this triggers recursive deallocation
 * of contained objects via reference counting.
 */
void freeObject(tfobj *object) {
    if (object == NULL) return;

    if (object->type == TF_OBJ_SYMBOL || object->type == TF_OBJ_STR) {
        free(object->str_obj.str);                                       /* Frees the deep-copied string buffer */
    }

    if (object->type == TF_OBJ_LIST) {
        for (size_t i = 0; i < object->list_obj.len; i++) {
            tfobj *element = object->list_obj.element[i];
            decrementReferenceCount(element);                       /* Recursively decrease refcount for each item in the list */
        }
        free(object->list_obj.element);                                  /* Frees the array holding the pointers */
    }

    free(object);                                                        /* Finally, free the object struct itself */
}


/*
 * incrementReferenceCount() implementation
 *
 * Increases the reference count when a new reference is acquired.
 * NULL-safe (no-op if passed NULL).
 */
void incrementReferenceCount(tfobj *object) {
    if (object == NULL) return;
    object->refcount++;
}


/*
 * decrementReferenceCount() implementation
 *
 * Decreases the reference count when a reference is released.
 * When count reaches 0, immediately deallocates the object.
 * NULL-safe (no-op if passed NULL).
 */
void decrementReferenceCount(tfobj *object) {
    if (object == NULL) return;

    object->refcount--;
    
    /* Automatic deallocation when no references remain */
    if (object->refcount == 0) {
        freeObject(object);
    }
}


/*
 * createObject() implementation
 *
 * Base constructor for all tfobj instances. Allocates the structure,
 * initializes type, and sets refcount=1 (creator owns one reference).
 */
tfobj *createObject(TF_OBJ_TYPE type) {
    tfobj *object = wmalloc(sizeof(tfobj));
    object->type = type;
    object->refcount = 1;  /* New objects start with one reference from the creator */
    
    return object;
}


/*
 * createStringObject() implementation
 *
 * Creates a string object with a deep copy of the input data.
 * Input does not need to be NUL-terminated.
 */
tfobj *createStringObject(const char *str, size_t len) {
    tfobj *object = createObject(TF_OBJ_STR);
    
    object->str_obj.str = wmalloc(sizeof(char) * len + 1);
    object->str_obj.len = len;
    
    memcpy(object->str_obj.str, str, len);
    object->str_obj.str[len] = '\0';  
    
    return object;
}


/*
 * createIntegerObject() implementation
 *
 * Creates an integer object containing a 32-bit signed value.
 */
tfobj *createIntegerObject(int number) {
    tfobj *object = createObject(TF_OBJ_INT);
    object->number = number;
    
    return object;
}


/*
 * createBooleanObject() implementation
 *
 * Creates a boolean object (internally stored as int: 0=false, non-zero=true).
 */
tfobj *createBooleanObject(int number) {
    tfobj *object = createObject(TF_OBJ_BOOL);
    object->number = number;
    
    return object;
}


/*
 * createSymbolObject() implementation
 *
 * Creates a symbol object (Forth word name) with deep-copied string data.
 * Symbols are implemented as strings but have distinct type for semantics.
 */
tfobj *createSymbolObject(const char *str, size_t len) {
    tfobj *object = createStringObject(str, len);
    object->type = TF_OBJ_SYMBOL;
    
    return object;
}


/*
 * createListObject() implementation
 *
 * Creates an empty list with initial capacity for dynamic growth.
 * Used for the data stack and compiled program representation.
 */
tfobj *createListObject() {
    tfobj *object = createObject(TF_OBJ_LIST);
    object->list_obj.capacity = INITIAL_STACK_CAPACITY;
    object->list_obj.element = wmalloc(object->list_obj.capacity * sizeof(tfobj*));
    object->list_obj.len = 0;

    return object;
}


/*
 * createContext() implementation
 *
 * Allocates and initializes a ToyForth execution context with
 * an empty data stack.
 */
tfcontext *createContext() {
    tfcontext *context = wmalloc(sizeof(tfcontext));
    context->stack = createListObject();
    
    return context;
}


/*
 * freeContext() implementation
 *
 * Deallocates a context and all objects remaining on its stack.
 * The stack deallocation is handled via reference counting.
 */
void freeContext(tfcontext *context) {
    if (context == NULL) return;
    
    decrementReferenceCount(context->stack);

    free(context);
}