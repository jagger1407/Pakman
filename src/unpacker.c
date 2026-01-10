#include "unpacker.h"

const char* unpackHelpText =
    "Pakman Unpacker\n" \
    "Syntax: pakman unpack [pak path] [output folder path] <section name file>\n" \
    "You can optionally provide a .txt file containing the names\n" \
    "for each subsection (one per line).";

int Unpack(int argc, char** argv) {
    if(argc <= 2) {
        puts(unpackHelpText);
        return 0;
    }

    if(argc <= 3) {
        fputs("ERROR: Missing the output folder path.\n", stderr);
        return 1;
    }
    if(argv[3] == NULL || *argv[3] == 0x00) {
        fputs("ERROR: Invalid output folder path.\n", stderr);
        return 1;
    }

    char** sectionNames = NULL;
    if(argc >= 5) {
        sectionNames = ReadFileLines(argv[3], NULL);
    }

    Pak* pak = pak_Open(argv[2], false);
    if(pak == NULL) {
        fputs("ERROR: Opening .pak file failed.\n", stderr);
        return 2;
    }
    int oflen = strlen(argv[3]);
    for(int i=0;i<pak->amountSections;i++) {
        char outname[0x100];
        memset(outname, 0x00, 0x100);
        strncpy(outname, argv[3], 0x100);
        if(outname[oflen-1] != PATH_SEP)
            outname[oflen] = PATH_SEP;
        if(sectionNames != NULL) {
            strcat(outname, sectionNames[i]);
        }
        else {
            char num[0x20];
            sprintf(num, "Section_%d", i);
            strcat(outname, num);
        }
        
        FILE* fp = fopen(outname, "wb+");
        u32 size = pak_GetSectionSize(pak, i);
        u8* section = (u8*)malloc(size);
        pak_ReadExtractSection(pak, i, section);
        fwrite(section, 1, size, fp);
        free(section);
        fclose(fp);

        printf("Section %d extracted to '%s'.\n", i, outname);
    }

    pak_Free(pak);
    FreeFileLines(sectionNames);
    return 0;
}