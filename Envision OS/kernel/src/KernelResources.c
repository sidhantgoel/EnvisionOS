///////////////////////////////////////////////////////////////////////////////////
//--------------------------------KernelResources.c------------------------------//
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
#include "KernelResources.h"

KESTATUS STDCALL KeResFind(IMAGE_RESOURCE_DIRECTORY *pRoot,IMAGE_RESOURCE_DIRECTORY *Dir,VIRTUAL_PTR *Data,UINT32 *IsDir,UINT32 Id)
{
    UINT32 Count;
    UINT32 i;
    VIRTUAL_PTR DirVA;
    Count = Dir->NumberOfNamedEntries + Dir->NumberOfIdEntries;
    for(i=0;i<Count;i++)
    {
        if(Dir->DirectoryEntries[i].Name & IMAGE_RESOURCE_NAME_IS_STRING)
        {
        }
        else
        {
            if(Dir->DirectoryEntries[i].Id==Id)
            {
                if(Dir->DirectoryEntries[i].Offset & IMAGE_RESOURCE_DATA_IS_DIR)
                {
                    DirVA = (VIRTUAL_PTR)(Dir->DirectoryEntries[i].Offset & 0x4FFFFFFF);
                    *Data = (VIRTUAL_PTR)pRoot + (SIZE_T)DirVA;
                    *IsDir = TRUE;
                    return STATUS_SUCCESS;
                }
                else
                {
                    DirVA = (VIRTUAL_PTR) Dir->DirectoryEntries[i].Offset;
                    *Data = (VIRTUAL_PTR)pRoot + (SIZE_T)DirVA;
                    *IsDir = FALSE;
                    return STATUS_SUCCESS;
                }
            }
        }
    }
    return STATUS_ERROR;
}

KESTATUS STDCALL KeResToBitmap(VIRTUAL_PTR **Bitmap,VIRTUAL_PTR Resource)
{
    BITMAPFILEHEADER *FileHeader;
    BITMAPINFOHEADER *InfoHeader;
    BYTE *Data;
    UINT32 Size=0;
    UINT32 Scanline;
    InfoHeader = (BITMAPINFOHEADER*)Resource;

    Size += InfoHeader->biSize;

    if(InfoHeader->biBitCount==4)
    {
        Size += 16 * sizeof(RGBQUAD);
        Scanline = InfoHeader->biWidth/2;
        if(Scanline & 0x3) Scanline = (Scanline|0x3)+1;
        Size += Scanline * InfoHeader->biHeight;

        KeMemAlloc(&Data,Size);
        memset(Data,0,Size);

        FileHeader = (BITMAPFILEHEADER*)Data;
        FileHeader->bfType = BF_TYPE;
        FileHeader->bfSize = Size;
        FileHeader->bfReserved1 = 0;
        FileHeader->bfReserved2 = 0;
        FileHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + InfoHeader->biSize + (16 * sizeof(RGBQUAD));

        memcpy(FileHeader+1,InfoHeader,Size-sizeof(BITMAPFILEHEADER));

        KeDebugPrintf("\nSize : %X",Size);
        _hang();

        return STATUS_SUCCESS;
    }
    _hang();
    return STATUS_ERROR;
}

