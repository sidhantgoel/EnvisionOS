#include <arch\arch.h>
#include <ata.h>
#include <vm86.h>
#include <debug.h>
#include <rs232.h>
#include <descriptors.h>
#include <pic.h>
#include <timer.h>
#include <keyboard.h>

VMTSS VM86_TSS;

#define JMP_FAR(seg,off)            \
    __asm _emit 0xEA                \
    __asm _emit off & 0xFF          \
    __asm _emit off >> 8 & 0xFF     \
    __asm _emit off >> 16 & 0xFF    \
    __asm _emit off >> 24 & 0xFF    \
    __asm _emit seg & 0xFF          \
    __asm _emit seg >> 8 & 0xFF

#define CALL_FAR(seg,off)           \
    __asm _emit 0x9A                \
    __asm _emit off & 0xFF          \
    __asm _emit off >> 8 & 0xFF     \
    __asm _emit off >> 16 & 0xFF    \
    __asm _emit off >> 24 & 0xFF    \
    __asm _emit seg & 0xFF          \
    __asm _emit seg >> 8 & 0xFF

#define	PFX_ES		0x001
#define	PFX_CS		0x002
#define	PFX_SS		0x004
#define	PFX_DS		0x008
#define	PFX_FS		0x010
#define	PFX_GS		0x020

#define	PFX_OP32	0x040
#define	PFX_ADR32	0x080
#define	PFX_LOCK	0x100
#define	PFX_REPNE	0x200
#define	PFX_REP		0x400

VM86_ARGS vm86_args;
void Vm86Call(VM86_ARGS *args)
{
	UINT32 Eflags;
	BYTE pMasks[2];

	memcpy(&vm86_args,args,sizeof(VM86_ARGS));
    memset(&VM86_TSS,0,sizeof(VM86_TSS));

    VM86_TSS.tss.esp0 = 0x800000;
    VM86_TSS.tss.ss0 = SEL_RING0_DATA;
    VM86_TSS.tss.eip = vm86_args.Eip;
    VM86_TSS.tss.eflags = args->Eflags|EFLAGS_VM;
    VM86_TSS.tss.eax = vm86_args.Eax;
	VM86_TSS.tss.ecx = vm86_args.Ecx;
	VM86_TSS.tss.edx = vm86_args.Edx;
	VM86_TSS.tss.ebx = vm86_args.Ebx;
	VM86_TSS.tss.esp = vm86_args.Esp;
	VM86_TSS.tss.ebp = vm86_args.Ebp;
	VM86_TSS.tss.esi = vm86_args.Esi;
	VM86_TSS.tss.edi = vm86_args.Edi;
	VM86_TSS.tss.cs = vm86_args.Cs;
	VM86_TSS.tss.ss = vm86_args.Ss;
	VM86_TSS.tss.ds = vm86_args.Ds;
	VM86_TSS.tss.fs = vm86_args.Fs;
	VM86_TSS.tss.gs = vm86_args.Gs;
    VM86_TSS.tss.bitmap = (WORD)sizeof(TSS);

	__asm
	{
		pushfd
		pop eax
		mov Eflags, eax
		cli
	}

	pMasks[0] = inp(0x21);
	pMasks[1] = inp(0xA1);
    
	outb(0x21, Pic0Mask);
	outb(0xA1, Pic1Mask);
    CALL_FAR(SEL_VM86_TSS,0);

	outb(0x21, pMasks[0]);
	outb(0xA1, pMasks[1]);

	__asm
	{
		mov eax, Eflags
		push eax
		popfd
	}

	memcpy(args,&vm86_args,sizeof(VM86_ARGS));
}

void Vm86CallInt(VM86_ARGS *args, BYTE IntNum)
{
	*(BYTE*)0x7C00 = 0xCD;
	*(BYTE*)0x7C01 = IntNum;
	*(BYTE*)0x7C02 = 0xCF;
	
	args->Eflags = EFLAGS_1|EFLAGS_IF|EFLAGS_IOPL2;
	args->Eip = 0x7c00;
	args->Esp = 0x7c00;
	args->Cs = 0;
	args->Ss = 0;
	args->EndAddr = 0x7c03;

	Vm86Call(args);
}

