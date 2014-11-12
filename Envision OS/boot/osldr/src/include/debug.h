////////////////////////////////////////////////////////////////////////////////
//----------------------------------debug.h-----------------------------------//
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
#ifndef __debug_h__
#define __debug_h__
#include <arch\arch.h>
#include <rs232.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define DEBUG_INFO      0x00
#define DEBUG_WARNING   0x01
#define DEBUG_DEBUG     0x02
#define DEBUG_ERROR     0x03

UINT32 STDCALL DbgInit();
void STDCALL DbgEnable();
void STDCALL DbgDisable();
void STDCALL DbgStepEnable();
void STDCALL DbgStepDisable();
void STDCALL DbgException(UINT32 e, REGS_E *r);
void STDCALL DbgInterrupt(REGS_E *r);

char* STDCALL DbgStatusStr(UINT32 Status);


#ifdef DEBUG_RS232
#define DbgPrintf	SerialPrintf
#define DbgHexDump	SerialHexDump
#else
#define printf(...)
#endif

#define EXCEPT_DE   0x00
#define EXCEPT_DB   0x01
#define EXCEPT_BP   0x02
#define EXCEPT_OF   0x03
#define EXCEPT_BR   0x04
#define EXCEPT_UD   0x05
#define EXCEPT_NM   0x06
#define EXCEPT_DF   0x07
#define EXCEPT_TS   0x08
#define EXCEPT_NP   0x09
#define EXCEPT_SS   0x0A
#define EXCEPT_GP   0x0B
#define EXCEPT_PF   0x0C
#define EXCEPT_MF   0x0D
#define EXCEPT_AC   0x0E
#define EXCEPT_MC   0x0F
#define EXCEPT_XF   0x10

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //__debug_h__