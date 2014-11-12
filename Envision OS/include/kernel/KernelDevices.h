///////////////////////////////////////////////////////////////////////////////////
//---------------------------------KernelDevices.h-------------------------------//
//                                      v 1.0                                    //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
// Created by: Sidhant Goel                                                      //
// Created on: 11 march 2007                                                     //
//                                                                               //
//--------------------------------------Notes------------------------------------//
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
#ifndef KERNELDEVICES_H_INCLUDED
#define KERNELDEVICES_H_INCLUDED
#include "Kernel.h"
#include "KernelDeviceDriver.h"
#include "KernelBusDriver.h"

typedef struct _KDEVICE
{
    DEVICE  *Device;
}KDEVICE;

KESTATUS KeDevicesInit();

//IO Manager Exported Functions
KESTATUS STDCALL IoCreateDriver(DEVICEDRIVER *Driver);
KESTATUS STDCALL IoCreateDevice(DEVICE *Device);

#endif // KERNELDEVICES_H_INCLUDED
///////////////////////////////////////////////////////////////////////////////////
//---------------------------------KernelDevices.h-------------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
