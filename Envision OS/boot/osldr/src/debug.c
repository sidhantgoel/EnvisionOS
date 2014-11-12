////////////////////////////////////////////////////////////////////////////////
//----------------------------------debug.c-----------------------------------//
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
#include <status.h>
#include <debug.h>
#include <rs232.h>
#include <console.h>
#include <descriptors.h>

char MSG_DE[]  =  "Divide Error Exception";
char MSG_DB[]  =  "Debug Exception";
char MSG_BP[]  =  "Breakpoint Exception";
char MSG_OF[]  =  "Overflow Exception";
char MSG_BR[]  =  "BOUND Range Exceed Exception";
char MSG_UD[]  =  "Invalid Opcode Exception";
char MSG_NM[]  =  "Device Not Available Exception";
char MSG_DF[]  =  "Double Fault Exception";
char MSG_TS[]  =  "Invalid TSS Exception";
char MSG_NP[]  =  "Segment Not Present";
char MSG_SS[]  =  "Stack Fault Exception";
char MSG_GP[]  =  "General Protection Exception";
char MSG_PF[]  =  "Page Fault Exception";
char MSG_MF[]  =  "FPU Floating Point Error";
char MSG_AC[]  =  "Alignment Check Exception";
char MSG_MC[]  =  "Machine Check Exception";
char MSG_XF[]  =  "SIMD Floating Point Exception";

UINT32 IsDebugged = 0;
UINT32 IsStepped = 0;

void __declspec(naked) DebugTrap()
{
    __asm
    {
        jmp $
		iretd
    }
}

UINT32 STDCALL DbgInit()
{
    IsDebugged = 0;
    IsStepped = 0;
	SetIDTIntGate(0xcc,SEL_RING0_CODE,(UINT32)DebugTrap);
    return 0;
}

void STDCALL DbgEnable()
{
    IsDebugged = 1;
}

void STDCALL DbgDisable()
{
    IsDebugged = 0;
}

void STDCALL DbgStepEnable()
{
    IsStepped = 1;
    __asm
    {
        pushf
        pop eax
        or eax, EFLAGS_TF
        push eax
        popf
    }
}

void STDCALL DbgStepDisable()
{
    IsStepped = 0;
    __asm
    {
        pushf
        pop eax
        and eax, ~EFLAGS_TF
        push eax
        popf
    }
}

#define	VGA_AC_INDEX		0x3C0
#define	VGA_AC_WRITE		0x3C0
#define	VGA_AC_READ		    0x3C1
#define	VGA_MISC_WRITE		0x3C2
#define VGA_SEQ_INDEX		0x3C4
#define VGA_SEQ_DATA		0x3C5
#define	VGA_DAC_READ_INDEX	0x3C7
#define	VGA_DAC_WRITE_INDEX	0x3C8
#define	VGA_DAC_DATA		0x3C9
#define	VGA_MISC_READ		0x3CC
#define VGA_GC_INDEX 		0x3CE
#define VGA_GC_DATA 		0x3CF
/*			COLOR emulation		MONO emulation */
#define VGA_CRTC_INDEX		0x3D4		/* 0x3B4 */
#define VGA_CRTC_DATA		0x3D5		/* 0x3B5 */
#define	VGA_INSTAT_READ		0x3DA

#define	VGA_NUM_SEQ_REGS	5
#define	VGA_NUM_CRTC_REGS	25
#define	VGA_NUM_GC_REGS		9
#define	VGA_NUM_AC_REGS		21
#define	VGA_NUM_REGS		(1 + VGA_NUM_SEQ_REGS + VGA_NUM_CRTC_REGS + VGA_NUM_GC_REGS + VGA_NUM_AC_REGS)


unsigned char mode00h[62] = {
// MISC reg,  STATUS reg,    SEQ regs
   0x67,      0x00,          0x03,0x08,0x03,0x00,0x02,
// CRTC regs
0x2D,0x27,0x28,0x90,0x2B,0xA0,0xBF,0x1F,0x00,0x4F,0x06,0x07,0x00,0x00,0x00,
0x31,0x9C,0x8E,0x8F,0x14,0x1F,0x96,0xB9,0xA3,0xFF,
// GRAPHICS regs
0x00,0x00,0x00,0x00,0x00,0x10,0x0E,0x00,0xFF,
// ATTRIBUTE CONTROLLER regs
0x00,0x01,0x02,0x03,0x04,0x05,0x14,0x07,0x10,0x11,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
0x0C,0x00,0x0F,0x08,0x00
};

