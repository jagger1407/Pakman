#include "pak.h"

Pak* pak_Create(const char* path) {
    if(path == NULL || *path == 0x00) {
        fputs("pak_Open - Empty path argument.\n", stderr);
        return NULL;
    }
    FILE* file = fopen(path, "wb+");
    if(file == NULL) {
        fputs("pak_Open - fopen failed.\n", stderr);
        fprintf(stderr, "Path: '%s'\n", path);
        perror(NULL);
        return NULL;
    }

    Pak* pak = (Pak*)malloc(sizeof(Pak));
    pak->amountSections = 0;
    pak->offsets = NULL;
    pak->fp = file;
    pak->sections = NULL;
}

Pak* pak_Open(const char* path, int readSections) {
    if(path == NULL || *path == 0x00) {
        fputs("pak_Open - Empty path argument.\n", stderr);
        return NULL;
    }
    FILE* file = fopen(path, "rb+");
    if(file == NULL) {
        fputs("pak_Open - fopen failed.\n", stderr);
        fprintf(stderr, "Path: '%s'\n", path);
        perror(NULL);
        return NULL;
    }

    Pak* pak = (Pak*)malloc(sizeof(Pak));
    pak->fp = file;
    fread(&pak->amountSections, sizeof(u32), 1, pak->fp);
    if(pak->amountSections <= 0) {
        pak->offsets = NULL;
        return pak;
    }
    pak->offsets = (u32*)malloc((pak->amountSections + 1) * sizeof(u32));
    fread(pak->offsets, sizeof(u32), pak->amountSections + 1, pak->fp);

    if((pak->amountSections + 2) * sizeof(u32) > pak->offsets[0]) {
        fputs("pak_Open - File size mismatch (Header > First offset)\n", stderr);
        pak_Free(pak);
        return NULL;
    }
    fseek(pak->fp, 0, SEEK_SET);
    u32 size = ftell(pak->fp);
    fseek(pak->fp, 0, SEEK_END);
    size = ftell(pak->fp) - size;
    if(size != pak->offsets[pak->amountSections]) {
        fputs("pak_Open - File size mismatch (File size != Last offset)\n", stderr);
        pak_Free(pak);
        return NULL;
    }

    pak->sections = NULL;
    if(readSections) {
        pak_ReadSections(pak);
    }

    return pak;
}

void pak_Free(Pak* pak) {
    if(pak == NULL) return;
    if(pak->fp != NULL) {
        fclose(pak->fp);
        pak->fp = NULL;
    }
    if(pak->offsets != NULL) {
        free(pak->offsets);
        pak->offsets = NULL;
    }
    if(pak->sections != NULL) {
        for(int i=0;i<pak->amountSections;i++) {
            free(pak->sections[i].data);
        }
        free(pak->sections);
        pak->sections = NULL;
    }

    free(pak);
}

u32 pak_GetSectionSize(Pak* pak, int id) {
    if(pak == NULL) {
        fputs("pak_GetSectionSize - NULL pointer passed.\n", stderr);
        return 0;
    }
    if(pak->amountSections <= 0) {
        fputs("pak_GetSectionSize - Empty pak.\n", stderr);
        return 0;
    }
    if(id < 0 || id >= pak->amountSections) {
        fputs("pak_GetSectionSize - id out of range.\n", stderr);
        return 0;
    }

    return pak->offsets[id+1] - pak->offsets[id];
}

PakSection* pak_ReadSections(Pak* pak) {
    if(pak == NULL) {
        fputs("pak_ReadSections - NULL pak passed.\n", stderr);
        return NULL;
    }
    if(pak->amountSections <= 0) {
        fputs("pak_ReadSections - Empty pak.\n", stderr);
        return NULL;
    }
    if(pak->sections != NULL) {
        for(int i=0;i<pak->amountSections;i++) {
            free(pak->sections[i].data);
        }
        free(pak->sections);
    }
    pak->sections = (PakSection*)malloc(pak->amountSections * sizeof(PakSection));
    for(int i=0;i<pak->amountSections;i++) {
        pak->sections[i].size = pak_GetSectionSize(pak, i);
        pak->sections[i].data = (u8*)malloc(pak->sections[i].size);
        fseek(pak->fp, pak->offsets[i], SEEK_SET);
        fread(pak->sections[i].data, 1, pak->sections[i].size, pak->fp);
    }
    return pak->sections;
}

u8* pak_ReadExtractSection(Pak* pak, int id, u8* buffer) {
    if(pak == NULL) {
        fputs("pak_ReadExtractSection - NULL pak passed.\n", stderr);
        return NULL;
    }
    if(pak->amountSections <= 0) {
        fputs("pak_ReadExtractSection - Empty pak.\n", stderr);
        return NULL;
    }
    if(id < 0 || id >= pak->amountSections) {
        fputs("pak_ReadExtractSection - id out of range.\n", stderr);
        return NULL;
    }
    if(buffer == NULL) {
        fputs("pak_ReadExtractSection - NULL buffer passed.\n", stderr);
        return NULL;
    }

    fseek(pak->fp, pak->offsets[id], SEEK_SET);
    fread(buffer, 1, pak_GetSectionSize(pak, id), pak->fp);
    return buffer;
}

void pak_RecalculateOffsets(Pak* pak, u32 align) {
    if(pak == NULL) {
        fputs("pak_RecalculateOffsets - NULL pak passed.\n", stderr);
        return;
    }
    if(pak->sections == NULL) {
        fputs("pak_RecalculateOffsets - Need sections ptr for offset calculation.\n", stderr);
        return;
    }

    u32 curoff = (pak->amountSections + 2) * 4;
    curoff = (curoff + align-1) & ~(align-1);

    if(pak->offsets == NULL) {
        pak->offsets = (u32*)malloc((pak->amountSections+1) * sizeof(u32));
    }

    for(int i=0;i<pak->amountSections;i++) {
        pak->offsets[i] = curoff;
        curoff += pak->sections[i].size;
        curoff = (curoff + align-1) & ~(align-1);
    }
    pak->offsets[pak->amountSections] = curoff;
}

void pak_SetSections(Pak* pak, PakSection* sections, u32 amountSections) {
    if(pak == NULL) {
        fputs("pak_SetSections - NULL pak passed.\n", stderr);
        return;
    }
    if(sections == NULL) {
        fputs("pak_SetSections - NULL sections passed.\n", stderr);
        return;
    }
    if(pak->offsets != NULL) {
        free(pak->offsets);
        pak->offsets = NULL;
    }
    if(sections == NULL || amountSections <= 0) {
        return;
    }
    pak->amountSections = amountSections;
    pak->sections = sections;
}

void pak_Save(Pak* pak) {
    if(pak == NULL) {
        fputs("pak_Save - NULL pak passed.\n", stderr);
        return;
    }
    if(pak->offsets == NULL)
        pak_RecalculateOffsets(pak, PAK_DEFAULTALIGN);
    fseek(pak->fp, 0, SEEK_SET);
    fwrite(&pak->amountSections, sizeof(u32), 1, pak->fp);
    fwrite(pak->offsets, sizeof(u32), pak->amountSections+1, pak->fp);

    for(int i=0;i<pak->amountSections;i++) {
        fseek(pak->fp, pak->offsets[i], SEEK_SET);
        fwrite(pak->sections[i].data, 1, pak->sections[i].size, pak->fp);
    }
}