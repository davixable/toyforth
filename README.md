# ToyForth - A Minimal Forth Interpreter in C

## Introduction

ToyForth is an educational implementation of a minimal Forth interpreter written from scratch in C. The project demonstrates fundamental concepts in language implementation including lexical analysis, compilation, stack-based virtual machines, and automatic memory management through reference counting.

This project was inspired by the excellent educational work of Salvatore Sanfilippo (@antirez). Many thanks


## Table of Contents

- [How It Works](#how-it-works)
- [Building and Running](#building-and-running)
- [Testing](#testing)
- [Supported Operators](#supported-operators)
- [Architecture and Design](#architecture-and-design)
  - [Data Structures](#data-structures)
  - [Reference Counting](#reference-counting)
  - [Parser and Compilation](#parser-and-compilation)
  - [Virtual Machine and Execution Engine](#virtual-machine-and-execution-engine)
  - [Operation Dictionary](#operation-dictionary)

## How It Works

ToyForth implements a classic three-phase interpreter architecture:

1. **Lexical Analysis & Compilation**: Source code is tokenized and compiled into a list of executable objects (integers and operation symbols)
2. **Execution**: The compiled program is executed on a stack-based virtual machine
3. **Stack Operations**: All computations use a LIFO (Last In, First Out) data stack

The execution model is simple: integers are pushed onto the stack, and operations pop their operands, compute results, and push results back.

### Example

```forth
10 5 - 2 * 3 + dup . 13 - .
```

Execution trace:
```
Push 10          → [10]
Push 5           → [10, 5]
Execute -        → [5]
Push 2           → [5, 2]
Execute *        → [10]
Push 3           → [10, 3]
Execute +        → [13]
Execute dup      → [13, 13]
Execute .        → [13] (prints "13 ")
Push 13          → [13, 13]
Execute -        → [0]
Execute .        → [] (prints "0 ")
```

## Building and Running

### Prerequisites

- GCC compiler (or any C99-compatible compiler)
- Make
- Bash shell (for test suite)

### Build

```bash
make
```

This compiles all source files and produces the `toyforth` executable.

### Run

```bash
./toyforth <source-file>
```

Example:

```bash
./toyforth tests/add.tf
```

### Clean

```bash
make clean
```

Removes compiled objects and executables.

## Testing

ToyForth includes an automated test suite with comprehensive coverage of all operations and edge cases.

### Run Tests

```bash
make test
```

This runs the [`run_tests.sh`](run_tests.sh) script which:
- Executes each `.tf` test file
- Compares output against `.expected` files
- Reports pass/fail status with color-coded output

### Test Files

Test cases are in the [`tests`](tests) directory:
- [`tests/add.tf`](tests/add.tf) / [`tests/add.expected`](tests/add.expected) - Addition
- [`tests/sub.tf`](tests/sub.tf) / [`tests/sub.expected`](tests/sub.expected) - Subtraction
- [`tests/mul.tf`](tests/mul.tf) / [`tests/mul.expected`](tests/mul.expected) - Multiplication
- [`tests/div.tf`](tests/div.tf) / [`tests/div.expected`](tests/div.expected) - Division
- [`tests/dup.tf`](tests/dup.tf) / [`tests/dup.expected`](tests/dup.expected) - Duplication
- [`tests/drop.tf`](tests/drop.tf) / [`tests/drop.expected`](tests/drop.expected) - Drop from stack
- [`tests/swap.tf`](tests/swap.tf) / [`tests/swap.expected`](tests/swap.expected) - Swap top two elements
- [`tests/stack.tf`](tests/stack.tf) / [`tests/stack.expected`](tests/stack.expected) - Combined operations
- [`tests/complex.tf`](tests/complex.tf) / [`tests/complex.expected`](tests/complex.expected) - Complex expression

## Supported Operators

| Operator | Stack Diagram | Description |
|----------|---------------|-------------|
| `+` | `( a b -- a+b )` | Adds two integers |
| `-` | `( a b -- a-b )` | Subtracts b from a |
| `*` | `( a b -- a*b )` | Multiplies two integers |
| `/` | `( a b -- a/b )` | Divides a by b (integer division) |
| `.` | `( a -- )` | Pops and prints top of stack |
| `dup` | `( a -- a a )` | Duplicates top of stack |
| `drop` | `( a -- )` | Removes top of stack |
| `swap` | `( a b -- b a )` | Swaps top two elements |

**Stack Notation**: In Forth convention, the rightmost item is the top of the stack. `( a b -- result )` means pop b, pop a, push result.

## Architecture and Design

ToyForth demonstrates several important interpreter design patterns. This section provides a deep dive into the implementation.

### Core Modules

| Module | Purpose | Implementation |
|--------|---------|-----------------|
| [`tforth.h`](src/tforth.h) | Central type definitions | Defines `tfobj` tagged union and enumeration of all object types |
| [`mem.h`](src/mem.h) | Memory management | Reference counting with `incrementReferenceCount()` and `decrementReferenceCount()` for automatic deallocation |
| [`stack.h`](src/stack.h) | Stack operations | LIFO data structure with `stackPush()` and `stackPop()` maintaining reference counts |
| [`list.h`](src/list.h) | Dynamic arrays | Growable list of `tfobj*` with `listAppendObject()` and automatic capacity management |
| [`parser.h`](src/parser.h) | Lexical analysis & compilation | Tokenizes source text and produces compiled program list via `compile()` |
| [`dictionary.h`](src/dictionary.h) | Operation lookup | Static hash table mapping operation names to function pointers via `lookupOperation()` |
| [`ops.h`](src/ops.h) | Arithmetic & stack operations | Implements `operationAdd()`, `operationSub()`, `operationMul()`, `operationDiv()`, `operationDup()`, `operationDrop()`, `operationSwap()`, `operationPrint()` |
| [`engine.h`](src/engine.h) | Execution engine | Fetch-execute loop in `execute()` that interprets compiled programs on the stack VM |
| [`file_utils.h`](src/file_utils.h) | File I/O | Reads source files into memory via `readFile()` for compilation |

### Data Structures

The core of ToyForth is the **tagged union** data structure, defined in [`src/tforth.h`](src/tforth.h):

```c
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
```

Every value in ToyForth—integers, strings, symbols, booleans, and lists—is represented as a [`tfobj`](src/engine.h). The [`type`](src/mem.h) field identifies which union member is active, enabling type-safe operations.

**Type Enumeration**:
- [`TF_OBJ_INT`](src/tforth.h): 32-bit signed integer
- [`TF_OBJ_STR`](src/tforth.h): NUL-terminated string
- [`TF_OBJ_BOOL`](src/tforth.h): Boolean value (stored as int)
- [`TF_OBJ_LIST`](src/tforth.h): Dynamic array of tfobj pointers
- [`TF_OBJ_SYMBOL`](src/tforth.h): Forth word name (resolved at execution)

### Reference Counting

ToyForth uses **automatic memory management through reference counting**. Every [`tfobj`](src/tforth.h) has a `refcount` field that tracks the number of active references.

#### Ownership Semantics

The implementation establishes clear ownership rules to prevent memory leaks and use-after-free errors:

1. **Creating Functions**: Functions that allocate new objects return with `refcount=1` (creator owns it)
2. **Passing to Functions**: Passing an object to a function does NOT transfer ownership; the caller retains responsibility
3. **Container Operations**: When a container (like the stack) acquires a reference, it increments `refcount`
4. **Cleanup**: When a reference is no longer needed, the holder decrements `refcount`

#### Implementation in [`src/mem.c`](src/mem.c)

```c
void incrementReferenceCount(tfobj *object) {
    if (object == NULL) return;
    object->refcount++;
}

void decrementReferenceCount(tfobj *object) {
    if (object == NULL) return;

    object->refcount--;
    
    /* Automatic deallocation when no references remain */
    if (object->refcount == 0) {
        freeObject(object);
    }
}
```

When `refcount` reaches 0, the object is automatically freed. For container types (lists), deallocation recursively decrements refcounts of contained elements:

```c
if (object->type == TF_OBJ_LIST) {
    for (size_t i = 0; i < object->list_obj.len; i++) {
        tfobj *element = object->list_obj.element[i];
        decrementReferenceCount(element);  /* Recursive cleanup */
    }
    free(object->list_obj.element);
}
```

This pattern ensures no manual memory management bugs—memory is automatically freed when it's no longer needed.

### Parser and Compilation

The parser is implemented in [`src/parser.c`](src/parser.c) as an **iterative tokenizer** that converts source text into a compiled program list.

#### Tokenization

The parser maintains position state with line/column tracking:

```c
typedef struct {
    char *program;    /* Pointer to current position in program text */
    int line;         /* 1-based line number for error diagnostics */
    int column;       /* 1-based column number for error diagnostics */
} tfparser;
```

#### Parsing Numbers

Numbers are parsed using `strtol()` for robust handling of signs and overflow:

```c
tfobj *parseNumber(tfparser *parser) {
    char *start = parser->program;
    char *end;  

    long value = strtol(start, &end, 10);
    
    while (parser->program < end) {
        parserAdvance(parser);
    }
    
    return createIntegerObject(value);
}
```

#### Parsing Symbols

Symbols (Forth words) are validated against the operation dictionary during parsing. Unknown symbols are rejected with error reporting:

```c
tfobj *parseSymbol(tfparser *parser) {
    char *start = parser->program;

    while (!isspace(parserPeek(parser)) && parserPeek(parser) != '\0') {
        parserAdvance(parser);
    }

    size_t len_symbol = parser->program - start;
    tfobj *new_object = createSymbolObject(start, len_symbol);

    if (lookupOperation(new_object->str_obj.str) == NULL) {
        decrementReferenceCount(new_object);
        return NULL;  /* Unknown symbol */
    }

    return new_object;
}
```

#### Compilation Phase

The [`compile()`](src/parser.c) function produces a list of executable objects:

```c
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
```

### Virtual Machine and Execution Engine

The execution engine in [`src/engine.c`](src/engine.c) implements a simple fetch-execute loop:

```c
void execute(tfobj *program_list, tfcontext *context) {
    if (program_list == NULL || context == NULL) return;
     
    for (size_t i = 0; i < program_list->list_obj.len; i++) {
        tfobj *object = program_list->list_obj.element[i];

        if (object->type == TF_OBJ_INT || object->type == TF_OBJ_BOOL) {
            stackPush(context, object);
        } 
        else if (object->type == TF_OBJ_SYMBOL) {
            Operation op = lookupOperation(object->str_obj.str);
            
            if (op != NULL) {
                op(context);
            } else {
                fprintf(stderr, "Unknown word: %s\n", object->str_obj.str);
                exit(EXIT_FAILURE);
            }
        }
    }
}
```

For each compiled object:
- **Integers/Booleans**: Pushed onto the data stack
- **Symbols**: Looked up in the operation dictionary and executed

#### Stack Operations

The data stack is implemented as a dynamically resizing list. [`src/stack.c`](src/stack.c) provides push/pop operations with careful reference counting:

```c
void stackPush(tfcontext *context, tfobj *object) {
    listAppendObject(context->stack, object);
}

tfobj *stackPop(tfcontext *context) {
    if (context->stack->list_obj.len == 0) {
        fprintf(stderr, "Stack underflow error.\n");
        exit(EXIT_FAILURE);
    }

    size_t top_index = context->stack->list_obj.len - 1;
    tfobj *object = context->stack->list_obj.element[top_index];
    context->stack->list_obj.len--;

    return object;  /* Caller now owns this reference */
}
```

**Key insight**: [`stackPush()`](src/stack.c) increments refcount (stack acquires a reference), while [`stackPop()`](src/stack.c) does NOT decrement (caller acquires ownership).

### Operation Dictionary

The operation dictionary in [`src/dictionary.c`](src/dictionary.c) implements a static lookup table mapping operation names to function pointers:

```c
typedef void (*Operation)(tfcontext *context);

typedef struct {
    const char *operation;
    Operation op;
} OperationEntry;

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

Operation lookupOperation(const char *operation) {
    for (size_t i = 0; i < OP_COUNT; i++) {
        if (strcmp(operation, operations[i].operation) == 0) {
            return operations[i].op;
        }
    }
    return NULL;
}
```

All operations share a uniform signature, accepting a context and modifying VM state through stack operations.

#### Example Operation: Addition

From [`src/ops.c`](src/ops.c):

```c
void operationAdd(tfcontext *context) {
    tfobj *b = stackPop(context);
    tfobj *a = stackPop(context);

    if (a->type == TF_OBJ_INT && b->type == TF_OBJ_INT) {           
        tfobj *result = createIntegerObject(a->number + b->number);
        stackPush(context, result);
        decrementReferenceCount(result);  /* Release local reference */
    }

    decrementReferenceCount(a);
    decrementReferenceCount(b);
}
```

This pattern is common across all operations:
1. Pop operands (caller takes ownership)
2. Validate types
3. Create result object
4. Push result (stack acquires ownership)
5. Release local references via [`decrementReferenceCount()`](src/mem.c)

---

## License

This project is educational and intended for learning purposes.