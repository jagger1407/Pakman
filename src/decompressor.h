#ifndef DECOMPRESSOR
#define DECOMPRESSOR

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pak.h"
#include "utils.h"
    
/** Decompresses a CPAK.
 *
 * @param argc Arg Count
 * @param argv Arg List
 * @return 0 on success, 1 on error with the cpak path, 2 on error with the output pak path.
 */
int Decompress(int argc, char** argv);

#endif
