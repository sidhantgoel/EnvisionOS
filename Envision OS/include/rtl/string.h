#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED

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

#ifndef _NLSCMP_DEFINED
#define _NLSCMPERROR    2147483647  /* currently == INT_MAX */
#define _NLSCMP_DEFINED
#endif  /* _NLSCMP_DEFINED */

/* Define NULL pointer value */

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else  /* __cplusplus */
#define NULL    ((void *)0)
#endif  /* __cplusplus */
#endif  /* NULL */

char * __cdecl strcat(char * dst, const char * src);
char * __cdecl strchr(const char * string,int ch);
int __cdecl strcmp(const char * src, const char * dst);
char * __cdecl strcpy(char * dst, const char * src);
size_t __cdecl strcspn(const char * string, const char * control);
int __cdecl stricmp(const char * dst, const char * src);
size_t __cdecl strlen(const char * str);
char * __cdecl strncat(char * front, const char * back, size_t count);
int __cdecl strncmp(const char * first, const char * last, size_t count);
char * __cdecl strncpy(char * dest, const char * source, size_t count);
int __cdecl strnicmp(const char * first, const char * last, size_t count);
char * __cdecl strnset(char * string, int val, size_t count);
char * __cdecl strpbrk(const char * string, const char * control);
char * __cdecl strrchr(const char * string, int ch);
char * __cdecl strrev(char * string);
char * __cdecl strset(char * string, int val);
size_t __cdecl strspn(const char * string, const char * control);
char * __cdecl strstr(const char * str1, const char * str2);

wchar_t * __cdecl wcscat(wchar_t * dst, const wchar_t * src);
wchar_t * __cdecl wcschr(const wchar_t * string, wchar_t ch);
int __cdecl wcscmp(const wchar_t * src, const wchar_t * dst);
wchar_t * __cdecl wcscpy(wchar_t * dst, const wchar_t * src);
size_t __cdecl wcslen(const wchar_t * wcs);
wchar_t * __cdecl wcsncat(wchar_t * front, const wchar_t * back, size_t count);
int __cdecl wcsncmp(const wchar_t * first, const wchar_t * last, size_t count);
wchar_t * __cdecl wcsnset(wchar_t * string, wchar_t val, size_t count);
wchar_t * __cdecl wcsrchr(const wchar_t * string, wchar_t ch);
wchar_t * __cdecl wcsrev(wchar_t * string);
wchar_t * __cdecl wcsset(wchar_t * string, wchar_t val);
size_t __cdecl wcsspn(const wchar_t * string, const wchar_t * control);
wchar_t * __cdecl wcsstr(const wchar_t * wcs1, const wchar_t * wcs2);

#endif // STRING_H_INCLUDED
