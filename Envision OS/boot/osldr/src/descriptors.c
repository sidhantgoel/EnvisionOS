#include <descriptors.h>
#include <vm86.h>

#define MAX_GDTE    0x100

IDT_ENTRY IDT[256];
DESC_PTR IDT_PTR = {sizeof(IDT)-1,IDT};


GDT_ENTRY GDT[MAX_GDTE];
DESC_PTR GDT_PTR = {sizeof(GDT)-1,GDT};

struct
{
    TSS tss;
    BYTE iomap[8192];
}KERNEL_TSS;

UINT32 pic0mask, pic1mask;

BOOLEAN STDCALL DescriptorsInit()
{
    UINT32 i=0;

    GDT[i].low = 0;
    GDT[i].high = 0;
    i++;
    GDT[i].low = MAKE_DESC_LOW(0,0xFFFFF);
    GDT[i].high = MAKE_DESC_HIGH(0,0xFFFFF,DESC_G_4K,DESC_DB_32,0,0,1,DESC_DPL0,DESC_S_CODE_DATA,SEG_CODE);
    i++;
    GDT[i].low = MAKE_DESC_LOW(0,0xFFFFF);
    GDT[i].high = MAKE_DESC_HIGH(0,0xFFFFF,DESC_G_4K,DESC_DB_32,0,0,1,DESC_DPL0,DESC_S_CODE_DATA,SEG_DATA_WRITE);
    i++;
    GDT[i].low = MAKE_DESC_LOW(0,0xFFFFF);
    GDT[i].high = MAKE_DESC_HIGH(0,0xFFFFF,DESC_G_4K,DESC_DB_32,0,0,1,DESC_DPL3,DESC_S_CODE_DATA,SEG_CODE);
    i++;
    GDT[i].low = MAKE_DESC_LOW(0,0xFFFFF);
    GDT[i].high = MAKE_DESC_HIGH(0,0xFFFFF,DESC_G_4K,DESC_DB_32,0,0,1,DESC_DPL3,DESC_S_CODE_DATA,SEG_DATA_WRITE);
    i++;

    GDT[i].low = MAKE_TSS_DESC_LOW(&KERNEL_TSS,sizeof(KERNEL_TSS));
    GDT[i].high = MAKE_TSS_DESC_HIGH(&KERNEL_TSS,sizeof(KERNEL_TSS),DESC_G_B,0,1,DESC_DPL0,0);
    i++;
    GDT[i].low = MAKE_TSS_DESC_LOW(&VM86_TSS,sizeof(VM86_TSS));
    GDT[i].high = MAKE_TSS_DESC_HIGH(&VM86_TSS,sizeof(VM86_TSS),DESC_G_B,0,1,DESC_DPL0,0);
    i++;
	GDT[i].low = MAKE_DESC_LOW(0,0xFFFF);
    GDT[i].high = MAKE_DESC_HIGH(0,0xFFFF,DESC_G_B,DESC_DB_16,0,0,1,DESC_DPL0,DESC_S_CODE_DATA,SEG_DATA_WRITE);
    i++;

    for(;i<MAX_GDTE;i++)
    {
        GDT[i].low = 0;
        GDT[i].high = 0;
    }

    __asm lgdt [GDT_PTR]

	__asm
	{
		mov ax,SEL_RING0_DATA
		mov ds,ax
		mov es,ax
		mov fs,ax
		mov gs,ax
		mov ss,ax
	}

    return TRUE;
}

void TssInit()
{
    memset(&KERNEL_TSS,0,sizeof(KERNEL_TSS));
    KERNEL_TSS.tss.ss0 = SEL_RING0_DATA;
    KERNEL_TSS.tss.bitmap = (WORD)sizeof(TSS);

    __asm
    {
        mov ax, SEL_KERNEL_TSS
        ltr ax
    }
}

BOOLEAN STDCALL InterruptsInit()
{
    UINT32 i;
    for(i=0;i<256;i++)
    {
        IDT[i].low = MAKE_INTGATE_LOW(SEL_RING0_CODE,UnhandledInterrupt);
        IDT[i].high = MAKE_INTGATE_HIGH(UnhandledInterrupt,1,DESC_DPL0,1);
        //IDT[i].low = MAKE_INTGATE_LOW(0,0);
        //IDT[i].high = MAKE_INTGATE_HIGH(0,0,0,0);
    }

    __asm lidt [IDT_PTR]

    return TRUE;
}

