#ifndef PAK_H
#define PAK_H

#include "types.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h> 

typedef struct _pakSection {
    u8* data;
    u32 size;
} PakSection;

typedef struct _pak {
    u32 amountSections;
    u32* offsets;
    FILE* fp;
    PakSection* sections;
} Pak;

#define PAK_DEFAULTALIGN 0x10

/** 
 * Creates an empty Pak object
 */
Pak* pak_Create(const char* path);
/**
 * Creates a Pak object from an existing .pak file
 * @note Optionally reads the file contents into a PakSection array
 */
Pak* pak_Open(const char* path, int readSections);
/**
 * Frees all Pak related memory
 */
void pak_Free(Pak* pak);

/**
 * Gets the section size of the given section
 */
u32 pak_GetSectionSize(Pak* pak, int id);

/**
 * Reads the section from the file and extracts it into a buffer.
 * @note The buffer must be large enough to hold the section data!
 */
u8* pak_ReadExtractSection(Pak* pak, int id, u8* buffer);
/**
 * Reads all sections of the .pak file into a PakSection object array
 */
PakSection* pak_ReadSections(Pak* pak);

/**
 * Calculates the entire Pak::offsets array
 */
void pak_RecalculateOffsets(Pak* pak, u32 align);
/**
 * Sets the sections member and the amountSections of a Pak object.
 * @note The sections array passed here will be freed by pak_Free()
 */
void pak_SetSections(Pak* pak, PakSection* sections, u32 amountSections);

/**
 * Saves the current Pak object by using the offsets and sections arrays.
 */
void pak_Save(Pak* pak);

#endif