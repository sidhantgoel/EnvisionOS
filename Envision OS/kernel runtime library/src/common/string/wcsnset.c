#include <string.h>

wchar_t * __cdecl wcsnset(wchar_t * string, wchar_t val, size_t count)
{
    wchar_t *start = string;
    while (count-- && *string)
        *string++ = (wchar_t)val;
    return(start);
}
