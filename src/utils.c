#include "utils.h"

u32 StrCountChar(char* str, char c) {
    u32 occ = 0;
    while(*str != 0x00) {
        if(*str == c) occ++;
        str++;
    }
    return occ;
}

char* StrLower(char* str) {
    int len = strlen(str);
    char* lwr = (char*)malloc(len+1);
    lwr[len] = 0x00;
    strcpy(lwr, str);

    char* cur = lwr;
    while(*cur != 0x00) {
        if(*cur >= 'A' && *cur <= 'Z') {
            *cur = *cur + 0x20;
        }
        cur++;
    }
    return lwr;
}

u64 FileSize(const char* path) {
    if (path == NULL || strlen(path) == 0) {
        return 0;
    }

    u64 size = 0;

#ifdef _WIN32

    WIN32_FILE_ATTRIBUTE_DATA info;
    
    if (!GetFileAttributesExA(path, GetFileExInfoStandard, &info)) {
        return 0;
    }
    if (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        return 0;
    }
    
    size = ((u64)info.nFileSizeHigh << 32) | info.nFileSizeLow;

#endif
#ifdef __linux__

    struct stat st;
    stat(path, &st);
    if(S_ISDIR(st.st_mode)) {
        return 0;
    }
    size = st.st_size;

#endif

    return size;
}

char** ReadFileLines(const char* path, int* lineCount) {
    if(path == NULL || *path == 0x00) {
        fputs("ERROR: section name file invalid.\n", stderr);
        return NULL;
    }
    FILE* fp = fopen(path, "r");
    if(fp == NULL) {
        fputs("ERROR: section name file couldn't be opened.\n", stderr);
        fprintf(stderr, "Path: '%s'\n", path);
        perror(NULL);
        return NULL;
    }

    u64 size = FileSize(path);

    char* rawFile = (char*)malloc(size+1);
    rawFile[size] = 0x00;

    fread(rawFile, 1, size, fp);

    int lc = 0;
    if(lineCount == NULL) lineCount = &lc;
    *lineCount = StrCountChar(rawFile, '\n') + 1;
    char** list = (char**)malloc(*lineCount * sizeof(char*));
    
    char* cur = rawFile;
    list[0] = rawFile;
    int line = 1;
    while(*cur != 0x00) {
        if(*cur == '\n') {
            if(cur[1] == '\n') {
                *cur = 0x00;
                cur++;
                (*lineCount)--;
                continue;
            }
            if(cur[1] == 0x00) {
                *cur = 0x00;
                (*lineCount)--;
                break;
            }
            *cur = 0x00;
            list[line] = cur + 1;
            line++;
        }
        cur++;
    }

    lineCount = NULL;
    return list;
}

DirFiles* ReadDirFiles(const char* dirpath) {
    DirFiles* files = (DirFiles*)malloc(sizeof(DirFiles));

    int fcount = 0;
#ifdef _WIN32

    char searchpath[MAX_PATH];
    snprintf(searchpath, sizeof(searchpath), "%s\\*", dirpath);
    
    WIN32_FIND_DATA fd;
    HANDLE hFind = FindFirstFile(searchpath, &fd);
    
    if (hFind == INVALID_HANDLE_VALUE) {
        fputs("Error opening directory.\n", stderr);
        perror(NULL);
        return NULL;
    }

    do {
        if (strcmp(fd.cFileName, ".") == 0 || 
            strcmp(fd.cFileName, "..") == 0 ||
            (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            continue;
        }
        
        fcount++;
        
    } while (FindNextFile(hFind, &fd) != 0);
    FindClose(hFind);

    files->count = fcount;
    files->list = (char**)malloc(fcount * sizeof(char*));
    fcount = 0;

    FindFirstFile(searchpath, &fd);
    if (hFind == INVALID_HANDLE_VALUE) {
        fputs("Error opening directory.\n", stderr);
        perror(NULL);
        return NULL;
    }
    do {
        if (strcmp(fd.cFileName, ".") == 0 || 
            strcmp(fd.cFileName, "..") == 0 ||
            (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            continue;
        }
        
        files->list[fcount] = (char*)malloc(0x100);
        strncpy(files->list[fcount], fd.cFileName, 0xFF);
        fcount++;

    } while (FindNextFile(hFind, &fd) != 0);

#endif
#ifdef __linux__

    DIR* dir = opendir(dirpath);
    if(!dir) {
        perror(NULL);
        return NULL;
    }
    char fpath[0x400];
    struct dirent* entry = readdir(dir);
    while(entry != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || 
            strcmp(entry->d_name, "..") == 0) {
            entry = readdir(dir);
            continue;
        }
        memset(fpath, 0x00, 0x400);
        strncpy(fpath, dirpath, 0x3FF);
        if(fpath[strlen(fpath)-1] != PATH_SEP) {
            fpath[strlen(fpath)] = PATH_SEP;
        }
        strcat(fpath, entry->d_name);
        struct stat st;
        lstat(fpath, &st);
        if(S_ISDIR(st.st_mode)) {
            continue;
        }
        
        fcount++;

        entry = readdir(dir);
    }
    
    rewinddir(dir);
    files->count = fcount;
    files->list = (char**)malloc(fcount * sizeof(char*));
    fcount = 0;

    entry = readdir(dir);
    while(entry != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || 
            strcmp(entry->d_name, "..") == 0) {
            entry = readdir(dir);
            continue;
        }
        memset(fpath, 0x00, 0x400);
        strncpy(fpath, dirpath, 0x3FF);
        if(fpath[strlen(fpath)-1] != PATH_SEP) {
            fpath[strlen(fpath)] = PATH_SEP;
        }
        strcat(fpath, entry->d_name);
        struct stat st;
        lstat(fpath, &st);
        if(S_ISDIR(st.st_mode)) {
            continue;
        }
        
        files->list[fcount] = (char*)malloc(0x100);
        strncpy(files->list[fcount], entry->d_name, 0xFF);
        fcount++;

        entry = readdir(dir);
    }

#endif

    return files;
}

void FreeDirFiles(DirFiles* files) {
    if(files == NULL) return;
    for(int i=0;i<files->count;i++) {
        free(files->list[i]);
    }
    free(files->list);
    files->list = NULL;

    free(files);
}

void FreeFileLines(char** list) {
    if(list == NULL) return;
    free(list[0]);
    free(list);
}
