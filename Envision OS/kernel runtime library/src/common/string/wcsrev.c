#include <string.h>

wchar_t * __cdecl wcsrev(wchar_t * string)
{
    wchar_t *start = string;
    wchar_t *left = string;
    wchar_t ch;
    while (*string++)                 /* find end of string */
        ;
    string -= 2;
    while (left < string)
    {
        ch = *left;
        *left++ = *string;
        *string-- = ch;
    }
    return(start);
}
