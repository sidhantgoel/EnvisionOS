#include "keyboard.h"
#include <kernelDescriptors.h>
#include <KernelApic.h>
//#include <debug.h>

#define false 0
#define true 1
#define outb			_outp
#define inb				_inp
#define inw				_inpw
#define DbgPrintf		KeDebugPrintf

#define KBD_DATA	0x60
#define KBD_CMD		0x60
#define KBD_ACK		0x61
#define KBD_STATUS	0x64
#define KBD_CONTROL	0x64

#define IN_ERROR_KEYS		0x00
#define IN_BATEND			0xAA
#define IN_ESC				0xE0
#define IN_ECHO				0xEE
#define IN_BREAK			0xF0
#define IN_ACK				0xFA
#define IN_BAT_FAILED		0xFC
#define IN_INT_FAILURE		0xFD
#define IN_RESEND			0xFE
#define IN_ERROR			0xFF

#define CMD_SETLEDS			0xED
#define CMD_ECHO			0xEE
#define CMD_NOOP			0xEF
#define CMD_SETSCANCODE		0xF0
#define CMD_IDENTIFY		0xF2
#define CMD_SETTYPEMATIC	0xF3
#define CMD_KBD_ENABLE		0xF4
#define CMD_KBD_DISABLE		0xF5
#define CMD_KBD_RESET		0xF6
#define CMD_ALLTYPEMATIC	0xF7
#define CMD_ALLMAKEBREAK	0xF8
#define CMD_ALLMAKEONLY		0xF9
#define CMD_ALL_ALL			0xFA
#define CMD_TYPEMATIC		0xFB
#define CMD_MAKEBREAK		0xFC
#define CMD_MAKEONLY		0xFD
#define CMD_RESEND			0xFE
#define CMD_DIAG			0xFF

#define STATUS_OBF			0x01
#define STATUS_IBF			0x02
#define STATUS_SELFTESTOK	0x04
#define STATUS_LOCKED		0x10
#define STATUS_GTO			0x40
#define STATUS_PERR			0x80

#define CTRL_READBYTE		0x20
#define CTRL_WRITEBYTE		0x60
#define CTRL_DISABLEMOUSE	0xA7
#define CTRL_ENABLEMOUSE	0xA8
#define CTRL_MOUSETEST		0xA9
#define CTRL_CTRLRTEST		0xAA
#define CTRL_KBDTTEST		0xAB
#define CTRL_DEACTIVATE		0xAD
#define CTRL_ACTIVATE		0xAE
#define CTRL_READINPORT		0xC0
#define CTRL_READOUTPORT	0xD0
#define CTRL_WRITEOUTPORT	0xD1
#define CTRL_A20DISABLE		0xDD
#define CTRL_A20ENABLE		0xDF

#define CMDB_EKI			0x01	//enables IRQ1 on keyboard output buffer full
#define CMDB_EIRQ12			0x02	//enables IRQ12 from PS/2 mouse when set
#define CMDB_SYS			0x04	//System Flag (same as b2 in Status register, it seems)
#define CMDB_INGNORELOCK	0x08	//ignore keyboard lock switch when set
#define CMDB_DISABLE		0x10	//disables keyboard when set
#define CMDB_DMS			0x20	//disables PS/2 mouse when set
#define CMDB_KCC			0x40	//convert set 2 scancodes to set 1 ("IBM PC compatibility mode")

#define KBD_BUFFER_SIZE		64

char KbdBuffer[KBD_BUFFER_SIZE];
volatile UINT32 KbdBuffRInd;
volatile UINT32 KbdBuffWInd;
volatile UINT32 KbdBuffCnt;

char ScancodeToAscii(BYTE key);

void KbdReset()
{
	BYTE temp = inb(KBD_ACK);
	outb(KBD_ACK,temp|0x80);
	outb(KBD_ACK,temp);
}

void KbdOut(UINT16 port, BYTE data)
{
	UINT32 timeout;
	BYTE stat;
	for(timeout = 500000L; timeout != 0; timeout--)
	{
		stat = inb(KBD_STATUS);
		if((stat & STATUS_IBF) == 0) break;
	}
	if(timeout == 0)
	{
		return;
	}
	outb(port, data);
}

BYTE KbdIn()
{
	UINT32 timeout;
	BYTE data, stat;
	for(timeout = 500000L; timeout != 0; timeout--)
	{
		stat = inb(KBD_STATUS);
		if(stat & STATUS_OBF)
		{
			data = inb(KBD_DATA);
			if((stat & (STATUS_GTO|STATUS_PERR)) == 0) return data;
		}
	}
	return 0xFF;
}

