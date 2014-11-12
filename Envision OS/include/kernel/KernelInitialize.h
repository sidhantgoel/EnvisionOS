///////////////////////////////////////////////////////////////////////////////////
//-------------------------------KernelInitialize.h------------------------------//
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
#ifndef KERNELINITIALIZE_H_INCLUDED
#define KERNELINITIALIZE_H_INCLUDED

#include <Kernel.h>
#include <KernelDebug.h>
#include <KernelACPI.h>
#include <KernelCpu.h>
#include <KernelAPIC.h>
#include <KernelDescriptors.h>
#include <KernelInterrupts.h>
#include <KernelExceptions.h>
#include <KernelIRQ.h>
#include <KernelMemoryManager.h>
#include <KernelNamespace.h>
#include <KernelObjectManager.h>
#include <KernelTasks.h>
#include <KernelSyscalls.h>
#include <KernelBootVideo.h>
#include <KernelDevices.h>

KESTATUS STDCALL KeInitialize();

#endif // KERNELINITIALIZE_H_INCLUDED
///////////////////////////////////////////////////////////////////////////////////
//-------------------------------KernelInitialize.h------------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
