#include "decompressor.h"

typedef struct _dictEntry {
    s32 byte;
    u8 pair[2];
} DictEntry;

typedef struct _dict {
    DictEntry entries[256];
} Dict;

typedef struct _bpe {
    u32 decompressedSize;
    u32 compressedSize;
    Dict dict;

    FILE* fp;
} BPE;

u32 fillDict(Dict* dict, u8* data, u32 fsize) {
    u8* cur = data;
    u32 idx = 0;

    for(int i=0;i<256;i++) {
        dict->entries[i].byte = -1;
        dict->entries[i].pair[0] = 0x00;
        dict->entries[i].pair[1] = 0x00;
    }

    while(idx < 256 && cur < (data + fsize)) {
        if(*cur > 0x7F) {
            idx += *cur - 0x7F;
            if(idx >= 256) {
                cur++;
                break;
            }
            cur++;
            if(*cur == idx) {
                cur++;
                idx++;
                continue;
            }
            
            dict->entries[idx].byte = idx;
            dict->entries[idx].pair[0] = *cur;
            cur++;
            dict->entries[idx].pair[1] = *cur;
            cur++;
            idx++;
        } 
        else {
            int count = *cur;
            cur++;
            for(int i=0;i<=count;i++) {
                if(*cur == idx) {
                    idx++;
                    cur++;
                    continue;
                }
                dict->entries[idx].byte = idx;
                dict->entries[idx].pair[0] = *cur;
                cur++;
                dict->entries[idx].pair[1] = *cur;
                cur++;
                idx++;
            }
        }
    }

    return cur - data;
}

int decompressBlock(Dict* dict, u8* cptr, u8* dptr, u32 blocksize) {
    u8* end = cptr + blocksize;
    u8* start = dptr;

    u8 stack[256];
    u8 sp = 0;
    while(cptr < end) {
        if(dict->entries[*cptr].byte == -1) {
            *dptr = *cptr;
            dptr++;
            cptr++;
        }
        else {
            stack[sp] = dict->entries[*cptr].pair[1];
            sp++;
            stack[sp] = dict->entries[*cptr].pair[0];
            sp++;
            while(sp > 0) {
                if(dict->entries[stack[sp-1]].byte != -1) {
                    sp--;
                    u8 c = stack[sp];
                    stack[sp] = dict->entries[c].pair[1];
                    sp++;
                    stack[sp] = dict->entries[c].pair[0];
                    sp++;
                }
                else {
                    *dptr = stack[sp-1];
                    dptr++;
                    sp--;
                }
            }
            cptr++;
        }
    }

    return dptr - start;
}

const char* decompressHelpText =
    "Pakman Decompressor\n" \
    "Syntax: pakman decompress [cpak path] [output pak path]\n";

int Decompress(int argc, char** argv) {
    if(argc <= 2) {
        puts(decompressHelpText);
        return 0;
    }

    if(argc <= 2) {
        fputs("ERROR: Missing the cpak path.\n", stderr);
        return 1;
    }
    if(argc <= 3) {
        fputs("ERROR: Missing the output pak path.\n", stderr);
        return 2;
    }
    if(argv[3] == NULL || *argv[3] == 0x00) {
        fputs("ERROR: Invalid output pak path.\n", stderr);
        return 2;
    }

    BPE* bpe = (BPE*)malloc(sizeof(BPE));
    memset(bpe, 0x00, sizeof(BPE));
    bpe->fp = fopen(argv[2], "rb");
    if(bpe->fp == NULL) {
        fputs("ERROR: Opening .cpak file failed.\n", stderr);
        return 1;
    }
    FILE* output = fopen(argv[3], "wb+");
    if(output == NULL) {
        fprintf(stderr, "ERROR: Creating file '%s' failed.\n", argv[3]);
        perror(NULL);
        return 2;
    }

    u32 fsize = GetFileSize(argv[2]);
    u8* rawdata = (u8*)malloc(fsize);
    fread(rawdata, 1, fsize, bpe->fp);

    bpe->decompressedSize = *((u32*)rawdata);
    bpe->compressedSize = *((u32*)rawdata+1);

    u8* outdata = (u8*)malloc(bpe->decompressedSize);
    memset(outdata, 0x00, bpe->decompressedSize);

    u8* data = rawdata + 8;
    u32 leftover = fsize - 8;
    u8* curout = outdata;
    u32 cnt = 0;
    while(leftover > 0) {
        if(curout >= (outdata + bpe->decompressedSize)) {
            break;
        }
        u32 off = fillDict(&bpe->dict, data, leftover);
        u16 blocksize = data[off] << 8;
        blocksize |= data[off+1];
        data += off+2;

        int write = decompressBlock(&bpe->dict, data, curout, blocksize);
        data += blocksize;
        leftover -= blocksize;
        curout += write;
        cnt++;
    }

    fwrite(outdata, 1, bpe->decompressedSize, output);
    printf("decompressed file successfully saved as '%s'.\n", argv[3]);
    
    free(rawdata);
    free(outdata);
    fclose(output);
    fclose(bpe->fp);
    free(bpe);
    return 0;
}