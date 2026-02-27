/*
 * List/Array Implementation
 *
 * Implements dynamic array operations for TF_OBJ_LIST, with automatic
 * capacity doubling when needed. Lists are the primary container type
 * in ToyForth and are used for stacks, compiled programs, and user data.
 */

#include "list.h"
#include "mem.h"
#include "tforth.h"


/*
 * listAppendObject() implementation
 *
 * Appends an object to the end of a list, with automatic capacity
 * management. When the list is full, capacity is doubled. The list
 * acquires a new reference to the object (increments refcount).
 */
void listAppendObject(tfobj *list, tfobj *object) {
    if (list->list_obj.len >= list->list_obj.capacity) {
        list->list_obj.capacity *= 2;  
        list->list_obj.element = wrealloc(list->list_obj.element, sizeof(tfobj *) * list->list_obj.capacity);
    }

    list->list_obj.element[list->list_obj.len] = object;                            
    list->list_obj.len++;
    
    /* The list acquires a reference to the object */
    incrementReferenceCount(object);                                                
}