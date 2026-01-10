#ifndef UTILS_H
#define UTILS_H

#include "types.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct _dirFiles {
    int count;
    char** list;
} DirFiles;

#ifdef _WIN32

#define PATH_SEP '\\'

#endif
#ifdef __linux__

#define PATH_SEP '/'

#endif

/**
 * Counts all occurrences of a given character inside a string
 */
u32 StrCountChar(char* str, char c);

char* StrLower(char* str);

/**
 * Reads every line of a file.
 * @note Empty lines will be skipped.
 */
char** ReadFileLines(const char* path, int* lineCount);

/**
 * Retrieves all files of a given directory.
 */
DirFiles* ReadDirFiles(const char* dirpath);

/**
 * Frees all memory related to a DirFiles object
 */
void FreeDirFiles(DirFiles* files);

/**
 * Frees all memory related to a string list created by ReadFileLines()
 */
void FreeFileLines(char** list);

#endif