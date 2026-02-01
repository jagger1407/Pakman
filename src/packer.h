#ifndef PACKER
#define PACKER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pak.h"
#include "utils.h"
    

/** Creates a .pak file from a folder.
 *
 * @param argc Arg Count
 * @param argv Arg List
 * @return 0 on success, 1 on error with the folder path, 2 on error with the pak section file, 3 on error with the output path.
 */
int Pack(int argc, char** argv);

#endif
