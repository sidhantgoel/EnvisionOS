#include <string.h>

wchar_t * __cdecl wcsrchr(const wchar_t * string, wchar_t ch)
{
    wchar_t *start = (wchar_t *)string;
    while (*string++)                       /* find end of string */
        ;
    /* search towards front */
    while (--string != start && *string != (wchar_t)ch)
        ;
    if (*string == (wchar_t)ch)             /* wchar_t found ? */
        return( (wchar_t *)string );
    return(NULL);
}