KESTATUS STDCALL KeResFindResource(VIRTUAL_PTR Base,VIRTUAL_PTR *Resource,UINT32 Type,UINT32 Id,UINT32 Lang)
{
    UINT32 IsDir;
    IMAGE_RESOURCE_DIRECTORY *pRoot;
    IMAGE_RESOURCE_DIRECTORY *TypeDir;
    IMAGE_RESOURCE_DIRECTORY *ResDir;
    IMAGE_RESOURCE_DATA_ENTRY *ResData;
    VIRTUAL_PTR Addr;

    if(!KESTATUS_SUCCESS(KeResDirectory(Base,&pRoot)))
    {
        KeDebugPrintf("\nRoot Directory Not Found");
        return STATUS_ERROR;
    }

    if(!KESTATUS_SUCCESS(KeResFind(pRoot, pRoot,(VIRTUAL_PTR*)&TypeDir,&IsDir,Type)))
    {
        KeDebugPrintf("\nFailed to Find Bitmap Dir");
        return STATUS_ERROR;
    }
    if(IsDir)
    {
        if(!KESTATUS_SUCCESS(KeResFind(pRoot, TypeDir,(VIRTUAL_PTR*)&ResDir,&IsDir,Id)))
        {
            KeDebugPrintf("\nFailed to Find ID Dir");
            return STATUS_ERROR;
        }
        if(IsDir)
        {
            if(!KESTATUS_SUCCESS(KeResFind(pRoot, ResDir,(VIRTUAL_PTR*)&ResData,&IsDir,Lang)))
            {
                KeDebugPrintf("\nFailed to Find Lang");
                return STATUS_ERROR;
            }
            if(!IsDir)
            {
                KeDebugPrintf("\nResource Found");
                Addr = (VIRTUAL_PTR)Base + ResData->OffsetToData;
                *Resource = Addr;
                //KeDebugPrints(Addr);
                return STATUS_SUCCESS;
            }
            KeDebugPrintf("\nDIR");
            return STATUS_ERROR;
        }
        KeDebugPrintf("\nNot A Dir");
        return STATUS_ERROR;
    }
    KeDebugPrintf("\nNot A Dir");
    return STATUS_ERROR;
}

KESTATUS STDCALL KeResFindBitmap(VIRTUAL_PTR Base,VIRTUAL_PTR *Resource, UINT32 Id,UINT32 Lang)
{
    return KeResFindResource(Base,Resource,RT_BITMAP,Id,Lang);
}

KESTATUS STDCALL KeResFindRCData(VIRTUAL_PTR Base,VIRTUAL_PTR *Resource, UINT32 Id,UINT32 Lang)
{
    return KeResFindResource(Base,Resource,RT_RCDATA,Id,Lang);
}

KESTATUS STDCALL KeResDirList(IMAGE_RESOURCE_DIRECTORY *pRoot,IMAGE_RESOURCE_DIRECTORY *Dir)
{
    UINT32 Count;
    UINT32 i;
    VIRTUAL_PTR DirVA;
    Count = Dir->NumberOfNamedEntries + Dir->NumberOfIdEntries;
    KeDebugPrintf("\nListing Directory");
    KeDebugPrintf("\nTotal Entries : %X",Count);
    KeDebugPrintf(" Named Entries : %X",Dir->NumberOfNamedEntries);
    KeDebugPrintf(" ID Entries : %X",Dir->NumberOfIdEntries);
    for(i=0;i<Count;i++)
    {
        KeDebugPrintf("\nName : %X",Dir->DirectoryEntries[i].Name);
        KeDebugPrintf(" Offset : %p",Dir->DirectoryEntries[i].Offset);
        if(Dir->DirectoryEntries[i].Offset & IMAGE_RESOURCE_DATA_IS_DIR)
        {
            KeDebugPrintf(" Type : Subdirectory");
            DirVA = (VIRTUAL_PTR)(Dir->DirectoryEntries[i].Offset & 0x4FFFFFFF);
            KeResDirList(pRoot,(IMAGE_RESOURCE_DIRECTORY*)((VIRTUAL_PTR)pRoot + (SIZE_T)DirVA));
            //return KESTATUS_SUCCESS;
        }
    }
    KeDebugPrintf("\nDone Listing Directory");

    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeResDirectory(VIRTUAL_PTR Base,IMAGE_RESOURCE_DIRECTORY **Dir)
{
    VIRTUAL_PTR DirVA;
    IMAGE_DOS_HEADER *dos_hdr = (IMAGE_DOS_HEADER*)Base;
    IMAGE_HEADERS32 *img_hdr = (IMAGE_HEADERS32*)(Base + dos_hdr->e_lfanew);
    DirVA = (VIRTUAL_PTR)img_hdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress;
    if(DirVA)
    {
        *Dir = (IMAGE_RESOURCE_DIRECTORY*)(Base + (SIZE_T)DirVA);
        return STATUS_SUCCESS;
    }
    KeDebugPrintf("\nDirectory Not Found");
    _hang();
    return STATUS_ERROR;
}

///////////////////////////////////////////////////////////////////////////////////
//--------------------------------KernelResources.c------------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
