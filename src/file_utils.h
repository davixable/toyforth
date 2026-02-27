/*
 * File I/O Utilities Module
 *
 * Provides convenient file reading functionality for loading ToyForth source
 * programs. Automatically handles file sizing, memory allocation, and error
 * reporting.
 */

#ifndef FILE_UTILS_H
#define FILE_UTILS_H

/*
 * readFromFile() - Reads entire file contents into memory
 *
 * Opens a file, determines its size, allocates a buffer with wmalloc(),
 * and reads the complete contents. Appends a NUL terminator for safe string
 * processing. Returns a newly allocated buffer that the caller is responsible
 * for freeing.
 *
 * On file open failure, prints error message and terminates the program.
 *
 * Args:
 *   filename - Path to file to read (relative or absolute)
 *
 * Returns:
 *   Heap-allocated buffer (refcount maintained by caller) containing the
 *   file contents as a NUL-terminated string
 */
char* readFromFile(const char *filename);

#endif  