UCHAR mode03h[62] = 
{
    /* MISC */
    0x67,
    /* STATUS */
    0x00,
    /* SEQ */
    0x03, 0x00, 0x03, 0x00, 0x02,
    /* CRTC */
    0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0xBF, 0x1F,
    0x00, 0x4F, 0x0E, 0x0F, 0x00, 0x00, 0x00, 0x00,
    0x9C, 0x0E, 0x8F, 0x28, 0x01, 0x96, 0xB9, 0xA3,
    0xFF,
    /* GC */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
    0xFF,
    /* AC */
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
    0x10, 0x11, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
    0x0C, 0x00, 0x0F, 0x08, 0x00
};

unsigned char mode04h[62] = {
// MISC reg,  STATUS reg,    SEQ regs
   0x63,      0x00,          0x03,0x09,0x03,0x00,0x02,
// CRTC regs
0x2D,0x27,0x28,0x90,0x2B,0x80,0x0BF,0x1F,0x00,0x0C1,0x00,0x00,0x00,0x00,0x00,
0x31,0x9C,0x8E,0x8F,0x14,0x00,0x96,0xB9,0x0A2,0xFF,
// GRAPHICS regs
0x00,0x00,0x00,0x00,0x00,0x30,0x0F,0x00,0xFF,
// ATTRIBUTE CONTROLLER regs
0x00,0x3B,0x3D,0x3F,0x02,0x04,0x06,0x07,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,
0x01,0x00,0x03,0x00,0x00
};

unsigned char modeJ[62] = {
// MISC reg,  STATUS reg,    SEQ regs
   0xa3,      0x00,          0x03,0x01,0x03,0x00,0x02,
// CRTC regs
0x5F,0x4F,0x50,0x82,0x55,0x81,0xBF,0x1F,0x00,0x47,0x0E,0x0F,0x00,0x00,0x00,
0x00,0x83,0x85,0x57,0x28,0x1F,0x60,0xB8,0xA3,0xFF,
// GRAPHICS regs
0x00,0x00,0x00,0x00,0x00,0x10,0x0E,0x00,0xFF,
// ATTRIBUTE CONTROLLER regs
0x00,0x01,0x02,0x03,0x04,0x05,0x14,0x07,0x10,0x11,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
0x0C,0x00,0x0F,0x00,0x00
};

void STDCALL DbgSetMode(UCHAR *regs)
{
    UINT32 i;

    _outp(VGA_MISC_WRITE, *regs);           //write MISCELLANEOUS reg
    regs++;

    _outp(VGA_INSTAT_READ, *regs);          //write STATUS reg
    regs++;

    for (i = 0; i < VGA_NUM_SEQ_REGS; i++)  //write SEQUENCER regs
    {
        _outp(VGA_SEQ_INDEX, i);
        _outp(VGA_SEQ_DATA, *regs);
        regs++;
    }

    _outp(VGA_CRTC_INDEX, 0x03);            //unlock CRTC registers
    _outp(VGA_CRTC_DATA, _inp(VGA_CRTC_DATA) | 0x80);
    _outp(VGA_CRTC_INDEX, 0x11);
    _outp(VGA_CRTC_DATA, _inp(VGA_CRTC_DATA) & ~0x80);

    regs[0x03] |= 0x80;                     //make sure they remain unlocked
    regs[0x11] &= ~0x80;

    for (i = 0; i < VGA_NUM_CRTC_REGS; i++) //write CRTC regs
    {
        _outp(VGA_CRTC_INDEX, i);
        _outp(VGA_CRTC_DATA, *regs);
        regs++;
    }

    for (i = 0; i < VGA_NUM_GC_REGS; i++)   //write GRAPHICS CONTROLLER regs
    {
        _outp(VGA_GC_INDEX, i);
        _outp(VGA_GC_DATA, *regs);
        regs++;
    }

    for (i = 0; i < VGA_NUM_AC_REGS; i++)   //write ATTRIBUTE CONTROLLER regs
    {
        (void)_inp(VGA_INSTAT_READ);
        _outp(VGA_AC_INDEX, i);
        _outp(VGA_AC_WRITE, *regs);
        regs++;
    }

    (void)_inp(VGA_INSTAT_READ);            //lock 16-color palette and unblank display
    _outp(VGA_AC_INDEX, 0x20);
}

void STDCALL DbgDeathScr(UINT32 e, REGS_E *r)
{
    /*UINT32 i;
    UINT16 *Buffer = (USHORT*)0xB8000;

    //Switch to text mode
    DbgSetMode(mode03h);
    //ClearScreen
    for (i = 0;i<80*25;i++) Buffer[i] = BGCOLOR_BLUE|FGCOLOR_BRIGHTWHITE;
    //DbgPrintf("sone");
    DbgPrintf("CS:EIP = %X:%X", r->cs, r->ebp);*/
    while(1);
}



