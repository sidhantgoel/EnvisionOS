#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

#ifndef _SIZE_T_DEFINED
#ifdef _BUILD64
typedef unsigned __int64    size_t;
#else  /* _WIN64 */
typedef unsigned int        size_t;
#endif  /* _WIN64 */
#define _SIZE_T_DEFINED
#endif  /* _SIZE_T_DEFINED */

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else  /* __cplusplus */
#define NULL    ((void *)0)
#endif  /* __cplusplus */
#endif  /* NULL */

void * __cdecl memchr(const void * buf, int chr, size_t cnt);
int __cdecl memcmp(const void * buf1, const void * buf2, size_t count);
void * __cdecl memcpy(void * dst, const void * src, size_t count);
void * __cdecl memmove(void * dst, const void * src, size_t count);
void * __cdecl memset(void *dst, int val, size_t count);

#endif // MEMORY_H_INCLUDED
