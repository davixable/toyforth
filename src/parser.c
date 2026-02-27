/*
 * Parser and Compiler Implementation
 *
 * Implements an iterative tokenizer/scanner that 
 * converts ToyForth source text into a linear list of 
 * executable objects (integers, symbols).
 * Provides line/column tracking for error diagnostics.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "parser.h"
#include "dictionary.h"
#include "tforth.h"
#include "mem.h"
#include "list.h"


/*
 * parserPeek() implementation
 *
 * Returns the current character without advancing the parser state.
 */
char parserPeek(tfparser *parser) {
    return *parser->program;
}


/*
 * parserAdvance() implementation
 *
 * Moves the parser forward by one character, maintaining accurate line/column
 * tracking for error diagnostics. Handles newline characters specially to
 * update line counters.
 */
void parserAdvance(tfparser *parser) {
    if (parserPeek(parser) == '\0') return;

    if (parserPeek(parser) == '\n') {
        parser->line++;
        parser->column = 1;
    } else {
        parser->column++;
    }
    parser->program++;
}


/*
 * parserSkipWhiteSpace() implementation
 *
 * Advances past whitespace characters (space, tab, newline, etc.)
 * until a non-whitespace character or EOF is encountered.
 */
void parserSkipWhiteSpace(tfparser *parser) {
    while (parserPeek(parser) != '\0' && isspace(parserPeek(parser))) {
        parserAdvance(parser);
    }
}


/*
 * parseNumber() implementation
 *
 * Parses a base-10 integer literal (with optional leading '-' sign).
 * Uses strtol() for robust number parsing. Advances the parser past
 * all consumed characters.
 */
tfobj *parseNumber(tfparser *parser) {
    char *start = parser->program;
    char *end;  

    long value = strtol(start, &end, 10);
    
    while (parser->program < end) {
        parserAdvance(parser);
    }
    
    return createIntegerObject(value);
}


/*
 * parseSymbol() implementation
 *
 * Parses a Forth word/symbol name (sequence of non-whitespace characters).
 * Looks up the parsed name in the operation dictionary and returns a symbol
 * object only if it's a known primitive. Returns NULL for unknown symbols.
 */
tfobj *parseSymbol(tfparser *parser) {
    char *start = parser->program;

    while (!isspace(parserPeek(parser)) && parserPeek(parser) != '\0') {
        parserAdvance(parser);
    }

    size_t len_symbol = parser->program - start;
    tfobj *new_object = createSymbolObject(start, len_symbol);

    if (lookupOperation(new_object->str_obj.str) == NULL) {
        decrementReferenceCount(new_object);
        return NULL;
    }

    return new_object;
}


/*
 * compile() implementation
 *
 * Main entry point for the compilation phase. Tokenizes the input program
 * text and produces a list of compiled objects (integers, symbols). Performs
 * basic validation by ensuring all symbols are known primitives.
 *
 * Returns NULL on syntax error (prints diagnostic message to stderr).
 */
tfobj *compile(char *program_txt) {
    tfobj *program_list = createListObject();
    tfparser parser;
    parser.program = program_txt;
    parser.line = 1;
    parser.column = 1;

    while (parserPeek(&parser)) {
        tfobj *new_object = NULL; 
        parserSkipWhiteSpace(&parser);
        int line = parser.line;
        int column = parser.column;
        char c = parserPeek(&parser);

        if (c == '\0') break;

        if (isdigit(c) || (c == '-' && isdigit(parser.program[1]))) {
            new_object = parseNumber(&parser);
        } else {
            new_object = parseSymbol(&parser);
        }

        if (new_object == NULL) {
            fprintf(stderr, "Syntax error. Check line %d column %d.\n", line, column);
            decrementReferenceCount(program_list); 
            return NULL;
        } else {
            listAppendObject(program_list, new_object);
            decrementReferenceCount(new_object);
        }
    }

    return program_list;
}