void Vm86Exit(REGS_E *regs, UINT32 ErrorCode)
{
	vm86_args.Eip = regs->eip;
	vm86_args.Eax = regs->eax;
	vm86_args.Ecx = regs->ecx;
	vm86_args.Edx = regs->edx;
	vm86_args.Ebx = regs->ebx;
	vm86_args.Esp = regs->user_esp;
	vm86_args.Ebp = regs->ebp;
	vm86_args.Esi = regs->esi;
	vm86_args.Edi = regs->edi;
	vm86_args.Cs = regs->cs;
	vm86_args.Ds = regs->v_ds;
	vm86_args.Ss = regs->user_ss;
	vm86_args.Es = regs->v_es;
	vm86_args.Fs = regs->v_fs;
	vm86_args.Gs = regs->v_gs;
	vm86_args.Eflags = regs->eflags;
	vm86_args.ErrorCode = ErrorCode;
}

UINT32 __stdcall Vm86Gpf(REGS_E *regs)
{
    BYTE opcode,prefix;
    prefix = 0;
	while(1)
	{
		opcode = peekb(regs->cs,regs->eip);
        regs->eip = (regs->eip+1) & 0xFFFF;
		switch(opcode)
		{
		case 0x26:
			prefix |= PFX_ES;
			break;
		case 0x2E:
			prefix |= PFX_CS;
			break;
		case 0x36:
			prefix |= PFX_SS;
			break;
		case 0x3E:
			prefix |= PFX_DS;
			break;
		case 0x64:
			prefix |= PFX_FS;
			break;
		case 0x65:
			prefix |= PFX_GS;
			break;
		case 0x66:
			prefix |= PFX_OP32;
			break;
		case 0x67:
			prefix |= PFX_ADR32;
			break;
		case 0xF0:
			prefix |= PFX_LOCK;
			break;
		case 0xF2:
			prefix |= PFX_REPNE;
			break;
		case 0xF3:
			prefix |= PFX_REP;
			break;
		default:
			goto END;
		}
	}

END:
    switch(opcode)
	{
	case 0x9C:  //PUSHF
        //printf("\nPUSHF\nEIP=%x",regs->eip);
		if(prefix & PFX_OP32)
        {
            regs->user_esp = (regs->user_esp-4) & 0xFFFF;
            pokel(regs->user_ss ,regs->user_esp,regs->eflags);
        }
		else
        {
			regs->user_esp = (regs->user_esp-2) & 0xFFFF;
            pokew(regs->user_ss ,regs->user_esp,regs->eflags);
        }
		break;

	case 0x9D:  //POPF
        //printf("\nPOPF\nEIP=%x",regs->eip);
		if(prefix & PFX_OP32)
		{
            regs->eflags = peekl(regs->user_ss,regs->user_esp)|EFLAGS_VM;
            regs->user_esp = (regs->user_esp+4) & 0xFFFF;
		}
		else
		{
			regs->eflags = peekw(regs->user_ss,regs->user_esp)|EFLAGS_VM;
            regs->user_esp = (regs->user_esp+2) & 0xFFFF;
		}
		break;

	case 0xCD:  //INT nn
        opcode = peekb(regs->cs,regs->eip);
        regs->eip = (regs->eip+1) & 0xFFFF;
        
		//printf("\nINT %d, EIP=%x",(UINT32)opcode,regs->eip);

        regs->user_esp = (regs->user_esp-2) & 0xFFFF;
        pokew(regs->user_ss ,regs->user_esp,regs->eflags);
        regs->user_esp = (regs->user_esp-2) & 0xFFFF;
        pokew(regs->user_ss ,regs->user_esp,regs->cs);
        regs->user_esp = (regs->user_esp-2) & 0xFFFF;
        pokew(regs->user_ss ,regs->user_esp,regs->eip);
        
        regs->eflags &= ~0x200;

        regs->eip = (regs->eip & ~0xFFFF) | peekw(0, (opcode*4) + 0);
	    regs->cs = peekw(0, (opcode*4) + 2);

        break;

	case 0xCF:  //IRET
        //printf("\nIRET\nEIP=%x",regs->eip);
		if((regs->user_ss == vm86_args.Ss)&&(regs->user_esp == vm86_args.Esp))
		{
			//printf("\nDONE");
			Vm86Exit(regs,0);
			return 1;
		}
		if(prefix & PFX_OP32)   /* pop (E)IP, CS, (E)FLAGS */
		{
            regs->eip = peekl(regs->user_ss,regs->user_esp);
            regs->user_esp = (regs->user_esp+4) & 0xFFFF;
            regs->cs = peekl(regs->user_ss,regs->user_esp);
            regs->user_esp = (regs->user_esp+4) & 0xFFFF;
            regs->eflags = peekl(regs->user_ss,regs->user_esp);
            regs->user_esp = (regs->user_esp+4) & 0xFFFF;
		}
		else
		{
            regs->eip = peekw(regs->user_ss,regs->user_esp);
            regs->user_esp = (regs->user_esp+2) & 0xFFFF;
            regs->cs = peekw(regs->user_ss,regs->user_esp);
            regs->user_esp = (regs->user_esp+2) & 0xFFFF;
            regs->eflags = (regs->eflags & 0xFFFF0000L) | peekw(regs->user_ss,regs->user_esp);
            regs->user_esp = (regs->user_esp+2) & 0xFFFF;
		}
		break;

	case 0xFA:  //CLI
        //printf("\nCLI\nEIP=%x",regs->eip);
		regs->eflags &= ~0x200;
		break;

	case 0xFB:  //STI
        //printf("\nSTI\nEIP=%x",regs->eip);
		regs->eflags |= 0x200;
		break;

	case 0xF4:  //HLT
		//printf("\nHLT\nEIP=%x",regs->eip);
		break;

	default:
		//printf("\nUNHANDLED OPCODE = %x, EIP=%x:%x",(UINT32)opcode, regs->cs, regs->eip);
		Vm86Exit(regs,1);
		return 1;
	}
	return 0;
}



