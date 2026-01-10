#include "types.h"
#include "packer.h"
#include "unpacker.h"
#include <string.h>

const char* helpText =  
    "Pakman\n" \
    "The program for all your Pak needs.\n\n" \
    "Syntax: pakman pack|unpack\n\n" \
    "Use either one without any arguments to see its help text.\n";

int main(int argc, char** argv) {
    if(argc <= 1) {
        puts(helpText);
        return 0;
    }

    char* arglow = StrLower(argv[1]);

    if(strcmp(arglow, "pack") == 0) {
        return Pack(argc, argv);
    }
    else if(strcmp(arglow, "unpack") == 0) {
        return Unpack(argc, argv);
    }
    else {
        printf("Unrecognized option '%s'.\n", argv[1]);
    }
    return 1;
}