///////////////////////////////////////////////////////////////////////////////////
//----------------------------------KernelTypes.h--------------------------------//
//                                      v 1.0                                    //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
// Created by: Sidhant Goel                                                      //
// Created on: 14 feb 2007                                                       //
//                                                                               //
//--------------------------------------Notes------------------------------------//
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
#ifndef KERNELTYPES_H_INCLUDED
#define KERNELTYPES_H_INCLUDED

typedef char CHAR;
typedef short SHORT;
typedef int INT;
typedef _int32 INT32;
typedef _int64 INT64;
typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned int UINT;
typedef unsigned _int32 UINT32;
typedef unsigned _int64 UINT64;
typedef signed int SINT;
typedef signed _int32 SINT32;
typedef signed _int64 SINT64;
typedef signed short SSHORT;
typedef unsigned char BYTE;
typedef USHORT WORD;
typedef UINT32 DWORD;
typedef unsigned long LONG;
typedef USHORT WCHAR;

typedef unsigned char UINT8;
typedef signed char SINT8;
typedef char INT8;
typedef unsigned short UINT16;
typedef signed short SINT16;
typedef short INT16;

#if MACHINE_WIDTH==32
typedef UINT32 NATIVE_UINT;
typedef INT32 NATIVE_INT;
#elif MACHINE_WIDTH==64
typedef UINT64 NATIVE_UINT;
typedef INT64 NATIVE_INT;
#else
#error MACHINE_WIDTH Invalid
#endif

typedef UINT32 BOOLEAN;

typedef UINT64 PHYSICAL_PTR;

typedef BYTE* VIRTUAL_PTR;
typedef size_t SIZE_T;
typedef void* HANDLE;

//#define LODWORD(x)     (*((UINT32*)&x))
//#define HIDWORD(x)     (*(((UINT32*)&x)+1))
/*
 * Data manipulation macros
 */

typedef struct _QWORD_STRUCT
{
    UINT32 LoDword;
    UINT32 HiDword;
}QWORD_STRUCT;

#define HIDWORD(l)                  ((UINT32)((QWORD_STRUCT*)(&l))->HiDword)
#define LODWORD(l)                  ((UINT32)(l))
#define LOWORD(l)                   ((UINT16)(UINT32)(l))
#define HIWORD(l)                   ((UINT16)((((UINT32)(l)) >> 16) & 0xFFFF))
#define LOBYTE(l)                   ((UINT8)(UINT16)(l))
#define HIBYTE(l)                   ((UINT8)((((UINT16)(l)) >> 8) & 0xFF))

#define STDCALL     __stdcall

#include "KernelStatus.h"

typedef struct _KPROCESS KPROCESS;
typedef struct _KTHREAD KTHREAD;
typedef struct _KMEMORY_INFO KMEMORY_INFO;
typedef struct _KIMAGE KIMAGE;
typedef struct _KIMAGE_MAPPING KIMAGE_MAPPING;

#endif // KERNELTYPES_H_INCLUDED
///////////////////////////////////////////////////////////////////////////////////
//----------------------------------KernelTypes.h--------------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
