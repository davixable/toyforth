/*
 * List Module - Dynamic Array Operations
 *
 * Provides utilities for manipulating TF_OBJ_LIST objects with automatic
 * resizing and reference counting. The list is the primary container type
 * used for the data stack and compiled program representation.
 */

#ifndef LIST_H
#define LIST_H

#include "tforth.h"

/*
 * listAppendObject() - Appends an object to a list with automatic resizing
 *
 * Adds a new object to the end of a list, doubling capacity when needed.
 * Increments the object's refcount since the list now holds a reference.
 *
 * Args:
 *   list   - Target list object (must be TF_OBJ_LIST)
 *   object - Object to append (any tfobj type)
 */
void listAppendObject(tfobj *list, tfobj *object);


#endif 