void __stdcall Vm86HandleIrq(int irq, REGS *regs)
{
	//printf("\nIRQ %d, EIP=%x:%x",(UINT32)irq,regs->cs, regs->eip);
	if(irq<8)
	{
        regs->user_esp = (regs->user_esp-2) & 0xFFFF;
        pokew(regs->user_ss ,regs->user_esp,regs->eflags);
        regs->user_esp = (regs->user_esp-2) & 0xFFFF;
        pokew(regs->user_ss ,regs->user_esp,regs->cs);
        regs->user_esp = (regs->user_esp-2) & 0xFFFF;
        pokew(regs->user_ss ,regs->user_esp,regs->eip);
        
        regs->eflags &= ~0x200;
		//regs->eflags |= EFLAGS_TF;

        regs->eip = peekw(0, ((0x08+irq) * 4) + 0);
	    regs->cs = peekw(0, ((0x08+irq) * 4) + 2);
	}
	else
	{
		regs->user_esp = (regs->user_esp-2) & 0xFFFF;
        pokew(regs->user_ss ,regs->user_esp,regs->eflags);
        regs->user_esp = (regs->user_esp-2) & 0xFFFF;
        pokew(regs->user_ss ,regs->user_esp,regs->cs);
        regs->user_esp = (regs->user_esp-2) & 0xFFFF;
        pokew(regs->user_ss ,regs->user_esp,regs->eip);
        
        regs->eflags &= ~0x200;
		//regs->eflags |= EFLAGS_TF;

        regs->eip = peekw(0, ((0x70+irq-8) * 4) + 0);
	    regs->cs = peekw(0, ((0x70+irq-8) * 4) + 2);
	}
	//printf(", new EIP=%x:%x", regs->cs, regs->eip);
	//TxtHexDump(0,256);
}

void Vm86Init()
{
	int i;
	VM86_ARGS args;

	for(i=0;i<8;i++)
	{
		((UINT32*)(0x20*4))[i] = ((UINT32*)(0x08*4))[i];
		((UINT32*)(0x28*4))[i] = ((UINT32*)(0x70*4))[i];
	}
}

#define IrqHandler0(name,num)		\
void __declspec(naked) name()		\
{									\
	__asm push gs					\
	__asm push fs					\
	__asm push es					\
	__asm push ds					\
	__asm pushad					\
	__asm mov ax, SEL_RING0_DATA	\
	__asm mov ds, ax				\
	__asm mov es, ax				\
	__asm mov fs, ax				\
	__asm mov gs, ax				\
	__asm mov eax, [esp+14*4]		\
	__asm bt eax, 17				\
	__asm jnc novm86				\
	__asm mov eax, esp				\
	__asm push eax					\
	__asm push num					\
	__asm call Vm86HandleIrq		\
	__asm jmp exit					\
	__asm novm86:					\
	__asm mov dx,20h				\
    __asm mov al,20h				\
    __asm out dx,al					\
	__asm exit:						\
	__asm popad						\
	__asm pop ds					\
	__asm pop es					\
	__asm pop fs					\
	__asm pop gs					\
	__asm iretd						\
}

