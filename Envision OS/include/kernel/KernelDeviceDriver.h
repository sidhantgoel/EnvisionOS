///////////////////////////////////////////////////////////////////////////////////
//-------------------------------KernelDeviceDriver.h----------------------------//
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
#ifndef KERNELDEVICEDRIVER_H_INCLUDED
#define KERNELDEVICEDRIVER_H_INCLUDED
#include "Kernel.h"

typedef struct _DRIVER_INFO DRIVER_INFO;
typedef struct _DEVICE_INFO DEVICE_INFO;

typedef KESTATUS (__stdcall *PFN_DEV_IOCTL)(DEVICE_INFO *Device,UINT32 IOCode,VIRTUAL_PTR Buffer);
typedef KESTATUS (__stdcall *PFN_DEV_READ_FILE)(DEVICE_INFO *Device,VIRTUAL_PTR Buffer);
typedef KESTATUS (__stdcall *PFN_DEV_WRITE_FILE)(DEVICE_INFO *Device,VIRTUAL_PTR Buffer);

typedef struct _DEVICEDRIVER
{
    PFN_DEV_READ_FILE   ReadFile;
    PFN_DEV_WRITE_FILE  WriteFile;
    PFN_DEV_IOCTL       DeviceIOControl;
}DEVICEDRIVER;

typedef struct _DEVICE
{
    UINT32          StructSize;
    UINT32          Type;
    DEVICEDRIVER    *Driver;
}DEVICE;

#define DEVICE_TYPE_UNKNOWN     0x00
#define DEVICE_TYPE_BUS         0x01
#define DEVICE_TYPE_DISK        0x02
#define DEVICE_TYPE_VGA         0x03

#endif // KERNELDEVICEDRIVER_H_INCLUDED
///////////////////////////////////////////////////////////////////////////////////
//-------------------------------KernelDeviceDriver.h----------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