int KbdCommand(BYTE data, int recv_ack)
{
	BYTE r;
	if(!recv_ack)
	{
		KbdOut(KBD_CMD, data);
		return 0;
	}
	do
	{
		KbdOut(KBD_CMD, data);
		r = KbdIn();
	}while(r==IN_RESEND);
	return (r==IN_ACK)? 0 : -1;
}

int KbdController(BYTE data)
{
	KbdOut(KBD_CONTROL, data);
	return 0;
}

void KbdBufferInit()
{
	KbdBuffRInd=0;
	KbdBuffWInd=0;
	KbdBuffCnt=0;
}

void KbdBufferWrite(char Code)
{
	if(KbdBuffCnt<KBD_BUFFER_SIZE)
	{
		KbdBuffer[KbdBuffWInd] = Code;
		if(KbdBuffWInd==(KBD_BUFFER_SIZE-1))
			KbdBuffWInd=0;
		else
			KbdBuffWInd++;
		KbdBuffCnt++;
	}
	else
	{
		//BEEP
	}
}

char KbdBufferRead()
{
	char Code;
	while(KbdBuffCnt==0);
	Code = KbdBuffer[KbdBuffRInd];
	if(KbdBuffRInd==(KBD_BUFFER_SIZE-1))
		KbdBuffRInd=0;
	else
		KbdBuffRInd++;
	KbdBuffCnt--;
	return Code;
}

void KbdKbd()
{
	BYTE key;
	
	key = KbdIn();
	//DbgPrintf("KEY%x, %x\n", inp(0x64));
	key = ScancodeToAscii(key);
	if(key)
	{
		//DbgPrintf("%c", key);
		KbdBufferWrite(key);
	}
	/*
	switch(key)
	{
		case IN_ERROR_KEYS:
			break;
		case IN_BATEND:
			break;
		case IN_ESC:
			break;
		case IN_ECHO:
			break;
		case IN_BREAK:
			break;
		case IN_ACK:
			break;
		case IN_BAT_FAILED:
			break;
		case IN_INT_FAILURE:
			break;
		case IN_RESEND:	
			break;
		case IN_ERROR:
			break;
	}
	*/
}

int KbdProgram(int sc, int tp, int xlat)
{
	KbdReset();
	if(KbdCommand(CMD_KBD_DISABLE, true)) return -1;
	KbdController(CTRL_WRITEBYTE);
	KbdCommand(CMDB_EKI|CMDB_EIRQ12|CMDB_SYS|(xlat ? CMDB_KCC : 0), false);
	if(KbdCommand(CMD_SETSCANCODE, true)) return -1;
	if(KbdCommand(sc, true)) return -1;
	if(sc==3) if(KbdCommand(CMD_ALL_ALL, true)) return -1; //Doesnt Works on Bochs
	if(KbdCommand(CMD_SETTYPEMATIC, true)) return -1;
	if(KbdCommand(tp, true)) return -1;
	if(KbdCommand(CMD_KBD_ENABLE, true)) return -1;
	return 0;
}

int KbdSetLeds(int CL, int NL, int SL)
{
	if(KbdCommand(CMD_SETLEDS, true)) return -1;
	if(KbdCommand(((CL<<2)|(NL<<1)|SL) & 0x07, true)) return -1;
	return 0;
}

int ShiftDn=0,NumLockDn=0,CapsLockDn=0,ScrollLockDn=0;
int NumLock=0,CapsLock=0,ScrollLock=0;

int KbdInit()
{
	ShiftDn=0;NumLockDn=0;CapsLockDn=0;ScrollLockDn=0;
	NumLock=0;CapsLock=0;ScrollLock=0;
	KbdBufferInit();
	if(KbdProgram(2, 0x40, 1)) return -1;
	if(KbdSetLeds(0, 0, 0)) return -1;
	KeSetIDTTrapGate(0x21,SEL_RING0_CODE,(UINT32)&KeKeyboardIrq);
	KePICIRQEnable(1);
	return 0;
}

char KbdGetKey(UINT32 Timeout)
{
	return KbdBufferRead();
}

void KbdFlush()
{
	KbdBufferInit();
}

////////////////////////////////////////////////////////////////////////////
// SCANCODE 1 TO ASCII
////////////////////////////////////////////////////////////////////////////

