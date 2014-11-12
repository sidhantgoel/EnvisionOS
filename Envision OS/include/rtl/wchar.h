#ifndef WCHAR_H_INCLUDED
#define WCHAR_H_INCLUDED

#ifndef _WCHAR_T_DEFINED
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED
#endif  /* _WCHAR_T_DEFINED */

#ifndef _WCTYPE_T_DEFINED
typedef unsigned short wint_t;
typedef unsigned short wctype_t;
#define _WCTYPE_T_DEFINED
#endif  /* _WCTYPE_T_DEFINED */

#ifndef _VA_LIST_DEFINED
typedef char *  va_list;
#define _VA_LIST_DEFINED
#endif  /* _VA_LIST_DEFINED */

/* set bit masks for the possible character types */
#define _UPPER          0x1     /* upper case letter */
#define _LOWER          0x2     /* lower case letter */
#define _DIGIT          0x4     /* digit[0-9] */
#define _SPACE          0x8     /* tab, carriage return, newline, */
                                /* vertical tab or form feed */
#define _PUNCT          0x10    /* punctuation character */
#define _CONTROL        0x20    /* control character */
#define _BLANK          0x40    /* space char */
#define _HEX            0x80    /* hexadecimal digit */

#define _LEADBYTE       0x8000                  /* multibyte leadbyte */
#define _ALPHA          (0x0100|_UPPER|_LOWER)  /* alphabetic character */

#ifndef _WCTYPE_DEFINED

/* Character classification function prototypes */
/* also declared in ctype.h */

int __cdecl iswalpha(wint_t);
int __cdecl iswupper(wint_t);
int __cdecl iswlower(wint_t);
int __cdecl iswdigit(wint_t);
int __cdecl iswxdigit(wint_t);
int __cdecl iswspace(wint_t);
int __cdecl iswpunct(wint_t);
int __cdecl iswalnum(wint_t);
int __cdecl iswprint(wint_t);
int __cdecl iswgraph(wint_t);
int __cdecl iswcntrl(wint_t);
int __cdecl iswascii(wint_t);
int __cdecl isleadbyte(int);

wchar_t __cdecl towupper(wchar_t);
wchar_t __cdecl towlower(wchar_t);

int __cdecl iswctype(wint_t, wctype_t);

#define _WCTYPE_DEFINED
#endif  /* _WCTYPE_DEFINED */

#endif // WCHAR_H_INCLUDED
