///////////////////////////////////////////////////////////////////////////////////
//-------------------------------KernelDescriptors.h-----------------------------//
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
#ifndef KERNELDESCRIPTORS_H_INCLUDED
#define KERNELDESCRIPTORS_H_INCLUDED

#include "Kernel.h"
#include "KernelInterrupts.h"

#define SEL_RING0_CODE  0x08
#define SEL_RING0_DATA  0x10
#define SEL_RING3_CODE  0x18
#define SEL_RING3_DATA  0x20

KESTATUS STDCALL KeDescriptorsInit();

#endif // KERNELDESCRIPTORS_H_INCLUDED
///////////////////////////////////////////////////////////////////////////////////
//-------------------------------KernelDescriptors.h-----------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
