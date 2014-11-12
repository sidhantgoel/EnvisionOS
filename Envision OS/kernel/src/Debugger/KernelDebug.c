///////////////////////////////////////////////////////////////////////////////////
//----------------------------------KernelDebug.c--------------------------------//
//                                      v 1.0                                    //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
// Created by: Sidhant Goel                                                      //
// Created on: 14 feb 2007                                                       //
//                                                                               //
//--------------------------------------Notes------------------------------------//
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
#include <KernelDebug.h>

#define PUTINT_MINUS        0x0001
#define PUTINT_PLUS         0x0002
#define PUTINT_SPACE        0x0004
#define PUTINT_HASH         0x0008
#define PUTINT_ZERO         0x0010
#define PUTINT_WIDTH        0x0020
#define PUTINT_PRECISION    0x0040
#define PUTINT_SIGN         0x0080
#define PUTINT_UCASE        0x0100
#define PUTINT_MAX          PUTINT_UCASE

#define PUTSTR_PRECISION    PUTINT_PRECISION

#define PRINTF_MINUS        PUTINT_MINUS
#define PRINTF_PLUS         PUTINT_PLUS
#define PRINTF_SPACE        PUTINT_SPACE
#define PRINTF_HASH         PUTINT_HASH
#define PRINTF_ZERO         PUTINT_ZERO
#define PRINTF_WIDTH        PUTINT_WIDTH
#define PRINTF_PRECISION    PUTINT_PRECISION
#define PRINTF_SIGN         PUTINT_SIGN
#define PRINTF_UCASE        PUTINT_UCASE
#define PRINTF_LENGTH_SHORT (PUTINT_MAX<<1)
#define PRINTF_LENGTH_LONG  (PUTINT_MAX<<2)
#define PRINTF_LENGTH_DLONG (PUTINT_MAX<<3)

typedef struct _PRINTF_OPTIONS
{
    UINT32  Flags;
    UINT32  Base;
    UINT32  Width;
    UINT32  Precision;
    UINT32  Length;
    CHAR    Specifier;
    union
    {
        CHAR        *String;
        CHAR        Char;
        SINT32      Integer;
        UINT32      UInteger;
        long double LongDouble;
        VIRTUAL_PTR Pointer;
    };

}PRINTF_OPTIONS;

char MsgTblSuccess[][32] =
{
    "STATUS_SUCCESS"
};

char MsgTblInfo[][32] =
{
    "STATUS_INFO_FALSE",
    "STATUS_TRUE",
    "STATUS_GDTE_FREE",
    "STATUS_GDTE_PRESENT"
};

char MsgTblWarning[][32] =
{
    "STATUS_WARNING"
};

char MsgTblError[][32] =
{
    "STATUS_ERROR",
    "STATUS_INVALID_PARAM",
    "STATUS_ACCESS_DENIED",
    "STATUS_ACCESS_VOILATION",
    "STATUS_DIVIDE_BY_ZERO",
    "STATUS_FILE_NOT_FOUND",
    "STATUS_INDEX_INVALID",
    "STATUS_GDTE_EXIST",
    "STATUS_GDTE_NOT_PRESENT",
    "STATUS_INVALID_ADDRESS"
};

VIRTUAL_PTR DebugConsole=NULL;

#define PORT1 0x3F8
#define PORT2 0x2F8
#define PORT3 0x3E8
#define PORT4 0x2E8

CHAR DIG_UPPER[] = "0123456789ABCDEF";
CHAR DIG_LOWER[] = "0123456789abcdef";

#define MAX_CON_FUNC            0x04
#define CON_FUNC_CLEAR_SCREEN   0x00
#define CON_FUNC_SCROLL_LINE    0x01
#define CON_FUNC_WRITE_CHAR     0x02
#define CON_FUNC_UPDATE         0x03

void *ConsoleFunctions[MAX_CON_FUNC];

char FaultMessage[] = "Stack Fault ESP : ";
char *FaullMsg = FaultMessage;
//BYTE lock;

