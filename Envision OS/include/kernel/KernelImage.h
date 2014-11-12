///////////////////////////////////////////////////////////////////////////////////
//----------------------------------KernelImage.h--------------------------------//
//                                      v 1.0                                    //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
// Created by: Sidhant Goel                                                      //
// Created on: 5 march 2007                                                      //
//                                                                               //
//--------------------------------------Notes------------------------------------//
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
#ifndef KERNELIMAGE_H_INCLUDED
#define KERNELIMAGE_H_INCLUDED

#include "Kernel.h"
#include "KernelTasks.h"
#include "KernelPortableExecutable.h"

typedef struct _KIMAGE
{
    WCHAR *Name;
    WCHAR *Path;
    UINT32 Flags;
}KIMAGE;

typedef struct _KIMAGE_MAPPING
{
    KIMAGE      *Image;
    KPROCESS    *Process;
    VIRTUAL_PTR Base;
}KIMAGE_MAPPING;

KESTATUS STDCALL KeImageGetSize(SIZE_T *size,VIRTUAL_PTR image);

#endif // KERNELIMAGE_H_INCLUDED
///////////////////////////////////////////////////////////////////////////////////
//----------------------------------KernelImage.h--------------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
