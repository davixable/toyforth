/*
 * Forth Primitive Operations Module
 *
 * Implements built-in Forth words: arithmetic operations, I/O, and stack
 * manipulation. Each operation modifies the execution context (typically by
 * popping operands and pushing results).
 */

#ifndef OPS_H
#define OPS_H

#include "tforth.h"


/*
 * STACK DIAGRAM NOTATION
 *
 * Functions use standard Forth stack notation to document behavior:
 *
 *     ( before -- after )
 *
 * - 'before': Items expected on stack before execution (rightmost = top)
 * - '--':     Execution separator
 * - 'after':  Items left on stack after execution
 *
 * Example: "( a b -- sum )" means pop b, pop a, push a+b
 */


/*
 * operationAdd() - Adds two integers
 *
 * ( a b -- a+b )
 *
 * Pops two integers, computes their sum, and pushes the result.
 * No operation is performed if operands are not both integers.
 */
void operationAdd(tfcontext *context);

/*
 * operationSub() - Subtracts two integers
 *
 * ( a b -- a-b )
 *
 * Pops two integers, computes a minus b, and pushes the result.
 * No operation is performed if operands are not both integers.
 */
void operationSub(tfcontext *context);

/*
 * operationMul() - Multiplies two integers
 *
 * ( a b -- a*b )
 *
 * Pops two integers, computes their product, and pushes the result.
 * No operation is performed if operands are not both integers.
 */
void operationMul(tfcontext *context);

/*
 * operationDiv() - Divides two integers with truncation
 *
 * ( a b -- a/b )
 *
 * Pops two integers, computes integer division (truncated toward zero),
 * and pushes the result. Division by zero terminates the program with error.
 * No operation is performed if operands are not both integers.
 */
void operationDiv(tfcontext *context);


/*
 * operationPrint() - Prints and consumes the top of stack
 *
 * ( a -- )
 *
 * Pops one value and prints it to stdout in a human-readable format:
 *  - Integer: decimal representation followed by space
 *  - String: string contents followed by space
 *  - Boolean: "TRUE" or "FALSE" followed by space
 *
 * The popped value is destroyed after printing.
 */
void operationPrint(tfcontext *context);


/*
 * operationDup() - Duplicates the top of stack
 *
 * ( a -- a a )
 *
 * If stack is non-empty, pops the top element and pushes it twice,
 * leaving it duplicated on the stack. Safe no-op if stack is empty.
 */
void operationDup(tfcontext *context);

/*
 * operationDrop() - Discards the top of stack
 *
 * ( a -- )
 *
 * Pops and immediately destroys the top element.
 */
void operationDrop(tfcontext *context);

/*
 * operationSwap() - Exchanges the top two stack elements
 *
 * ( a b -- b a )
 *
 * Pops two elements and pushes them back in reverse order.
 */
void operationSwap(tfcontext *context);


#endif