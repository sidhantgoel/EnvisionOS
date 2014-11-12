////////////////////////////////////////////////////////////////////////////////
//----------------------------------rs232.h-----------------------------------//
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
#ifndef __rs232_h__
#define __rs232_h__
#include <arch\arch.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

UINT32 STDCALL SerialInit();

UINT32 STDCALL SerialWriteBin(VPTR addr, SIZE_T size);
UINT32 STDCALL SerialWriteChar(CHAR ch);
UINT32 STDCALL SerialWriteString(CHAR *String);
UINT32 STDCALL SerialWriteHexByte(BYTE Byte);
UINT32 STDCALL SerialWriteHexWord(USHORT Word);
UINT32 STDCALL SerialWriteHexDword(UINT32 Dword);
UINT32 STDCALL SerialWriteHexQword(UINT64 Qword);
UINT32 STDCALL SerialHexDump(VPTR Addr,SIZE_T Size);
UINT32 STDCALL SerialSendData(BYTE *String, SIZE_T size);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //__rs232_h__
////////////////////////////////////////////////////////////////////////////////
//----------------------------------rs232.h-----------------------------------//
//-----------------------Copyright © Sidhant Goel, 2008-----------------------//
////////////////////////////////////////////////////////////////////////////////
