///////////////////////////////////////////////////////////////////////////////////
//--------------------------------KernelNamespace.h------------------------------//
//                                      v 1.0                                    //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
// Created by: Sidhant Goel                                                      //
// Created on: 16 feb 2007                                                       //
//                                                                               //
//--------------------------------------Notes------------------------------------//
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
#ifndef KERNELNAMESPACE_H_INCLUDED
#define KERNELNAMESPACE_H_INCLUDED
#include <Kernel.h>
#include <KernelMemoryManager.h>
#include <KernelVolumeManager.h>

typedef struct _KNAMESPACE_FILE KNAMESPACE_FILE;
typedef struct _KNAMESPACE_DIR  KNAMESPACE_DIR;
typedef struct _KNAMESPACE      KNAMESPACE;

#define FILE_TYPE_UNKNOWN       0x00
#define FILE_TYPE_DEVICE        0x01
#define FILE_TYPE_VOLUME        0x02

typedef struct _KNAMESPACE_FILE
{
    WCHAR           *Name;
    UINT32          Type;
    union
    {
        void*       *Unknown;
        KDEVICE     *Device;
        KVOLUME     *Volume;
    };
    KNAMESPACE_DIR  *Parent;
    KNAMESPACE_FILE *Next;
    KNAMESPACE_FILE *Prev;
}KNAMESPACE_FILE;

typedef struct _KNAMESPACE_DIR
{
    WCHAR           *Name;
    UINT32          FileCount;
    UINT32          DirCount;
    KNAMESPACE_DIR  *FirstDir;
    KNAMESPACE_FILE *FirstFile;
    KNAMESPACE_DIR  *Parent;
    KNAMESPACE_DIR  *Next;
    KNAMESPACE_DIR  *Prev;
}KNAMESPACE_DIR;

typedef struct _KNAMESPACE
{
    KNAMESPACE_DIR  *RootDir;
}KNAMESPACE;

KESTATUS STDCALL KeNamespaceInit();

#endif // KERNELNAMESPACE_H_INCLUDED
///////////////////////////////////////////////////////////////////////////////////
//--------------------------------KernelNamespace.h------------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
