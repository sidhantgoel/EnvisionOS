////////////////////////////////////////////////////////////////////////////////
//----------------------------------rs232.c-----------------------------------//
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
#include <rs232.h>
#include <stdarg.h>

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
        VPTR        Pointer;
    };

}PRINTF_OPTIONS;

#define PORT1 0x3F8
#define PORT2 0x2F8
#define PORT3 0x3E8
#define PORT4 0x2E8

extern CHAR DIG_UPPER[];
extern CHAR DIG_LOWER[];

UINT32 STDCALL SerialInit()
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
	return 0;
}

UINT32 STDCALL SerialWriteBin(VPTR addr, SIZE_T size)
{
	UINT32 i;
	for(i=0; i<size; i++)
	{
		while ((_inp(PORT1+5)&0x20)==0);
        _outp(PORT1,*(addr+i));
	}
	return 0;
}

UINT32 STDCALL SerialWriteChar(CHAR ch)
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
    return 0;
}

UINT32 STDCALL SerialWriteString(CHAR *String)
{
    CHAR *str = String;
    while(*str)
    {
        SerialWriteChar(*str);
        str++;
    }
    return 0;
}

UINT32 STDCALL SerialSendData(BYTE *String, SIZE_T size)
{
    int i=0;
    while(i<size)
    {
        while ((_inp(PORT1+5)&0x20)==0);
        _outp(PORT1,String[i]);
        i++;
    }
    return 0;
}

UINT32 STDCALL SerialWriteHexByte(BYTE Byte)
{
    SerialWriteChar(DIG_UPPER[Byte>>4 & 0xF]);
    SerialWriteChar(DIG_UPPER[Byte>>0 & 0xF]);
    return 0;
}

UINT32 STDCALL SerialWriteHexWord(USHORT Word)
{
    SerialWriteChar(DIG_UPPER[Word>>12 & 0xF]);
    SerialWriteChar(DIG_UPPER[Word>>8 & 0xF]);
    SerialWriteChar(DIG_UPPER[Word>>4 & 0xF]);
    SerialWriteChar(DIG_UPPER[Word>>0 & 0xF]);
    return 0;
}

UINT32 STDCALL SerialWriteHexDword(UINT32 Dword)
{
    SerialWriteChar(DIG_UPPER[Dword>>28 & 0xF]);
    SerialWriteChar(DIG_UPPER[Dword>>24 & 0xF]);
    SerialWriteChar(DIG_UPPER[Dword>>20 & 0xF]);
    SerialWriteChar(DIG_UPPER[Dword>>16 & 0xF]);
    SerialWriteChar(DIG_UPPER[Dword>>12 & 0xF]);
    SerialWriteChar(DIG_UPPER[Dword>>8 & 0xF]);
    SerialWriteChar(DIG_UPPER[Dword>>4 & 0xF]);
    SerialWriteChar(DIG_UPPER[Dword>>0 & 0xF]);
    return 0;
}

UINT32 STDCALL SerialWriteHexQword(UINT64 Qword)
{
    SerialWriteChar(DIG_UPPER[Qword>>60 & 0xF]);
    SerialWriteChar(DIG_UPPER[Qword>>56 & 0xF]);
    SerialWriteChar(DIG_UPPER[Qword>>52 & 0xF]);
    SerialWriteChar(DIG_UPPER[Qword>>48 & 0xF]);
    SerialWriteChar(DIG_UPPER[Qword>>44 & 0xF]);
    SerialWriteChar(DIG_UPPER[Qword>>40 & 0xF]);
    SerialWriteChar(DIG_UPPER[Qword>>36 & 0xF]);
    SerialWriteChar(DIG_UPPER[Qword>>32 & 0xF]);
    SerialWriteChar(DIG_UPPER[Qword>>28 & 0xF]);
    SerialWriteChar(DIG_UPPER[Qword>>24 & 0xF]);
    SerialWriteChar(DIG_UPPER[Qword>>20 & 0xF]);
    SerialWriteChar(DIG_UPPER[Qword>>16 & 0xF]);
    SerialWriteChar(DIG_UPPER[Qword>>12 & 0xF]);
    SerialWriteChar(DIG_UPPER[Qword>>8 & 0xF]);
    SerialWriteChar(DIG_UPPER[Qword>>4 & 0xF]);
    SerialWriteChar(DIG_UPPER[Qword>>0 & 0xF]);
    return 0;
}

UINT32 STDCALL SerialHexDump(VPTR Addr,SIZE_T Size)
{
    UINT32 i;
    SerialWriteString("\nPreforming Hex Dump of Address 0x");
    SerialWriteHexDword((UINT32)Addr);
    SerialWriteString(", Size 0x");
    SerialWriteHexDword(Size);
    for(i=0;i<Size;i++)
    {
        if(!(i & 0xF))
        {
			SerialWriteString("\n0x");
			SerialWriteHexDword((UINT32)Addr+i);
			SerialWriteString(": ");
        }
        SerialWriteHexByte(Addr[i]);
        SerialWriteChar(' ');
    }
    SerialWriteString("\nHex Dump Complete\n");
	return 0;
}

UINT32 STDCALL SerialPrintfParseOptions(PRINTF_OPTIONS *opt,char **c,va_list *ap)
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
            opt->Pointer = va_arg(*ap,VPTR);
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
            va_arg(*ap,VPTR);
            break;
        case '%':
            opt->Char = '%';
            opt->Specifier = 'c';
            break;
        default:
            return 1;
        }

        *c = str;

        return 0;
    }
    return 1;
}

