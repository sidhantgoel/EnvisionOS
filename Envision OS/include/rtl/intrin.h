////////////////////////////////////////////////////////////////////////////////
//----------------------------------intrin.h----------------------------------//
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
#ifndef __intrin_h__
#define __intrin_h__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef _MSC_VER

int __cdecl _inp(unsigned short);
int __cdecl inp(unsigned short);
unsigned long __cdecl _inpd(unsigned short);
unsigned long __cdecl inpd(unsigned short);
unsigned short __cdecl _inpw(unsigned short);
unsigned short __cdecl inpw(unsigned short);
int __cdecl _outp(unsigned short,int);
int __cdecl outp(unsigned short,int);
unsigned long __cdecl _outpd(unsigned short,unsigned long);
unsigned long __cdecl outpd(unsigned short,unsigned long);
unsigned short __cdecl _outpw(unsigned short,unsigned short);
unsigned short __cdecl outpw(unsigned short,unsigned short);
char * __cdecl strcat(char *,const char *);
int __cdecl strcmp(const char *,const char *);
char * __cdecl strcpy(char *,const char *);
size_t __cdecl strlen(const char *);
char * __cdecl strset(char *,int);
void __wbinvd(void);
void __invlpg(void*);
unsigned __int64 __readmsr(unsigned long);
void __writemsr(unsigned long, unsigned __int64);
unsigned __int64 __rdtsc(void);
void __movsb(unsigned char *, unsigned char const *, size_t);
void __movsw(unsigned short *, unsigned short const *, size_t);
void __movsd(unsigned long *, unsigned long const *, size_t);
unsigned char __inbyte(unsigned short Port);
unsigned short __inword(unsigned short Port);
unsigned long __indword(unsigned short Port);
void __outbyte(unsigned short Port, unsigned char Data);
void __outword(unsigned short Port, unsigned short Data);
void __outdword(unsigned short Port, unsigned long Data);
void __inbytestring(unsigned short Port, unsigned char *Buffer, unsigned long Count);
void __inwordstring(unsigned short Port, unsigned short *Buffer, unsigned long Count);
void __indwordstring(unsigned short Port, unsigned long *Buffer, unsigned long Count);
void __outbytestring(unsigned short Port, unsigned char *Buffer, unsigned long Count);
void __outwordstring(unsigned short Port, unsigned short *Buffer, unsigned long Count);
void __outdwordstring(unsigned short Port, unsigned long *Buffer, unsigned long Count);
void _enable();
void _disable();

#endif //_MSC_VER

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //__intrin_h__
////////////////////////////////////////////////////////////////////////////////
//----------------------------------intrin.h----------------------------------//
//-----------------------Copyright © Sidhant Goel, 2008-----------------------//
////////////////////////////////////////////////////////////////////////////////