#define IrqHandler1(name,num)		\
void __declspec(naked) name()		\
{									\
	__asm push gs					\
	__asm push fs					\
	__asm push es					\
	__asm push ds					\
	__asm pushad					\
	__asm mov ax, SEL_RING0_DATA	\
	__asm mov ds, ax				\
	__asm mov es, ax				\
	__asm mov fs, ax				\
	__asm mov gs, ax				\
	__asm mov eax, [esp+14*4]		\
	__asm bt eax, 17				\
	__asm jnc novm86				\
	__asm mov eax, esp				\
	__asm push eax					\
	__asm push num					\
	__asm call Vm86HandleIrq		\
	__asm jmp exit					\
	__asm novm86:					\
	__asm mov dx,0A0h				\
    __asm mov al,20h				\
    __asm out dx,al					\
    __asm mov dx,20h				\
    __asm out dx,al					\
	__asm exit:						\
	__asm popad						\
	__asm pop ds					\
	__asm pop es					\
	__asm pop fs					\
	__asm pop gs					\
	__asm iretd						\
}

void __declspec(naked) TimerIrq()
{									
	__asm push gs					
	__asm push fs					
	__asm push es					
	__asm push ds					
	__asm pushad					
	__asm mov ax, SEL_RING0_DATA	
	__asm mov ds, ax				
	__asm mov es, ax				
	__asm mov fs, ax				
	__asm mov gs, ax				
	__asm mov eax, [esp+14*4]		
	__asm bt eax, 17				
	__asm jnc novm86				
	__asm mov eax, esp				
	__asm push eax					
	__asm push 0					
	__asm call Vm86HandleIrq		
	__asm jmp exit					
	__asm novm86:
	__asm call TimerTimer
	__asm mov dx,20h				
    __asm mov al,20h				
    __asm out dx,al					
	__asm exit:						
	__asm popad						
	__asm pop ds					
	__asm pop es					
	__asm pop fs					
	__asm pop gs					
	__asm iretd						
}

void __declspec(naked) KbdIrq()
{									
	__asm push gs					
	__asm push fs					
	__asm push es					
	__asm push ds					
	__asm pushad					
	__asm mov ax, SEL_RING0_DATA	
	__asm mov ds, ax				
	__asm mov es, ax				
	__asm mov fs, ax				
	__asm mov gs, ax				
	__asm mov eax, [esp+14*4]		
	__asm bt eax, 17				
	__asm jnc novm86				
	__asm mov eax, esp				
	__asm push eax					
	__asm push 1					
	__asm call Vm86HandleIrq		
	__asm jmp exit					
	__asm novm86:
	__asm call KbdKbd
	__asm mov dx,20h				
    __asm mov al,20h				
    __asm out dx,al					
	__asm exit:						
	__asm popad						
	__asm pop ds					
	__asm pop es					
	__asm pop fs					
	__asm pop gs					
	__asm iretd						
}

void __declspec(naked) Ide0Irq()
{									
	__asm push gs					
	__asm push fs					
	__asm push es					
	__asm push ds					
	__asm pushad					
	__asm mov ax, SEL_RING0_DATA	
	__asm mov ds, ax				
	__asm mov es, ax				
	__asm mov fs, ax				
	__asm mov gs, ax				
	__asm mov eax, [esp+14*4]		
	__asm bt eax, 17				
	__asm jnc novm86				
	__asm mov eax, esp				
	__asm push eax					
	__asm push 1					
	__asm call Vm86HandleIrq		
	__asm jmp exit					
	__asm novm86:
	__asm call AtaIntPrimary
	__asm mov dx,0A0h
    __asm mov al,20h
    __asm out dx,al
    __asm mov dx,20h
    __asm out dx,al
	__asm exit:						
	__asm popad						
	__asm pop ds					
	__asm pop es					
	__asm pop fs					
	__asm pop gs					
	__asm iretd						
}