void STDCALL DbgException(UINT32 e, REGS_E *r)
{
	int i;
    switch(e)
    {
    case EXCEPT_DE:
        DbgPrintf("%s\n", MSG_DE);
        break;
    case EXCEPT_DB:
        if(IsStepped)
            DbgPrintf("%s %X\n", MSG_DB, r->eip);
        else
            DbgPrintf("%s\n", MSG_DB);
        return;
        break;
    case EXCEPT_BP:
        DbgPrintf("%s\n", MSG_BP);
        break;
    case EXCEPT_OF:
        DbgPrintf("%s\n", MSG_OF);
        break;
    case EXCEPT_BR:
		DbgPrintf("%s %X\n", MSG_BR, r->eip);
        break;
    case EXCEPT_UD:
        DbgPrintf("%s %X\n", MSG_UD, r->eip);
        //DbgPrintf("%s\n", MSG_UD);
        break;
    case EXCEPT_NM:
        DbgPrintf("%s\n", MSG_NM);
        break;
    case EXCEPT_DF:
        DbgPrintf("%s\n", MSG_DF);
        break;
    case EXCEPT_TS:
        DbgPrintf("%s\n", MSG_TS);
        break;
    case EXCEPT_NP:
        DbgPrintf("%s\n", MSG_NP);
        break;
    case EXCEPT_SS:
        DbgPrintf("%s\n", MSG_SS);
        break;
    case EXCEPT_GP:
        DbgPrintf("%s %X %X\n", MSG_GP, r->err_code, r->eip);
        break;
    case EXCEPT_PF:
		__asm mov eax, cr2; mov i, eax
			DbgPrintf("%s %X %X\n", MSG_PF, r->err_code, r->eip);
        break;
    case EXCEPT_MF:
        DbgPrintf("%s\n", MSG_MF);
        break;
    case EXCEPT_AC:
        DbgPrintf("%s\n", MSG_AC);
        break;
    case EXCEPT_MC:
        DbgPrintf("%s\n", MSG_MC);
        break;
    case EXCEPT_XF:
        DbgPrintf("%s\n", MSG_XF);
        break;
    }
    DbgDeathScr(e, r);
    while(1);
}

void STDCALL DbgInterrupt(REGS_E *r)
{
    DbgPrintf("Unhandled Interrupt");
    while(1);
}

char* STDCALL DbgStatusStr(UINT32 Status)
{
    char *Msg;
    switch(Status)
    {
    case ESUCCESS:
        Msg = "ESUCCESS"; 
        break;
    case EDETECTED:
        Msg = "EDETECTED"; 
        break;
    case EATA:
        Msg = "EATA"; 
        break;
    case EATAPI:
        Msg = "EATAPI"; 
        break;
    case EPRESENT:
        Msg = "EPRESENT"; 
        break;
    case ENOTPRESENT:
        Msg = "ENOTPRESENT"; 
        break;
    case ETIMEOUT:
        Msg = "ETIMEOUT";
        break;
    case EDRQTIMEOUT:
        Msg = "EDRQTIMEOUT";
        break;
    case EWRONGDEVICE:
        Msg = "EWRONGDEVICE";
        break;
    case EWRONGPARAMETER:
        Msg = "EWRONGPARAMETER";
        break;
    case ENOTDETECTED:
        Msg = "ENOTDETECTED";
        break;
    case EFAILED:
        Msg = "EFAILED";
        break;
    case ECMDERROR:
        Msg = "ECMDERROR";
        break;
    case EFILENOTFOUND:
        Msg = "EFILENOTFOUND";
        break;
    case EDIRNOTFOUND:
        Msg = "EDIRNOTFOUND";
        break;
    case EINVALIDPATH:
        Msg = "EINVALIDPATH";
        break;
    case EDUPLICATEPATH:
        Msg = "EDUPLICATEPATH";
        break;
    case ENOTSUPPORTED:
        Msg = "ENOTSUPPORTED";
        break;
    case EABORTED:
        Msg = "EABORTED";
        break;
    case ENOMEDIA:
        Msg = "ENOMEDIA";
        break;
    case EUNKNOWN:
    default:
        Msg = "EUNKNOWN";
        break;
    }
    return Msg;
}


////////////////////////////////////////////////////////////////////////////////
//----------------------------------debug.c-----------------------------------//
//-----------------------Copyright © Sidhant Goel, 2008-----------------------//
////////////////////////////////////////////////////////////////////////////////
