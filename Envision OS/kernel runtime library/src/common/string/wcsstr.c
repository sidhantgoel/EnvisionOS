#include <string.h>

wchar_t * __cdecl wcsstr(const wchar_t * wcs1, const wchar_t * wcs2)
{
    wchar_t *cp = (wchar_t *) wcs1;
    wchar_t *s1, *s2;

    if ( !*wcs2)
        return (wchar_t *)wcs1;
    while (*cp)
    {
        s1 = cp;
        s2 = (wchar_t *) wcs2;
        while ( *s1 && *s2 && !(*s1-*s2) )
            s1++, s2++;

        if (!*s2)
            return(cp);
        cp++;
    }
    return(NULL);
}
