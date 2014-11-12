////////////////////////////////////////////////////////////////////////////////
//-----------------------------------arch.h-----------------------------------//
//                                   v 1.0                                    //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
// Author: Sidhant Goel                                                       //
// Date Created: 08 July 2008                                                 //
//                                                                            //
//----------------------------------Notes-------------------------------------//
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
#ifndef __archdef_h__
#define __archdef_h__

#ifdef _MSC_VER

#include <intrin.h>

#define outb(prt, val)          _outp(prt, val)
#define inb(prt)                ((unsigned char)_inp(prt))
#define outw(prt, val)          _outpw(prt, val)
#define inw(prt)                ((unsigned short)_inpw(prt))
#define getRetAddr              _ReturnAddress
#define outbs					__outbytestring
#define outws					__outwordstring
#define inbs					__inbytestring
#define inws					__inwordstring

typedef unsigned long ULONG;
typedef char CHAR;
typedef short SHORT;
typedef int INT;
typedef _int32 INT32;
typedef _int64 INT64;
typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned int UINT;
typedef unsigned _int32 UINT32;
typedef unsigned _int64 UINT64;
typedef signed int SINT;
typedef signed _int32 SINT32;
typedef signed _int64 SINT64;
typedef signed short SSHORT;
typedef unsigned char BYTE;
typedef USHORT WORD;
typedef UINT32 DWORD;
typedef unsigned long LONG;
typedef USHORT WCHAR;
typedef unsigned char UINT8;
typedef signed char SINT8;
typedef char INT8;
typedef unsigned short UINT16;
typedef signed short SINT16;
typedef short INT16;
typedef UINT32 NATIVE_UINT;
typedef INT32 NATIVE_INT;
typedef UINT32 BOOLEAN;
typedef UINT32 BOOL;
typedef UINT64 PPTR;
typedef BYTE* VPTR;
typedef UINT32 SIZE_T;
typedef void* HANDLE;

#define true	1
#define false	0

//#define LODWORD(x)     (*((UINT32*)&x))
//#define HIDWORD(x)     (*(((UINT32*)&x)+1))

typedef struct _QWORD_STRUCT
{
    UINT32 LoDword;
    UINT32 HiDword;
}QWORD_STRUCT;

#define HIDWORD(l)                  ((UINT32)((QWORD_STRUCT*)(&l))->HiDword)
#define LODWORD(l)                  ((UINT32)(l))
#define LOWORD(l)                   ((UINT16)(UINT32)(l))
#define HIWORD(l)                   ((UINT16)((((UINT32)(l)) >> 16) & 0xFFFF))
#define LOBYTE(l)                   ((UINT8)(UINT16)(l))
#define HIBYTE(l)                   ((UINT8)((((UINT16)(l)) >> 8) & 0xFF))

#define STDCALL     __stdcall

#define TRUE    1
#define FALSE   0

#endif //_MSC_VER

#endif //__archdef_h__
////////////////////////////////////////////////////////////////////////////////
//-----------------------------------arch.h-----------------------------------//
//-----------------------Copyright © Sidhant Goel, 2008-----------------------//
////////////////////////////////////////////////////////////////////////////////