void __declspec(naked) Ide1Irq()
{									
	__asm push gs					
	__asm push fs					
	__asm push es					
	__asm push ds					
	__asm pushad					
	__asm mov ax, SEL_RING0_DATA	
	__asm mov ds, ax				
	__asm mov es, ax				
	__asm mov fs, ax				
	__asm mov gs, ax				
	__asm mov eax, [esp+14*4]		
	__asm bt eax, 17				
	__asm jnc novm86				
	__asm mov eax, esp				
	__asm push eax					
	__asm push 1					
	__asm call Vm86HandleIrq		
	__asm jmp exit					
	__asm novm86:
	__asm call AtaIntSecondary
	__asm mov dx,0A0h
    __asm mov al,20h
    __asm out dx,al
    __asm mov dx,20h
    __asm out dx,al
	__asm exit:						
	__asm popad						
	__asm pop ds					
	__asm pop es					
	__asm pop fs					
	__asm pop gs					
	__asm iretd						
}

IrqHandler0(KeHandlerIRQ2,2)
IrqHandler0(KeHandlerIRQ3,3)
IrqHandler0(KeHandlerIRQ4,4)
IrqHandler0(KeHandlerIRQ5,5)
IrqHandler0(KeHandlerIRQ6,6)
IrqHandler0(KeHandlerIRQ7,7)
IrqHandler1(KeHandlerIRQ8,8)
IrqHandler1(KeHandlerIRQ9,9)
IrqHandler1(KeHandlerIRQ10,10)
IrqHandler1(KeHandlerIRQ11,11)
IrqHandler1(KeHandlerIRQ12,12)
IrqHandler1(KeHandlerIRQ13,13)


char strUnhandledInterrupt[] = "Unhandled Interrupt";
void __declspec(naked) UnhandledInterrupt()
{
    __asm
    {
        push gs
        push fs
        push es
        push ds
        pushad
        mov ax, SEL_RING0_DATA
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        push esp
        call DbgInterrupt
        popad
        pop ds
        pop es
        pop fs
        pop gs
        iretd
    }
}

#define MAGIC_BP xchg bx, bx

void __declspec(naked) KeHandlerDE()
{
    __asm
    {
		push 0
        push gs
        push fs
        push es
        push ds
        pushad
        mov ax, SEL_RING0_DATA
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        push esp
        push EXCEPT_DE
        call DbgException
        popad
        pop ds
        pop es
        pop fs
        pop gs
		add esp, 4
        iretd
    }
}

void __declspec(naked) KeHandlerDB()
{
    __asm
    {
		push 0
        push gs
        push fs
        push es
        push ds
        pushad
        mov ax, SEL_RING0_DATA
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        push esp
        push EXCEPT_DB
        call DbgException
        popad
        pop ds
        pop es
        pop fs
        pop gs
		add esp, 4
        iretd
    }
}

void __declspec(naked) KeHandlerBP()
{
    __asm
    {
		push 0
        push gs
        push fs
        push es
        push ds
        pushad
        mov ax, SEL_RING0_DATA
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        push esp
        push EXCEPT_BP
        call DbgException
        popad
        pop ds
        pop es
        pop fs
        pop gs
		add esp, 4
        iretd
    }
}

void __declspec(naked) KeHandlerOF()
{
    __asm
    {
		push 0
        push gs
        push fs
        push es
        push ds
        pushad
        mov ax, SEL_RING0_DATA
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        push esp
        push EXCEPT_OF
        call DbgException
        popad
        pop ds
        pop es
        pop fs
        pop gs
		add esp, 4
        iretd
    }
}

void __declspec(naked) KeHandlerBR()
{
    __asm
    {
		push 0
        push gs
        push fs
        push es
        push ds
        pushad
        mov ax, SEL_RING0_DATA
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        push esp
        push EXCEPT_BR
        call DbgException
        popad
        pop ds
        pop es
        pop fs
        pop gs
		add esp, 4
        iretd
    }
}

void __declspec(naked) KeHandlerUD()
{
    __asm
    {
		push 0
        push gs
        push fs
        push es
        push ds
        pushad
        mov ax, SEL_RING0_DATA
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        push esp
        push EXCEPT_UD
        call DbgException
        popad
        pop ds
        pop es
        pop fs
        pop gs
		add esp, 4
        iretd
    }
}