char ccase(char c)
{
	if(c >= 'a' && c <= 'z')
	{
		return c-'a'+'A';
	}
	else if(c >= 'A' && c<= 'Z')
	{
		return c-'A'+'a';
	}
	else
	{
		return c;
	}
}
	static const unsigned char map[128] =
	{
		/*00*/	0,		0,		'1',	'2',	'3',	'4',	'5',	'6',
		/*08*/	'7',	'8',	'9',	'0',	'-',	'=',	'\b',	'\t',
		/*10*/	'q',	'w',	'e',	'r',	't',	'y',	'u',	'i',
		/*18*/	'o',	'p',	'[',	']',	'\n',	0,		'a',	's',
		/*20*/	'd',	'f',	'g',	'h',	'j',	'k',	'l',	';',
		/*28*/	'\'',	'`',	0,		'\\',	'z',	'x',	'c',	'v',
		/*30*/	'b',	'n',	'm',	',',	'.',	'/',	0,		0,
		/*38*/	0,		' ',	0,		0,		0,		0,		0,		0,
		/*40*/	0,		0,		0,		0,		0,		0,		0,		0,
		/*48*/	0,		0,		'-',	0,		'5',	0,		'+',	0,
		/*50*/	0,		0,		0,		0,		0,		0,		0,		0,
		/*58*/	0,		0,		0,		0,		0,		0,		0,		0,
		/*60*/	0,		0,		0,		0,		0,		0,		0,		0,
		/*68*/	0,		0,		0,		0,		0,		0,		0,		0,
		/*70*/	0,		0,		0,		0,		0,		0,		0,		0,
		/*78*/	0,		0,		0,		0,		0,		0,		0,		0
	};
	static const unsigned char shift_map[128] =
	{
		/*00*/	0,		0,		'!',	'@',	'#',	'$',	'%',	'^',
		/*08*/	'&',	'*',	'(',	')',	'_',	'+',	'\b',	'\t',
		/*10*/	'Q',	'W',	'E',	'R',	'T',	'Y',	'U',	'I',
		/*18*/	'O',	'P',	'{',	'}',	'\n',	0,		'A',	'S',
		/*20*/	'D',	'F',	'G',	'H',	'J',	'K',	'L',	':',
		/*28*/	'\"',	'~',	0,		'|',	'Z',	'X',	'C',	'V',
		/*30*/	'B',	'N',	'M',	'<',	'>',	'?',	0,		0,
		/*38*/	0,		' ',	0,		0,		0,		0,		0,		0,
		/*40*/	0,		0,		0,		0,		0,		0,		0,		0,
		/*48*/	0,		0,		'-',	0,		'5',	0,		'+',	0,
		/*50*/	0,		0,		0,		0,		0,		0,		0,		0,
		/*58*/	0,		0,		0,		0,		0,		0,		0,		0,
		/*60*/	0,		0,		0,		0,		0,		0,		0,		0,
		/*68*/	0,		0,		0,		0,		0,		0,		0,		0,
		/*70*/	0,		0,		0,		0,		0,		0,		0,		0,
		/*78*/	0,		0,		0,		0,		0,		0,		0,		0
	};
char ScancodeToAscii(BYTE key)
{

	char c=0;
	if(key & 0x80)
	{
		switch(key)
		{
		case 0x80+0x2A:
		case 0x80+0x36:
			ShiftDn=0;
			break;
		case 0x80+0x3A:
			CapsLockDn=0;
			break;
		case 0x80+0x45:
			NumLockDn=0;
			break;
		case 0x80+0x46:
			ScrollLockDn=0;
			break;
		}
	}
	else
	{
		switch(key)
		{
		case 0x2A:
		case 0x36:
			ShiftDn=1;
			break;
		case 0x3A:
			if(CapsLockDn) break;
			CapsLockDn=1;
			CapsLock ^= 1;
			KbdSetLeds(CapsLock, NumLock, ScrollLock);
			break;
		case 0x45:
			if(NumLockDn) break;
			NumLockDn=1;
			NumLock ^= 1;
			KbdSetLeds(CapsLock, NumLock, ScrollLock);
			break;
		case 0x46:
			if(ScrollLockDn) break;
			ScrollLockDn=1;
			ScrollLock ^= 1;
			KbdSetLeds(CapsLock, NumLock, ScrollLock);
			break;
		default:
			if(ShiftDn)
				c = shift_map[key];
			else
				c = map[key];
			if(CapsLock)
				c = ccase(c);
		}
	}
	return c;
}