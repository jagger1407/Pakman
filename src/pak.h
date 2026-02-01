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

/** Creates an empty Pak object.
 *
 * @param path A path to the .pak file to open.
 * @return A handle to the Pak object.
 */
Pak* pak_Create(const char* path);

/** Creates a Pak object from an existing .pak file.
 *
 * @param path A path to the .pak file to open.
 * @param readSections (Optional) Pass a non-zero value to read sections into a PakSection array.
 * @return A handle to the Pak object.
 */
Pak* pak_Open(const char* path, int readSections);

/** Frees all Pak related memory.
 *
 * @param pak The Pak object to free.
 */
void pak_Free(Pak* pak);

/** Gets the size of the given section.
 *
 * @param pak A handle to the Pak object.
 * @param id The index of the section.
 * @return The size of the section.
 */
u32 pak_GetSectionSize(Pak* pak, int id);

/** Reads the section from the file and extracts it into a buffer.
 * @note The buffer must be large enough to hold the section data!
 *
 * @param pak A handle to the Pak object.
 * @param id The index of the section
 * @param buffer The buffer which will hold the section data.
 * @return NULL if an error occurred, 'buffer' if successful.
 */
u8* pak_ReadExtractSection(Pak* pak, int id, u8* buffer);

/** Reads all sections of the .pak file into a PakSection object array
 * @note This requires the pak to have an opened FILE*.
 *
 * @param pak A handle to the Pak object.
 * @return The PakSection array.
 */
PakSection* pak_ReadSections(Pak* pak);

/** Recalculates the offsets inside pak->offsets.
 *
 * @param pak A handle to the Pak object.
 * @param align The alignment of the sections. Must be a power of 2.
 */
void pak_RecalculateOffsets(Pak* pak, u32 align);

/** Sets the sections member and the amountSections of a Pak object.
 * @note The sections array passed here will be freed by pak_Free()
 *
 * @param pak A handle to the Pak object.
 * @param sections An array of PakSection objects.
 * @param amountSections The amount of elements in the sections array.
 */
void pak_SetSections(Pak* pak, PakSection* sections, u32 amountSections);

/** Saves the current Pak object by using the offsets and sections arrays.
 *
 * @param pak A handle to the Pak object.
 */
void pak_Save(Pak* pak);

#endif