void __declspec(naked) KeHandlerNM()
{
    __asm
    {
		push 0
        push gs
        push fs
        push es
        push ds
        pushad
        mov ax, SEL_RING0_DATA
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        push esp
        push EXCEPT_NM
        call DbgException
        popad
        pop ds
        pop es
        pop fs
        pop gs
		add esp, 4
        iretd
    }
}

void __declspec(naked) KeHandlerDF()
{
    __asm
    {
        push gs
        push fs
        push es
        push ds
        pushad
        mov ax, SEL_RING0_DATA
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        push esp
        push EXCEPT_DF
        call DbgException
        popad
        pop ds
        pop es
        pop fs
        pop gs
		add esp, 4
        iretd
    }
}

void __declspec(naked) KeHandlerTS()
{
    __asm
    {
        push gs
        push fs
        push es
        push ds
        pushad
        mov ax, SEL_RING0_DATA
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        push esp
        push EXCEPT_TS
        call DbgException
        popad
        pop ds
        pop es
        pop fs
        pop gs
        add esp,4
        iretd
    }
}


void __declspec(naked) KeHandlerNP()
{
    __asm
    {
        push gs
        push fs
        push es
        push ds
        pushad
        mov ax, SEL_RING0_DATA
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        push esp
        push EXCEPT_NP
        call DbgException
        popad
        pop ds
        pop es
        pop fs
        pop gs
        add esp,4
        iretd
    }
}

void __declspec(naked) KeHandlerSS()
{
    __asm
    {
        push gs
        push fs
        push es
        push ds
        pushad
        mov ax, SEL_RING0_DATA
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        push esp
        push EXCEPT_SS
        call DbgException
        popad
        pop ds
        pop es
        pop fs
        pop gs
        add esp,4
        iretd
    }
}

void __declspec(naked) KeHandlerGP()
{
    __asm
    {
        push gs
        push fs
        push es
        push ds
        pushad
        mov ax, SEL_RING0_DATA
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
		mov eax, [esp+15*4]	//Saved EFLAGS
		bt eax, 17
        jnc gpnovm86
		mov eax, esp
        push eax
        call Vm86Gpf
		cmp eax,1
		jne gpexit
		pushfd
		pop eax
		or eax, EFLAGS_NT
		push eax
		popfd
		jmp gpexit
gpnovm86:
		push esp
        push EXCEPT_GP
        call DbgException
gpexit:
        popad
        pop ds
        pop es
        pop fs
        pop gs
        add esp,4
        iretd
    }
}

void __declspec(naked) KeHandlerPF()
{
    __asm
    {
        push gs
        push fs
        push es
        push ds
        pushad
        mov ax, SEL_RING0_DATA
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        push esp
        push EXCEPT_PF
        call DbgException
        popad
        pop ds
        pop es
        pop fs
        pop gs
        add esp,4
        iretd
    }
}

void __declspec(naked) KeHandlerMF()
{
    __asm
    {
		push 0
        push gs
        push fs
        push es
        push ds
        pushad
        mov ax, SEL_RING0_DATA
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        push esp
        push EXCEPT_MF
        call DbgException
        popad
        pop ds
        pop es
        pop fs
        pop gs
		add esp, 4
        iretd
    }
}

void __declspec(naked) KeHandlerAC()
{
    __asm
    {
        push gs
        push fs
        push es
        push ds
        pushad
        mov ax, SEL_RING0_DATA
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        push esp
        push EXCEPT_AC
        call DbgException
        popad
        pop ds
        pop es
        pop fs
        pop gs
        add esp,4
        iretd
    }
}

void __declspec(naked) KeHandlerMC()
{
    __asm
    {
		push 0
        push gs
        push fs
        push es
        push ds
        pushad
        mov ax, SEL_RING0_DATA
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        push esp
        push EXCEPT_MC
        call DbgException
        popad
        pop ds
        pop es
        pop fs
        pop gs
		add esp, 4
        iretd
    }
}

void __declspec(naked) KeHandlerXF()
{
    __asm
    {
		push 0
        push gs
        push fs
        push es
        push ds
        pushad
        mov ax, SEL_RING0_DATA
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        push esp
        push EXCEPT_XF
        call DbgException
        popad
        pop ds
        pop es
        pop fs
        pop gs
		add esp, 4
        iretd
    }
}