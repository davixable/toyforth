/*
 * Parser Module - Tokenization and Compilation
 *
 * Implements an iterative tokenizer/scanner that converts ToyForth source
 * text into a compiled linear list of executable objects (integers, symbols, etc.).
 * Maintains position tracking for human-friendly error reporting.
 */

#ifndef PARSER_H
#define PARSER_H

#include "tforth.h"

/*
 * parserPeek() - Returns the current character without advancing
 *
 * Returns the character at the parser's current position. Returns '\0' if
 * the parser has reached the end of the program text.
 *
 * Args:
 *   parser - Parser state to examine
 *
 * Returns:
 *   Current character or '\0' at end of input
 */
char parserPeek(tfparser *parser);

/*
 * parserAdvance() - Moves the parser forward by one character
 *
 * Updates the parser position and maintains line/column tracking for error
 * reporting. Handles newlines to correctly track line breaks. Safe no-op
 * when at end of input.
 *
 * Args:
 *   parser - Parser state to advance
 */
void parserAdvance(tfparser *parser);

/*
 * parserSkipWhiteSpace() - Skips all whitespace characters
 *
 * Advances the parser past spaces, tabs, newlines, and other isspace()
 * characters. Correctly updates line/column counters for newlines.
 *
 * Args:
 *   parser - Parser state to advance
 */
void parserSkipWhiteSpace(tfparser *parser);


/*
 * parseNumber() - Parses a base-10 integer literal
 *
 * Recognizes optional leading '-' sign followed by decimal digits. Uses
 * strtol() for parsing, handling overflow/underflow as per strtol semantics.
 * Advances the parser past all consumed characters.
 *
 * Args:
 *   parser - Parser positioned at first digit or '-' sign
 *
 * Returns:
 *   New TF_OBJ_INT with value from parsed decimal number (refcount=1)
 */
tfobj *parseNumber(tfparser *parser);

/*
 * parseSymbol() - Parses a Forth word/symbol name
 *
 * Reads characters until whitespace or end-of-input. The resulting string
 * is looked up in the operation dictionary; returns NULL if not found.
 * Advances the parser past the parsed symbol.
 *
 * Args:
 *   parser - Parser positioned at first character of symbol
 *
 * Returns:
 *   New TF_OBJ_SYMBOL if found in dictionary (refcount=1), or
 *   NULL if the symbol is not a known operation
 */
tfobj *parseSymbol(tfparser *parser);


/*
 * compile() - Compiles ToyForth source code into executable objects
 *
 * Tokenizes the input program text and produces a list of compiled objects
 * (integers, symbols, etc.). Performs basic syntax validation by checking
 * that each symbol is a known Forth operation.
 *
 * On successful compilation, returns a TF_OBJ_LIST containing the compiled
 * program with refcount=1. On syntax error, prints error location and
 * returns NULL.
 *
 * Args:
 *   program_text - NUL-terminated source code string
 *
 * Returns:
 *   New TF_OBJ_LIST (refcount=1) containing compiled program objects, or
 *   NULL if a syntax error is detected
 */
tfobj *compile(char *program_text);

#endif