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

/** Counts all occurrences of a given character inside a string.
 *
 * @param str The string containing the character.
 * @param c The character to count.
 * @return The total amount of occurrences of the character inside the string.
 */
u32 StrCountChar(char* str, char c);

/** Creates a copy of the given string converted to lowercase.
 * @note must be manually freed.
 *
 * @param str The string to convert.
 * @return The lowercase string.
 */
char* StrLower(char* str);

/** Gets the file size of a specified file.
 * @note Only Linux and Windows compatibility as of right now.
 *
 * @param path The path to the file.
 * @return The filesize in bytes.
 */
u64 FileSize(const char* path);

/** Reads every line of a file.
 * @note Empty lines will be skipped.
 *
 * @param path The path to the file.
 * @param lineCount If a pointer is passed, the total line count will be written to it.
 * @return A string array containing each line.
 */
char** ReadFileLines(const char* path, int* lineCount);

/** Retrieves all files of a given directory.
 *
 * @param dirpath The path to the directory.
 * @return A pointer to a DirFiles struct containing the file list.
 */
DirFiles* ReadDirFiles(const char* dirpath);

/** Frees all memory related to a DirFiles object
 *
 * @param files A handle to the DirFiles object.
 */
void FreeDirFiles(DirFiles* files);

/** Frees all memory related to a string list created by ReadFileLines().
 *
 * @param list The string array.
 */
void FreeFileLines(char** list);

#endif
