///////////////////////////////////////////////////////////////////////////////////
//------------------------------KernelVolumeManager.h----------------------------//
//                                      v 1.0                                    //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
// Created by: Sidhant Goel                                                      //
// Created on: 12 march 2007                                                     //
//                                                                               //
//--------------------------------------Notes------------------------------------//
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
#ifndef KERNELVOLUMEMANAGER_H_INCLUDED
#define KERNELVOLUMEMANAGER_H_INCLUDED
#include "Kernel.h"
#include "KernelDevices.h"

typedef struct _KVOLUME
{
    KDEVICE     *Disk;
    UINT32      Start;
    UINT32      End;
}KVOLUME;

KESTATUS STDCALL KeVolumeManagerInit();

#endif // KERNELVOLUMEMANAGER_H_INCLUDED
///////////////////////////////////////////////////////////////////////////////////
//------------------------------KernelVolumeManager.h----------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
