///////////////////////////////////////////////////////////////////////////////////
//-------------------------------KernelLoaderInfo.h------------------------------//
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
#ifndef KERNELLOADERINFO_H_INCLUDED
#define KERNELLOADERINFO_H_INCLUDED

#include <Kernel.h>

#pragma pack(push,4)
typedef struct _KEMEM_MAP
{
    UINT64 Start;
    UINT64 Size;
    UINT32 Type;
	UINT32 p;
}KEMEM_MAP;

#define MEM_RANGE_MEMORY        0x01
#define MEM_RANGE_RESERVED      0x02
#define MEM_RANGE_ACPI          0x03
#define MEM_RANGE_NVS           0x04

typedef struct _KEMODULE_INFO
{
    PHYSICAL_PTR Start;
    UINT32 Size;
}KEMODULE_INFO;

typedef struct _KEVIDEO_MODE
{
    PHYSICAL_PTR FrameBuffer;
    UINT32 FrameBufferSize;
    UINT32 ScanLineSize;
    UINT32 ModeWidth;
    UINT32 ModeHeight;
    UINT32 BitsPerPixel;
	UINT32 RedMaskSize;
	UINT32 GreenMaskSize;
	UINT32 BlueMaskSize;
	UINT32 RedFieldPosition;
	UINT32 GreenFieldPosition;
	UINT32 BlueFieldPosition;
}KEVIDEO_MODE;

typedef struct _KELOADER_INFO
{
    PHYSICAL_PTR    KernelAddress;
    UINT32          KernelSize;
    PHYSICAL_PTR    KernelStack;
    UINT32          KernelStackSize;
    VIRTUAL_PTR     KernelStackTop;
    PHYSICAL_PTR    KernelPageData;
    UINT32          KernelPageDataSize;
    UINT32          MemoryMapCount;
    UINT32          ModulesCount;
    KEMEM_MAP       *MemoryMaps;
    KEMODULE_INFO   *Modules;
    KEVIDEO_MODE    VideoMode;
}KELOADER_INFO;
#pragma pack(pop)

#endif // KERNELLOADERINFO_H_INCLUDED
///////////////////////////////////////////////////////////////////////////////////
//-------------------------------KernelLoaderInfo.h------------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
