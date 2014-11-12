///////////////////////////////////////////////////////////////////////////////////
//--------------------------------KernelResources.h------------------------------//
//                                      v 1.0                                    //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
// Created by: Sidhant Goel                                                      //
// Created on: 7 march 2007                                                      //
//                                                                               //
//--------------------------------------Notes------------------------------------//
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
#ifndef KERNELRESOURCES_H_INCLUDED
#define KERNELRESOURCES_H_INCLUDED
#include <Kernel.h>
#include <KernelMemoryManager.h>
#include <KernelPortableExecutable.h>
#include <KernelBitmap.h>
#include <KernelDebug.h>

#define RT_CURSOR           1
#define RT_BITMAP           2
#define RT_ICON             3
#define RT_MENU             4
#define RT_DIALOG           5
#define RT_STRING           6
#define RT_FONTDIR          7
#define RT_FONT             8
#define RT_ACCELERATOR      9
#define RT_RCDATA           10
#define RT_MESSAGETABLE     11


KESTATUS STDCALL KeResDirectory(VIRTUAL_PTR Base,IMAGE_RESOURCE_DIRECTORY **Dir);
KESTATUS STDCALL KeResDirList(IMAGE_RESOURCE_DIRECTORY *pRoot,IMAGE_RESOURCE_DIRECTORY *Dir);
KESTATUS STDCALL KeResFind(IMAGE_RESOURCE_DIRECTORY *pRoot,IMAGE_RESOURCE_DIRECTORY *Dir,VIRTUAL_PTR *Data,UINT32 *IsDir,UINT32 Id);
KESTATUS STDCALL KeResFindResource(VIRTUAL_PTR Base,VIRTUAL_PTR *Resource,UINT32 Type,UINT32 Id,UINT32 Lang);
KESTATUS STDCALL KeResFindRCData(VIRTUAL_PTR Base,VIRTUAL_PTR *Resource, UINT32 Id,UINT32 Lang);
KESTATUS STDCALL KeResFindBitmap(VIRTUAL_PTR Base,VIRTUAL_PTR *Resource, UINT32 Id,UINT32 Lang);

#endif // KERNELRESOURCES_H_INCLUDED
///////////////////////////////////////////////////////////////////////////////////
//--------------------------------KernelResources.h------------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
