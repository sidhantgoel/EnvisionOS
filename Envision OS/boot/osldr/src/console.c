#include <console.h>
#include <stdarg.h>

#pragma warning(disable : 4311 4312 4313 4018 4013)

USHORT *Buffer;
UINT32 ScreenX,ScreenY;
UINT32 Width,Height,Flags;

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

CHAR DIG_UPPER[] = "0123456789ABCDEF";
CHAR DIG_LOWER[] = "0123456789abcdef";

UINT32 STDCALL TxtClearScreen()
{
    UINT32 i;
    for (i = 0;i<Width*Height;i++) Buffer[i] = Flags|' ';
    ScreenX = 0;
    ScreenY = 0;
    TxtUpdateCursor();
    return 0;
}

UINT32 STDCALL TxtScrollLine()
{
    UINT32 i;
    memcpy(Buffer,Buffer+Width,Width*(Height-1)*2);
    for (i=Width*(Height-1);i<(Width*Height);i++) Buffer[i] = Flags|' ';
    ScreenX = 0;
    TxtUpdateCursor();
    return 0;
}

UINT32 STDCALL TxtPutChar(CHAR ch)
{
    switch (ch)
    {
        case '\r':
            ScreenX=0;
            break;
        case '\n':
            ScreenX=0;
            ScreenY++;
            if (ScreenY==Height)
            {
                ScreenY--;
                TxtScrollLine();
            }
            break;
        case '\t':
            //TODO : Tab Handler
            break;
        case '\b':
            if (ScreenX!=0) ScreenX--;
            break;
        default:
            Buffer[ScreenY*Width+ScreenX] = Flags|ch;
            ScreenX++;
            if (ScreenX==Width)
            {
                ScreenX=0;
                ScreenY++;
                if (ScreenY==Height)
                {
                    ScreenY--;
                    TxtScrollLine();
                }
            }
    }
    TxtUpdateCursor();
    return 0;
}

UINT32 STDCALL TxtUpdateCursor()
{
    USHORT temp = ScreenY*80+ScreenX;
    _outp(0x3D4 + 0, 14);
    _outp(0x3D4 + 1, temp >> 8);
    _outp(0x3D4 + 0, 15);
    _outp(0x3D4 + 1, temp);
    return 0;
}

UINT32 STDCALL ConsoleInit()
{
    Buffer = (USHORT*)0xB8000;
    Width = 80;
    Height = 25;
    Flags = BGCOLOR_RED|FGCOLOR_BRIGHTWHITE;
    TxtClearScreen();
    return 0;
}

UINT32 STDCALL TxtSetFlags(UINT32 tFlags)
{
	Flags = tFlags & 0x0000FF00;
	return 0;
}

UINT32 STDCALL TxtPutStr(char *str,UINT32 flags,INT32 precision)
{
    CHAR *s = str;
    INT32 i;
    if(flags & PUTSTR_PRECISION)   
    {
        while (*s && precision)
        {
            TxtPutChar(*s);
            s++;
            precision--;
        }
        while(precision)
        {
            TxtPutChar(' ');
            precision--;
        }
    }
    else
    {
        while (*s)
        {
            TxtPutChar(*s);
            s++;
        }
    }
    TxtUpdateCursor();
    return 0;
}

UINT32 STDCALL TxtPutFloat(double num)
{
    INT32 a;
    INT32 b;
    
    a = num;
    b = ((num-a) * 10000);
    if(b & 0x80000000) b=-b;

    TxtPutInt(a,10,PUTINT_SIGN,0,0);
    TxtPutChar('.');
    TxtPutInt(b,10,0,0,0);
	return 0;
}

UINT32 STDCALL TxtHexDump(VPTR addr, SIZE_T size)
{
	UINT32 i;
	for(i=0;i<size;i++)
	{
		if(i%16==0) Printf("\n0x%08X: ", addr+i);
		Printf("%02X ", 0xFF & *(UINT32*)(addr+i));
	}
	return 0;
}

UINT32 STDCALL TxtPutInt(UINT32 num,UINT32 base,UINT32 flags,UINT32 width,UINT32 precision)
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
        TxtPutChar('-');
        digit_count--;
    }
    else if(flags & PUTINT_PLUS)
    {
        TxtPutChar('+');
        digit_count--;
    }
    else if(flags & PUTINT_SPACE)
    {
        TxtPutChar(' ');
        digit_count--;
    }

    if((flags & PUTINT_HASH) && (base==8 || base==16))
    {
        if(!(flags & PUTINT_MINUS) && !(flags & PUTINT_ZERO))
            for(i=0;i<pad_count;i++) TxtPutChar(' ');

        if (base==8)
        {
            digit_count-=2;
            if(flags & PUTINT_UCASE)
                TxtPutStr("0O",0,0);
            else
                TxtPutStr("0o",0,0);
        }
        else if (base==16)
        {
            digit_count-=2;
            if(flags & PUTINT_UCASE)
                TxtPutStr("0X",0,0);
            else
                TxtPutStr("0x",0,0);
        }

        if(!(flags & PUTINT_MINUS) && (flags & PUTINT_ZERO))
            for(i=0;i<pad_count;i++) TxtPutChar('0');
    }
    else
    {
        if(!(flags & PUTINT_MINUS))
        {
            if(flags & PUTINT_ZERO)
                for(i=0;i<pad_count;i++) TxtPutChar('0');
            else
                for(i=0;i<pad_count;i++) TxtPutChar(' ');
        }
    }

    do
    {
        if(flags & PUTINT_UCASE)
            TxtPutChar(DIG_UPPER[temp1 % base]);
        else
            TxtPutChar(DIG_LOWER[temp1 % base]);

        temp1 = temp1/base;
        digit_count--;
    }
    while(temp1 || digit_count);

    if(flags & PUTINT_MINUS)
    {
        for(i=0;i<pad_count;i++) TxtPutChar(' ');
    }

    return 0;
}

UINT32 STDCALL PrintfParseOptions(PRINTF_OPTIONS *opt,char **c,va_list *ap)
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

UINT32 __cdecl Printf(char *format,...)
{
    va_list ap;
    PRINTF_OPTIONS opt;
    char *s = format;
    va_start(ap,format);
    while (*s)
    {
        if(*s=='%')
        {
            if(PrintfParseOptions(&opt,&s,&ap)==0)
            {
                switch(opt.Specifier)
                {
                    case 'c':
                        TxtPutChar(opt.Char);
                        break;
                    case 'd':
                    case 'i':
                        TxtPutInt(opt.Integer,opt.Base,opt.Flags,opt.Width,opt.Precision);
                        break;
                    case 'o':
                        TxtPutInt(opt.Integer,opt.Base,opt.Flags,opt.Width,opt.Precision);
                        break;
                    case 's':
                        TxtPutStr(opt.String,opt.Flags,opt.Precision);
                        break;
                    case 'u':
                        TxtPutInt(opt.Integer,opt.Base,opt.Flags,opt.Width,opt.Precision);
                        break;
                    case 'x':
                        TxtPutInt(opt.Integer,opt.Base,opt.Flags,opt.Width,opt.Precision);
                        break;
                    case 'X':
                        TxtPutInt(opt.Integer,opt.Base,opt.Flags,opt.Width,opt.Precision);
                        break;
                    case 'p':
                        TxtPutInt((UINT32)opt.Pointer,opt.Base,opt.Flags,opt.Width,opt.Precision);
                        break;
                    case 'f':
                        TxtPutFloat(opt.LongDouble);
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
            TxtPutChar(*s);
        }
        s++;
    }
    TxtUpdateCursor();
    return 0;
}



