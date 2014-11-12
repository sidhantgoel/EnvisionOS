///////////////////////////////////////////////////////////////////////////////////
//---------------------------------KernelStatus.h--------------------------------//
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
#ifndef KERNELSTATUS_H_INCLUDED
#define KERNELSTATUS_H_INCLUDED

typedef SINT32 KESTATUS;

//  Values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-------------------------+-------------------------------+
//  |Sev|C|       Facility          |               Code            |
//  +---+-+-------------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      Facility - is the facility code
//
//      Code - is the facility's status code

#define SEVERITY_SUCCESS        0x0
#define SEVERITY_INFORMATION    0x1
#define SEVERITY_WARNING        0x2
#define SEVERITY_ERROR          0x3

#define FACILITY_KERNEL			0x0
#define FACILITY_ACPI           0x1

#define KESTATUS_SUCCESS(x)     (x >= 0)
#define KESTATUS_INFORMATION(x) (((UINT32)x >> 30) == SEVERITY_INFORMATION)
#define KESTATUS_WARNING(x)     (((UINT32)x >> 30) == SEVERITY_WARNING)
#define KESTATUS_ERROR(x)       (x < 0)

#define STATUS_SUCCESS              0x00000000
#define STATUS_INFORMATION          0x40000000
#define STATUS_FALSE                0x40000000
#define STATUS_TRUE                 0x40000001
#define STATUS_GDTE_FREE            0x40000002
#define STATUS_GDTE_PRESENT         0x40000003
#define STATUS_DETECTED				0x40000004
#define STATUS_WARNING              0x80000000
#define STATUS_ERROR                0xC0000000
#define STATUS_INVALID_PARAM        0xC0000001
#define STATUS_ACCESS_DENIED        0xC0000002
#define STATUS_ACCESS_VOILATION     0xC0000003
#define STATUS_DIVIDE_BY_ZERO       0xC0000004
#define STATUS_FILE_NOT_FOUND       0xC0000005
#define STATUS_INDEX_INVALID        0xC0000006
#define STATUS_GDTE_EXIST           0xC0000007
#define STATUS_GDTE_NOT_PRESENT     0xC0000008
#define STATUS_INVALID_ADDRESS      0xC0000009
#define STATUS_INVALID_COUNT        0xC000000A
#define STATUS_NOTSUPPORTED			0xC000000B
#define STATUS_WRONGDEVICE			0xC000000C
#define STATUS_TIMEOUT				0xC000000D
#define STATUS_NOTDETECTED			0xC000000E
#define STATUS_NOMEDIA				0xC000000F
#define STATUS_UNKNOWN              0xC000FFFF

/*HACK*/
#define STATUS_ATA					0x40000005
#define STATUS_ATAPI				0x40000006
#define STATUS_PRESENT				0x40000007
#define STATUS_NOTPRESENT			0x40000010
#define EFAIL(a)					KESTATUS_ERROR(((KESTATUS)a))
#define EOK(a)						KESTATUS_SUCCESS(((KESTATUS)a))

#define ACPI_SUCCESS(a)                 (a >= 0)
#define ACPI_FAILURE(a)                 (a < 0)

#define ACPI_OK						0x00010000
#define ACPI_ENABLED				0x40010000
#define ACPI_DISABLED				0xC0010000
#define ACPI_RSDP_INVALID           0xC0010001
#define ACPI_ADDR_OUT_OF_RANGE      0xC0010002
#define ACPI_UNKNOWN				0xC001FFFF

#endif // KERNELSTATUS_H_INCLUDED
///////////////////////////////////////////////////////////////////////////////////
//---------------------------------KernelStatus.h--------------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
