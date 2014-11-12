#include <string.h>

wchar_t * __cdecl wcschr(const wchar_t * string, wchar_t ch)
{
    while (*string && *string != (wchar_t)ch)
        string++;
    if (*string == (wchar_t)ch)
        return((wchar_t *)string);
    return(NULL);
}
