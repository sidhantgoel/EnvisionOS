#ifndef STDLIB_H_INCLUDED
#define STDLIB_H_INCLUDED

#include <stddef.h>
#include <stdbool.h>
#include <memory.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <stdarg.h>
#include <conio.h>
#include <dos.h>

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


/* Define NULL pointer value */

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else  /* __cplusplus */
#define NULL    ((void *)0)
#endif  /* __cplusplus */
#endif  /* NULL */

#endif // STDLIB_H_INCLUDED
