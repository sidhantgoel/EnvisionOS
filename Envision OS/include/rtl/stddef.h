#ifndef STDDEF_H_INCLUDED
#define STDDEF_H_INCLUDED

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else  /* __cplusplus */
#define NULL    ((void *)0)
#endif  /* __cplusplus */
#endif  /* NULL */

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

#ifndef _INTPTR_T_DEFINED
#ifdef _BUILD64
typedef __int64             intptr_t;
#else  /* _WIN64 */
typedef int                 intptr_t;
#endif  /* _WIN64 */
#define _INTPTR_T_DEFINED
#endif  /* _INTPTR_T_DEFINED */

#ifndef _UINTPTR_T_DEFINED
#ifdef _BUILD64
typedef unsigned __int64    uintptr_t;
#else  /* _WIN64 */
typedef unsigned int        uintptr_t;
#endif  /* _WIN64 */
#define _UINTPTR_T_DEFINED
#endif  /* _UINTPTR_T_DEFINED */

#ifndef _PTRDIFF_T_DEFINED
#ifdef _BUILD64
typedef __int64             ptrdiff_t;
#else  /* _WIN64 */
typedef int                 ptrdiff_t;
#endif  /* _WIN64 */
#define _PTRDIFF_T_DEFINED
#endif  /* _PTRDIFF_T_DEFINED */

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

#endif // STDDEF_H_INCLUDED
