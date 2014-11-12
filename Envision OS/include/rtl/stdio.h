#ifndef STDIO_H_INCLUDED
#define STDIO_H_INCLUDED

#ifndef _SIZE_T_DEFINED
#ifdef _BUILD64
typedef unsigned __int64    size_t;
#else  /* _WIN64 */
typedef unsigned int        size_t;
#endif  /* _WIN64 */
#define _SIZE_T_DEFINED
#endif  /* _SIZE_T_DEFINED */

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

#endif // STDIO_H_INCLUDED
