/*
 * ToyForth Interpreter - Main Entry Point
 *
 * Implements the main execution flow:
 *   1. Load source code from file
 *   2. Compile to intermediate representation
 *   3. Execute on the virtual machine
 *   4. Clean up resources
 */

#include <stdio.h>
#include <stdlib.h>

#include "mem.h"
#include "engine.h"
#include "tforth.h"
#include "file_utils.h"
#include "parser.h"

/*
 * main() - Program entry point
 *
 * Orchestrates the three phases of the ToyForth interpreter:
 *   1. File Loading: Reads the source program from the specified file
 *   2. Compilation:  Parses and tokenizes into executable objects
 *   3. Execution:    Runs the compiled program on the VM
 *
 * Usage:
 *   toyfort <source-file>
 *
 * Returns:
 *   EXIT_SUCCESS (0) if successful
 *   EXIT_FAILURE (1) if arguments invalid or execution error occurs
 */
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Error. How to use: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *program_txt = readFromFile(argv[1]);
    tfobj *program = compile(program_txt);
    tfcontext *context = createContext();
    execute(program, context);

    /* Clean up allocated resources */
    decrementReferenceCount(program);
    freeContext(context);
    free(program_txt);
    
    return EXIT_SUCCESS;
}