BOOLEAN STDCALL SetIDTIntGate(UINT32 index,SELECTOR sel,UINT32 offset)
{
    if(index>=256)
    {
        return FALSE;
    }
    IDT[index].low = MAKE_INTGATE_LOW(sel,offset);
    IDT[index].high = MAKE_INTGATE_HIGH(offset,1,DESC_DPL0,1);
    return TRUE;
}

BOOLEAN STDCALL ExceptionsInit()
{
    SetIDTIntGate(0,SEL_RING0_CODE,(UINT32)&KeHandlerDE);
    SetIDTIntGate(1,SEL_RING0_CODE,(UINT32)&KeHandlerDB);
    //SetIDTTrapGate(2,SEL_RING0_CODE,(UINT32)KeHandler);
    SetIDTIntGate(3,SEL_RING0_CODE,(UINT32)&KeHandlerBP);
    SetIDTIntGate(4,SEL_RING0_CODE,(UINT32)&KeHandlerOF);
    SetIDTIntGate(5,SEL_RING0_CODE,(UINT32)&KeHandlerBR);
    SetIDTIntGate(6,SEL_RING0_CODE,(UINT32)&KeHandlerUD);
    SetIDTIntGate(7,SEL_RING0_CODE,(UINT32)&KeHandlerNM);
	SetIDTIntGate(8,SEL_RING0_CODE,(UINT32)&KeHandlerDF);
    //SetIDTTrapGate(9,SEL_RING0_CODE,(UINT32)KeHandler);
    SetIDTIntGate(10,SEL_RING0_CODE,(UINT32)&KeHandlerTS);
    SetIDTIntGate(11,SEL_RING0_CODE,(UINT32)&KeHandlerNP);
    SetIDTIntGate(12,SEL_RING0_CODE,(UINT32)&KeHandlerSS);
    SetIDTIntGate(13,SEL_RING0_CODE,(UINT32)&KeHandlerGP);
    SetIDTIntGate(14,SEL_RING0_CODE,(UINT32)&KeHandlerPF);
    //SetIDTTrapGate(15,SEL_RING0_CODE,(UINT32)KeHandler);
    SetIDTIntGate(16,SEL_RING0_CODE,(UINT32)&KeHandlerMF);
    SetIDTIntGate(17,SEL_RING0_CODE,(UINT32)&KeHandlerAC);
    SetIDTIntGate(18,SEL_RING0_CODE,(UINT32)&KeHandlerMC);
    SetIDTIntGate(19,SEL_RING0_CODE,(UINT32)&KeHandlerXF);
    
    return TRUE;
}

BOOLEAN STDCALL IrqInit()
{
	SetIDTIntGate(0x20,SEL_RING0_CODE,(UINT32)TimerIrq);
	SetIDTIntGate(0x21,SEL_RING0_CODE,(UINT32)KbdIrq);
	SetIDTIntGate(0x22,SEL_RING0_CODE,(UINT32)KeHandlerIRQ2);
	SetIDTIntGate(0x23,SEL_RING0_CODE,(UINT32)KeHandlerIRQ3);
	SetIDTIntGate(0x24,SEL_RING0_CODE,(UINT32)KeHandlerIRQ4);
	SetIDTIntGate(0x25,SEL_RING0_CODE,(UINT32)KeHandlerIRQ5);
	SetIDTIntGate(0x26,SEL_RING0_CODE,(UINT32)KeHandlerIRQ6);
	SetIDTIntGate(0x27,SEL_RING0_CODE,(UINT32)KeHandlerIRQ7);
	SetIDTIntGate(0x28,SEL_RING0_CODE,(UINT32)KeHandlerIRQ8);
	SetIDTIntGate(0x29,SEL_RING0_CODE,(UINT32)KeHandlerIRQ9);
	SetIDTIntGate(0x2A,SEL_RING0_CODE,(UINT32)KeHandlerIRQ10);
	SetIDTIntGate(0x2B,SEL_RING0_CODE,(UINT32)KeHandlerIRQ11);
	SetIDTIntGate(0x2C,SEL_RING0_CODE,(UINT32)KeHandlerIRQ12);
	SetIDTIntGate(0x2D,SEL_RING0_CODE,(UINT32)KeHandlerIRQ13);
	SetIDTIntGate(0x2E,SEL_RING0_CODE,(UINT32)Ide0Irq);
	SetIDTIntGate(0x2F,SEL_RING0_CODE,(UINT32)Ide1Irq);
	return 0;
}


