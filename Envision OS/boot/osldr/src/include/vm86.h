#ifndef __vm86_h__
#define __vm86_h__

#include <arch\arch.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define to_linear(seg,off)      ((seg & 0xFFFF) * 16L + off)
#define peekb(seg,off)          (*(BYTE*)to_linear(seg,off))
#define peekw(seg,off)          (*(UINT16*)to_linear(seg,off))
#define peekl(seg,off)          (*(UINT32*)to_linear(seg,off))
#define pokeb(seg,off,val)      (*(BYTE*)to_linear(seg,off)=val)
#define pokew(seg,off,val)      (*(UINT16*)to_linear(seg,off)=val)
#define pokel(seg,off,val)      (*(UINT32*)to_linear(seg,off)=val)

typedef struct _VM86_ARGS
{
	UINT32 Eip;
	UINT32 EndAddr;
	UINT32 Eax, Ecx, Edx, Ebx, Esp, Ebp, Esi, Edi;
	UINT32 Cs, Ss, Ds, Es, Fs, Gs;
	UINT32 Eflags;
	UINT32 ErrorCode;
}VM86_ARGS;

void Vm86Call(VM86_ARGS *args);
void Vm86CallInt(VM86_ARGS *args, BYTE IntNum);
void Vm86Init();

typedef struct
{
    TSS tss;
    BYTE iomap[8192];
}VMTSS;

extern VMTSS VM86_TSS;

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //__vm86_h__