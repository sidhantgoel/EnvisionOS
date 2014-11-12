#include <string.h>

wchar_t * __cdecl wcsset(wchar_t * string, wchar_t val)
{
    wchar_t *start = string;
    while (*string)
        *string++ = (wchar_t)val;
    return(start);
}
