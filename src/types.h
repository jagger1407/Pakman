#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

#include <stdint.h>

typedef uint8_t u8;
typedef int8_t s8;

typedef uint16_t u16;
typedef int16_t s16;

typedef uint32_t u32;
typedef int32_t s32;

typedef uint64_t u64;
typedef uint64_t s64;

typedef uint8_t byte;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;
typedef unsigned long long ulonglong;
typedef unsigned int uint;

#include <stdbool.h>
#define false 0
#define true 1

#define nullptr ((void*)0x00)

#ifdef _WIN32
#ifdef BUILDING
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __declspec(dllimport)
#endif
#else
#define EXPORT
#endif

#endif // TYPES_H_INCLUDED
