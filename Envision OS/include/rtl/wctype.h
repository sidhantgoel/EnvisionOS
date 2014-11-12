#ifndef WCTYPE_H_INCLUDED
#define WCTYPE_H_INCLUDED

#ifndef _WCHAR_T_DEFINED
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED
#endif

#ifndef _WCTYPE_T_DEFINED
typedef unsigned short wint_t;
typedef unsigned short wctype_t;
#define _WCTYPE_T_DEFINED
#endif

#ifndef WEOF
#define WEOF (wint_t)(0xFFFF)
#endif


#endif // WCTYPE_H_INCLUDED
