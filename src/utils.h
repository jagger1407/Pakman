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
#include <windows.h>

#endif
#ifdef __linux__

#define PATH_SEP '/'
#include <dirent.h>
#include <sys/stat.h>

#endif

/**
 * Counts all occurrences of a given character inside a string
 */
u32 StrCountChar(char* str, char c);

/**
 * Creates a copy of the string in lowercase.
 * @note Must be manually freed.
 */
char* StrLower(char* str);

/**
 * Gets the file size of a specified file.
 * @note Only Linux and Windows compatibility as of right now.
 */
u64 GetFileSize(const char* path);

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
