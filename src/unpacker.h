#ifndef UNPACKER
#define UNPACKER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pak.h"
#include "utils.h"


/** Unpacks the sections within a .pak file.
 *
 * @param argc Arg Count
 * @param argv Arg List
 * @return 0 on success, 1 on error with the folder path, 2 on error with the pak path.
 */
int Unpack(int argc, char** argv);

#endif