UINT32 STDCALL SerialPrintfWriteInt(UINT32 num,UINT32 base,UINT32 flags,UINT32 width,UINT32 precision);
UINT32 STDCALL SerialPrintfWriteStr(char *str,UINT32 flags,INT32 precision);
UINT32 STDCALL SerialPrintfWriteFloat(double num);

UINT32 __cdecl SerialPrintf(char *format,...)
{
    va_list ap;
    PRINTF_OPTIONS opt;
    char *s = format;
    va_start(ap,format);
    while (*s)
    {
        if(*s=='%')
        {
            if(SerialPrintfParseOptions(&opt,&s,&ap)==0)
            {
                switch(opt.Specifier)
                {
                    case 'c':
                        SerialWriteChar(opt.Char);
                        break;
                    case 'd':
                    case 'i':
                        SerialPrintfWriteInt(opt.Integer,opt.Base,opt.Flags,opt.Width,opt.Precision);
                        break;
                    case 'o':
                        SerialPrintfWriteInt(opt.Integer,opt.Base,opt.Flags,opt.Width,opt.Precision);
                        break;
                    case 's':
                        SerialPrintfWriteStr(opt.String,opt.Flags,opt.Precision);
                        break;
                    case 'u':
                        SerialPrintfWriteInt(opt.Integer,opt.Base,opt.Flags,opt.Width,opt.Precision);
                        break;
                    case 'x':
                        SerialPrintfWriteInt(opt.Integer,opt.Base,opt.Flags,opt.Width,opt.Precision);
                        break;
                    case 'X':
                        SerialPrintfWriteInt(opt.Integer,opt.Base,opt.Flags,opt.Width,opt.Precision);
                        break;
                    case 'p':
                        SerialPrintfWriteInt((UINT32)opt.Pointer,opt.Base,opt.Flags,opt.Width,opt.Precision);
                        break;
                    case 'f':
                        SerialPrintfWriteFloat(opt.LongDouble);
                        break;
                    default:
                        break;
                }
            }
            else
            {
                return 1;
            }
        }
        else
        {
            SerialWriteChar(*s);
        }
        s++;
    }
    return 0;
}

UINT32 STDCALL SerialPrintfWriteInt(UINT32 num,UINT32 base,UINT32 flags,UINT32 width,UINT32 precision)
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
        SerialWriteChar('-');
        digit_count--;
    }
    else if(flags & PUTINT_PLUS)
    {
        SerialWriteChar('+');
        digit_count--;
    }
    else if(flags & PUTINT_SPACE)
    {
        SerialWriteChar(' ');
        digit_count--;
    }

    if((flags & PUTINT_HASH) && (base==8 || base==16))
    {
        if(!(flags & PUTINT_MINUS) && !(flags & PUTINT_ZERO))
            for(i=0;i<pad_count;i++) SerialWriteChar(' ');

        if (base==8)
        {
            digit_count-=2;
            if(flags & PUTINT_UCASE)
                SerialPrintfWriteStr("0O",0,0);
            else
                SerialPrintfWriteStr("0o",0,0);
        }
        else if (base==16)
        {
            digit_count-=2;
            if(flags & PUTINT_UCASE)
                SerialPrintfWriteStr("0X",0,0);
            else
                SerialPrintfWriteStr("0x",0,0);
        }

        if(!(flags & PUTINT_MINUS) && (flags & PUTINT_ZERO))
            for(i=0;i<pad_count;i++) SerialWriteChar('0');
    }
    else
    {
        if(!(flags & PUTINT_MINUS))
        {
            if(flags & PUTINT_ZERO)
                for(i=0;i<pad_count;i++) SerialWriteChar('0');
            else
                for(i=0;i<pad_count;i++) SerialWriteChar(' ');
        }
    }

    do
    {
        if(flags & PUTINT_UCASE)
            SerialWriteChar(DIG_UPPER[temp1 % base]);
        else
            SerialWriteChar(DIG_LOWER[temp1 % base]);

        temp1 = temp1/base;
        digit_count--;
    }
    while(temp1 || digit_count);

    if(flags & PUTINT_MINUS)
    {
        for(i=0;i<pad_count;i++) SerialWriteChar(' ');
    }

    return 0;
}

UINT32 STDCALL SerialPrintfWriteStr(char *str,UINT32 flags,INT32 precision)
{
    CHAR *s = str;
    INT32 i;
    if(flags & PUTSTR_PRECISION)   
    {
        while (*s && precision)
        {
            SerialWriteChar(*s);
            s++;
            precision--;
        }
        while(precision)
        {
            SerialWriteChar(' ');
            precision--;
        }
    }
    else
    {
        while (*s)
        {
            SerialWriteChar(*s);
            s++;
        }
    }
    return 0;
}

UINT32 STDCALL SerialPrintfWriteFloat(double num)
{
    INT32 a;
    INT32 b;
    
    a = num;
    b = ((num-a) * 10000);
    if(b & 0x80000000) b=-b;

    SerialPrintfWriteInt(a,10,PUTINT_SIGN,0,0);
    SerialWriteChar('.');
    SerialPrintfWriteInt(b,10,0,0,0);
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//----------------------------------rs232.c-----------------------------------//
//-----------------------Copyright © Sidhant Goel, 2008-----------------------//
////////////////////////////////////////////////////////////////////////////////
