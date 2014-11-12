#ifndef CONIO_H_INCLUDED
#define CONIO_H_INCLUDED

int __cdecl _inp(unsigned short);
unsigned short __cdecl _inpw(unsigned short);
unsigned long __cdecl _inpd(unsigned short);

int __cdecl _outp(unsigned short, int);
unsigned short __cdecl _outpw(unsigned short, unsigned short);
unsigned long __cdecl _outpd(unsigned short, unsigned long);

#endif // CONIO_H_INCLUDED
