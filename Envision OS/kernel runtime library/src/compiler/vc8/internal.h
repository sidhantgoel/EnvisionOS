#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifndef _INC_INTERNAL
#define _INC_INTERNAL

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include <limits.h>


typedef void (__cdecl *_PVFV)(void);
typedef int  (__cdecl *_PIFV)(void);
typedef void (__cdecl *_PVFI)(int);

#if _MSC_VER >= 1400 && defined(_M_CEE)
typedef const void* (__clrcall *_PVFVM)(void);
typedef int (__clrcall *_PIFVM)(void);
typedef void (__clrcall *_CPVFV)(void);
#endif  /* _MSC_VER >= 1400 && defined(_M_CEE) */

int __cdecl _cinit();
void __cdecl _cexit();
int __cdecl atexit(_PVFV fn);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#ifdef _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif  /* _INC_INTERNAL */
