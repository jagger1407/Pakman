#include "packer.h"

const char* packHelpText =  
    "Pakman Packer\n" \
    "Syntax: pakman pack [pak path] [input folder path] [section name file] <custom alignment>\n" \
    "Optionally, a custom alignment for files (default: 16 Bytes) can be specified.";

int Pack(int argc, char** argv) {
    if(argc <= 2) {
        puts(packHelpText);
        return 0;
    }

    if(argc <= 3) {
        fputs("ERROR: Missing the input folder path.\n", stderr);
        return 1;
    }
    if(argv[3] == NULL || *argv[3] == 0x00) {
        fputs("ERROR: Invalid input folder path.\n", stderr);
        return 1;
    }

    // Reading the section names
    int scount = 0;
    char** sectionNames = NULL;
    if(argc <= 4) {
        fputs("ERROR: Missing the pak section file.\n", stderr);
        return 3;
    }
    sectionNames = ReadFileLines(argv[4], &scount);

    // Getting the alignment if available
    int alignment = PAK_DEFAULTALIGN;
    if(argc >= 6) {
        alignment = atoi(argv[5]);
    }

    // Read the files inside the input dir
    int fcount = 0;
    DirFiles* df = ReadDirFiles(argv[3]);

    // Check whether the user has all the necessary files for the .pak
    int missing = 0;
    for(int i=0;i<scount;i++) {
        if(*(sectionNames[i]) == 0x00) continue;
        int exists = false;
        for(int f=0;f<df->count;f++) {
            if(strcmp(sectionNames[i], df->list[f]) == 0) {
                exists = true;
                break;
            }
        }
        if(!exists) {
            missing++;
            printf("Section '%s' is missing.\n", sectionNames[i]);
        }
    }

    if(missing > 0) {
        FreeFileLines(sectionNames);
        FreeDirFiles(df);
        fputs("ERROR: Sections are missing.", stderr);
        return 4;
    }

    // Reading all the files
    PakSection* sections = (PakSection*)malloc(scount * sizeof(PakSection));
    int plen = strlen(argv[3]);
    char path[0x400];
    strncpy(path, argv[3], 0x400);
    if(path[plen-1] != PATH_SEP) {
        path[plen] = PATH_SEP;
        plen++;
    }
    char* name = path + plen;
    for(int i=0;i<scount;i++) {
        if(*(sectionNames[i]) == 0x00) continue;
        strncpy(name, sectionNames[i], 0x400 - plen);
        FILE* fp = fopen(path, "rb+");
        if(fp == NULL) {
            missing++;
            fprintf(stderr, "ERROR: File '%s' couldn't be opened.\n", path);
            perror("");
            FreeDirFiles(df);
            FreeFileLines(sectionNames);
            return 4;
        }

        sections[i].size = GetFileSize(path);

        sections[i].data = (u8*)malloc(sections[i].size);
        fread(sections[i].data, 1, sections[i].size, fp);
        
        fclose(fp);
    }

    // Create the .pak, then put the sections in it
    printf("Creating '%s'...\n", argv[2]);
    Pak* pak = pak_Create(argv[2]);
    printf("Inserting sections...\n");
    pak_SetSections(pak, sections, scount);
    pak_RecalculateOffsets(pak, alignment);
    pak_Save(pak);
    printf("'%s' has been successfully created.\n", argv[2]);

    // Cleanup
    pak_Free(pak);
    FreeDirFiles(df);
    FreeFileLines(sectionNames);
    return 0;
}