///////////////////////////////////////////////////////////////////////////////////
//-------------------------------KernelImageManager.c----------------------------//
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
#include <KernelImage.h>

KESTATUS STDCALL KeImageGetSize(SIZE_T *size,VIRTUAL_PTR image)
{
    IMAGE_DOS_HEADER *dos_hdr = (IMAGE_DOS_HEADER*)image;
    IMAGE_HEADERS32 *img_hdr = (IMAGE_HEADERS32*)(image + dos_hdr->e_lfanew);
    *size = img_hdr->OptionalHeader.SizeOfImage;
    return STATUS_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////
//-------------------------------KernelImageManager.c----------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