KESTATUS __declspec(naked) STDCALL KeDebugPanic()
{
    while(*FaullMsg)
    {
        while ((_inp(PORT1+5)&0x20)==0);
        _outp(PORT1,*FaullMsg);
        FaullMsg++;
    }
    __asm
    {
        retn
    }
    //return STATUS_SUCCESS;
    //_jmp(KernelPanic);
}

KESTATUS STDCALL KeDebugInit()
{
    _outp(PORT1 + 1 , 0);   /* Turn off interrupts - Port1 */

    /*         PORT 1 - Communication Settings         */

    _outp(PORT1 + 3 , 0x80);  /* SET DLAB ON */
    _outpw(PORT1 + 0 , 0x01);  /* Set Baud rate - Divisor Latch Low Byte */
			      /* Default 0x03 =  38,400 BPS */
			      /*         0x01 = 115,200 BPS */
			      /*         0x02 =  57,600 BPS */
			      /*         0x06 =  19,200 BPS */
			      /*         0x0C =   9,600 BPS */
			      /*         0x18 =   4,800 BPS */
			      /*         0x30 =   2,400 BPS */
    //_outp(PORT1 + 1 , 0x00);  /* Set Baud rate - Divisor Latch High Byte */
    _outp(PORT1 + 3 , 0x03);  /* 8 Bits, No Parity, 1 Stop Bit */
    //_outp(PORT1 + 2 , 0xC7);  /* FIFO Control Register */
    _outp(PORT1 + 2 , 0x00);
    //_outp(PORT1 + 4 , 0x0B);  /* Turn on DTR, RTS, and OUT2 */
    _outp(PORT1 + 4 , 0x00);

    KeTextConsoleInit();
    KeDebugConsoleHook(CON_FUNC_CLEAR_SCREEN,&KeTextConsoleClearScreen);
    KeDebugConsoleHook(CON_FUNC_SCROLL_LINE,&KeTextConsoleScrollLine);
    KeDebugConsoleHook(CON_FUNC_WRITE_CHAR,&KeTextConsolePutch);
    KeDebugConsoleHook(CON_FUNC_UPDATE,&KeTextConsoleUpdate);
    //KeConsoleCreate(&DebugConsole,(VIRTUAL_PTR)0xB8000,80,25,CONSOLE_CLEARSCREEN|CONSOLE_DEFAULT|BGCOLOR_BLUE|FGCOLOR_BRIGHTWHITE);
    //lock=0;
    
    //KeDebugPrintf("%#8X %#8X",0xFF,0xEE);
    //_hang();

    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeDebugUpdate()
{
    return (*(PFN_UPDATE)ConsoleFunctions[CON_FUNC_UPDATE])();
}

KESTATUS STDCALL KeDebugScrollLine()
{
    return (*(PFN_SCROLL_LINE)ConsoleFunctions[CON_FUNC_SCROLL_LINE])();
}

KESTATUS STDCALL KeDebugPutChar(CHAR ch)
{
    if(ch=='\n')
    {
        while ((_inp(PORT1+5)&0x20)==0);
        _outp(PORT1,'\r');
        while ((_inp(PORT1+5)&0x20)==0);
        _outp(PORT1,'\n');
    }
    else
    {
        while ((_inp(PORT1+5)&0x20)==0);
        _outp(PORT1,ch);
    }
    return (*(PFN_WRITE_CHAR)ConsoleFunctions[CON_FUNC_WRITE_CHAR])(ch);
}

KESTATUS STDCALL KeDebugPutStr(char *str,UINT32 flags,INT32 precision)
{
    CHAR *s = str;
    INT32 i;
    if(flags & PUTSTR_PRECISION)   
    {
        while (*s && precision)
        {
            KeDebugPutChar(*s);
            s++;
            precision--;
        }
        while(precision)
        {
            KeDebugPutChar(' ');
            precision--;
        }
    }
    else
    {
        while (*s)
        {
            KeDebugPutChar(*s);
            s++;
        }
    }
    KeDebugUpdate();
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeDebugClearScreen()
{
    return (*(PFN_CLEAR_SCREEN)ConsoleFunctions[CON_FUNC_CLEAR_SCREEN])();
}

KESTATUS STDCALL KeDebugPutFloat(double num)
{
    INT32 a;
    INT32 b;
    
    a = num;
    b = ((num-a) * 10000);
    if(b & 0x80000000) b=-b;

    //KeDebugSerialHexDump(&num,8);
    KeDebugPutInt(a,10,PUTINT_SIGN,0,0);
    KeDebugPutChar('.');
    KeDebugPutInt(b,10,0,0,0);
	return STATUS_SUCCESS;
}

KESTATUS STDCALL KeDebugPutInt(UINT32 num,UINT32 base,UINT32 flags,UINT32 width,UINT32 precision)
{
    UINT32 temp1=0,temp2=num;
    UINT32 digit_count=0;
    UINT32 pad_count=0;
    UINT32 i;
    BOOL neg=FALSE;

    if(flags & PUTINT_SIGN)
    {
        if(num & 0x80000000)
        {
            temp2 = (~temp2) + 1;
            neg=TRUE;
        }
    }

    do
    {
        temp1 = (temp1 * base) + (temp2 % base);
        temp2 = temp2 / base;
        digit_count++;
    }
    while(temp2);

    if(flags & PUTINT_HASH)
    {
        if(base==8 || base==16)
            digit_count += 2;
    }

    if(neg || flags & PUTINT_SPACE || flags & PUTINT_PLUS) digit_count += 1;
    
    if(flags & PUTINT_WIDTH)
    {
        if(width >= digit_count)
            pad_count = width-digit_count;
        else
            pad_count=0;
    }

    if(neg)
    {
        KeDebugPutChar('-');
        digit_count--;
    }
    else if(flags & PUTINT_PLUS)
    {
        KeDebugPutChar('+');
        digit_count--;
    }
    else if(flags & PUTINT_SPACE)
    {
        KeDebugPutChar(' ');
        digit_count--;
    }

    if((flags & PUTINT_HASH) && (base==8 || base==16))
    {
        if(!(flags & PUTINT_MINUS) && !(flags & PUTINT_ZERO))
            for(i=0;i<pad_count;i++) KeDebugPutChar(' ');

        if (base==8)
        {
            digit_count-=2;
            if(flags & PUTINT_UCASE)
                KeDebugPutStr("0O",0,0);
            else
                KeDebugPutStr("0o",0,0);
        }
        else if (base==16)
        {
            digit_count-=2;
            if(flags & PUTINT_UCASE)
                KeDebugPutStr("0X",0,0);
            else
                KeDebugPutStr("0x",0,0);
        }

        if(!(flags & PUTINT_MINUS) && (flags & PUTINT_ZERO))
            for(i=0;i<pad_count;i++) KeDebugPutChar('0');
    }
    else
    {
        if(!(flags & PUTINT_MINUS))
        {
            if(flags & PUTINT_ZERO)
                for(i=0;i<pad_count;i++) KeDebugPutChar('0');
            else
                for(i=0;i<pad_count;i++) KeDebugPutChar(' ');
        }
    }

    do
    {
        if(flags & PUTINT_UCASE)
            KeDebugPutChar(DIG_UPPER[temp1 % base]);
        else
            KeDebugPutChar(DIG_LOWER[temp1 % base]);

        temp1 = temp1/base;
        digit_count--;
    }
    while(temp1 || digit_count);

    if(flags & PUTINT_MINUS)
    {
        for(i=0;i<pad_count;i++) KeDebugPutChar(' ');
    }

    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeDebugShowStatus(KESTATUS Status)
{
    switch(Status>>30)
    {
    case 0:
        KeDebugPutStr(MsgTblSuccess[Status & 0xFFFF],0,0);
        break;
    case 1:
        KeDebugPutStr(MsgTblInfo[Status & 0xFFFF],0,0);
        break;
    case 2:
        KeDebugPutStr(MsgTblWarning[Status & 0xFFFF],0,0);
        break;
    case 3:
        KeDebugPutStr(MsgTblError[Status & 0xFFFF],0,0);
        break;
    }
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeDebugParseOptions(PRINTF_OPTIONS *opt,char **c,va_list *ap)
{
    UINT32 j;
    char *str = *c;
    
    opt->Flags = 0;
    opt->Base = 10;
    opt->Width = 0;
    opt->Precision = 0;
    opt->Length = 0;
    opt->Specifier = '\0';

    if(*str=='%')
    {
        str++;
        while(1)
        {
            if(*str=='+')
            {
                opt->Flags |= PRINTF_PLUS;
            }
            else if(*str=='-')
            {
                opt->Flags |= PRINTF_MINUS;
            }
            else if(*str==' ')
            {
                opt->Flags |= PRINTF_SPACE;
            }
            else if(*str=='#')
            {
                opt->Flags |= PRINTF_HASH;
            }
            else if(*str=='0')
            {
                opt->Flags |= PRINTF_ZERO;
            }
            else
            {
                break;
            }
            str++;
        }

        if(*str > '0' && *str <= '9')
        {
            opt->Flags |= PRINTF_WIDTH;
            j=0;
            while(*str >= '0' && *str <= '9')
            {
                j = (j*10) + (*str) - '0';
                str++;
            }
            opt->Width = j;
        }
        else if(*str=='*')
        {
            opt->Flags |= PRINTF_WIDTH;
            opt->Width = va_arg(*ap,UINT32);
            str++;
        }

        if(*str=='.')
        {
            opt->Flags |= PRINTF_PRECISION;
            str++;
            if(*str=='*')
            {
                opt->Precision = va_arg(*ap,UINT32);
                str++;
            }
            else
            {
                j=0;
                while(*str >= '0' && *str <= '9')
                {
                    j = (j*10) + (*str) - '0';
                    str++;
                }
                opt->Precision = j;
            }
        }

        if(*str=='h')
        {
            opt->Flags |= PRINTF_LENGTH_SHORT;
            str++;
        }
        else if(*str=='l')
        {
            opt->Flags |= PRINTF_LENGTH_LONG;
            str++;
        }
        else if(*str=='L')
        {
            opt->Flags |= PRINTF_LENGTH_DLONG;
            str++;
        }

        opt->Specifier = *str;

        switch(opt->Specifier)
        {
        case 'X':
            opt->Flags |= PRINTF_UCASE;
        case 'x':
            opt->Base = 16;
            if(opt->Flags & PRINTF_LENGTH_SHORT)
                opt->Integer = va_arg(*ap,SHORT);
            else
                opt->Integer = va_arg(*ap,INT32);
            break;
        case 'u':
            if(opt->Flags & PRINTF_LENGTH_SHORT)
                opt->Integer = va_arg(*ap,SHORT);
            else
                opt->Integer = va_arg(*ap,INT32);
            break;
        case 'i':
        case 'd':
            opt->Flags |= PRINTF_SIGN;
            if(opt->Flags & PRINTF_LENGTH_SHORT)
                opt->Integer = va_arg(*ap,SHORT);
            else
                opt->Integer = va_arg(*ap,INT32);
            break;
        case 'o':
            opt->Base = 8;
            opt->Flags |= PRINTF_SIGN;
            if(opt->Flags & PRINTF_LENGTH_SHORT)
                opt->Integer = va_arg(*ap,SHORT);
            else
                opt->Integer = va_arg(*ap,INT32);
            break;
        case 'p':
            opt->Base = 16;
            opt->Pointer = va_arg(*ap,VIRTUAL_PTR);
            break;
        case 'e':
        case 'E':
        case 'f':
        case 'g':
        case 'G':
            if(opt->Flags & PRINTF_LENGTH_DLONG)
                opt->LongDouble = va_arg(*ap,long double);
            else
                opt->LongDouble = va_arg(*ap,double);
            break;
        case 'c':
            opt->Char = va_arg(*ap,CHAR);
            break;
        case 's':
            opt->String = va_arg(*ap,CHAR*);
            break;
        case 'n':
            va_arg(*ap,VIRTUAL_PTR);
            break;
        case '%':
            opt->Char = '%';
            opt->Specifier = 'c';
            break;
        default:
            return STATUS_ERROR;
        }

        *c = str;

        return STATUS_SUCCESS;
    }
    return STATUS_ERROR;
}

KESTATUS __cdecl KeDebugPrintf(char *format,...)
{
    va_list ap;
    PRINTF_OPTIONS opt;
    char *s = format;
    va_start(ap,format);
    while (*s)
    {
        if(*s=='%')
        {
            if(KESTATUS_SUCCESS(KeDebugParseOptions(&opt,&s,&ap)))
            {
                switch(opt.Specifier)
                {
                    case 'c':
                        KeDebugPutChar(opt.Char);
                        break;
                    case 'd':
                    case 'i':
                        KeDebugPutInt(opt.Integer,opt.Base,opt.Flags,opt.Width,opt.Precision);
                        break;
                    case 'o':
                        KeDebugPutInt(opt.Integer,opt.Base,opt.Flags,opt.Width,opt.Precision);
                        break;
                    case 's':
                        KeDebugPutStr(opt.String,opt.Flags,opt.Precision);
                        break;
                    case 'u':
                        KeDebugPutInt(opt.Integer,opt.Base,opt.Flags,opt.Width,opt.Precision);
                        break;
                    case 'x':
                        KeDebugPutInt(opt.Integer,opt.Base,opt.Flags,opt.Width,opt.Precision);
                        break;
                    case 'X':
                        KeDebugPutInt(opt.Integer,opt.Base,opt.Flags,opt.Width,opt.Precision);
                        break;
                    case 'p':
                        KeDebugPutInt((UINT32)opt.Pointer,opt.Base,opt.Flags,opt.Width,opt.Precision);
                        break;
                    case 'f':
                        KeDebugPutFloat(opt.LongDouble);
                        break;
                    default:
                        break;
                }
            }
            else
            {
                return STATUS_ERROR;
            }
        }
        else
        {
            KeDebugPutChar(*s);
        }
        s++;
    }
    KeDebugUpdate();
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeDebugConsoleHook(UINT32 Index,void* pfn)
{
    if(Index <= MAX_CON_FUNC)
    {
        ConsoleFunctions[Index] = pfn;
        return STATUS_SUCCESS;
    }
    return STATUS_ERROR;
}

KESTATUS KeDebugPanic2e(UINT32 EDI,UINT32 ESI,UINT32 EBP,UINT32 ESP,UINT32 EBX,UINT32 EDX,UINT32 ECX,UINT32 EAX,UINT32 ErrorCode,UINT32 EIP,UINT32 CS,UINT32 EFLAGS)
{
    KeDebugPrintf("\nRegisters Dump");
    KeDebugPrintf("\nGerneral Purpose Registers");
    KeDebugPrintf("\nEAX : %p, ECX : %p, EDX : %p, EBX : %p, ESX : %p, EBX : %p, ESI : %p, EDI : %p",EAX,ECX,EDX,EBX,ESP,EBP,ESI,EDI);
    _hang();
    return STATUS_SUCCESS;
}

KESTATUS KeDebugPanic2(UINT32 EDI,UINT32 ESI,UINT32 EBP,UINT32 ESP,UINT32 EBX,UINT32 EDX,UINT32 ECX,UINT32 EAX,UINT32 EIP,UINT32 CS,UINT32 EFLAGS)
{
    KeDebugPrintf("\nRegisters Dump");
    KeDebugPrintf("\nGerneral Purpose Registers");
    KeDebugPrintf("\nEAX : %p, ECX : %p, EDX : %p, EBX : %p, ESX : %p, EBX : %p, ESI : %p, EDI : %p",EAX,ECX,EDX,EBX,ESP,EBP,ESI,EDI);
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeDebugSerialWriteChar(CHAR ch)
{
    if(ch=='\n')
    {
        while ((_inp(PORT1+5)&0x20)==0);
        _outp(PORT1,'\r');
        while ((_inp(PORT1+5)&0x20)==0);
        _outp(PORT1,'\n');
    }
    else
    {
        while ((_inp(PORT1+5)&0x20)==0);
        _outp(PORT1,ch);
    }
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeDebugSerialWriteString(CHAR *String)
{
    CHAR *str = String;
    while(*str)
    {
        KeDebugSerialWriteChar(*str);
        str++;
    }
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeDebugSerialWriteHexByte(BYTE Byte)
{
    KeDebugSerialWriteChar(DIG_UPPER[Byte>>4 & 0xF]);
    KeDebugSerialWriteChar(DIG_UPPER[Byte>>0 & 0xF]);
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeDebugSerialWriteHexWord(USHORT Word)
{
    KeDebugSerialWriteChar(DIG_UPPER[Word>>12 & 0xF]);
    KeDebugSerialWriteChar(DIG_UPPER[Word>>8 & 0xF]);
    KeDebugSerialWriteChar(DIG_UPPER[Word>>4 & 0xF]);
    KeDebugSerialWriteChar(DIG_UPPER[Word>>0 & 0xF]);
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeDebugSerialWriteHexDword(UINT32 Dword)
{
    KeDebugSerialWriteChar(DIG_UPPER[Dword>>28 & 0xF]);
    KeDebugSerialWriteChar(DIG_UPPER[Dword>>24 & 0xF]);
    KeDebugSerialWriteChar(DIG_UPPER[Dword>>20 & 0xF]);
    KeDebugSerialWriteChar(DIG_UPPER[Dword>>16 & 0xF]);
    KeDebugSerialWriteChar(DIG_UPPER[Dword>>12 & 0xF]);
    KeDebugSerialWriteChar(DIG_UPPER[Dword>>8 & 0xF]);
    KeDebugSerialWriteChar(DIG_UPPER[Dword>>4 & 0xF]);
    KeDebugSerialWriteChar(DIG_UPPER[Dword>>0 & 0xF]);
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeDebugSerialWriteHexQword(UINT64 Qword)
{
    KeDebugSerialWriteChar(DIG_UPPER[Qword>>60 & 0xF]);
    KeDebugSerialWriteChar(DIG_UPPER[Qword>>56 & 0xF]);
    KeDebugSerialWriteChar(DIG_UPPER[Qword>>52 & 0xF]);
    KeDebugSerialWriteChar(DIG_UPPER[Qword>>48 & 0xF]);
    KeDebugSerialWriteChar(DIG_UPPER[Qword>>44 & 0xF]);
    KeDebugSerialWriteChar(DIG_UPPER[Qword>>40 & 0xF]);
    KeDebugSerialWriteChar(DIG_UPPER[Qword>>36 & 0xF]);
    KeDebugSerialWriteChar(DIG_UPPER[Qword>>32 & 0xF]);
    KeDebugSerialWriteChar(DIG_UPPER[Qword>>28 & 0xF]);
    KeDebugSerialWriteChar(DIG_UPPER[Qword>>24 & 0xF]);
    KeDebugSerialWriteChar(DIG_UPPER[Qword>>20 & 0xF]);
    KeDebugSerialWriteChar(DIG_UPPER[Qword>>16 & 0xF]);
    KeDebugSerialWriteChar(DIG_UPPER[Qword>>12 & 0xF]);
    KeDebugSerialWriteChar(DIG_UPPER[Qword>>8 & 0xF]);
    KeDebugSerialWriteChar(DIG_UPPER[Qword>>4 & 0xF]);
    KeDebugSerialWriteChar(DIG_UPPER[Qword>>0 & 0xF]);
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeDebugSerialHexDump(VIRTUAL_PTR Addr,SIZE_T Size)
{
    UINT32 i;
    BYTE *HeadMsg0 = "\nPreforming Hex Dump of Address 0x";
    BYTE *HeadMsg1 = ", Size 0x";
    BYTE *HeadMsg2 = "\nHex Dump Complete\n";

    __asm pushfd
	__asm cli

    KeDebugSerialWriteString(HeadMsg0);

    KeDebugSerialWriteHexDword((UINT32)Addr);

    KeDebugSerialWriteString(HeadMsg1);

    KeDebugSerialWriteHexDword(Size);

    for(i=0;i<Size;i++)
    {
        if(!(i & 0xF))
        {
            KeDebugSerialWriteChar('\n');
			/*KeDebugSerialWriteHexDword(Addr+i);
			KeDebugSerialWriteChar(':');
			KeDebugSerialWriteChar(' ');*/
        }
        KeDebugSerialWriteHexByte(Addr[i]);
        KeDebugSerialWriteChar(' ');
    }

    KeDebugSerialWriteString(HeadMsg2);

    __asm popfd
}

///////////////////////////////////////////////////////////////////////////////////
//----------------------------------KernelDebug.c--------------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
