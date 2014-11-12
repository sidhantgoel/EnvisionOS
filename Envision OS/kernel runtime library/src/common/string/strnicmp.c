#include <string.h>

int __cdecl strnicmp(const char * first, const char * last, size_t count)
{
    int f, l;
    do
    {
        if ( ((f = (unsigned char)(*(first++))) >= 'A') &&
                (f <= 'Z') )
            f -= 'A' - 'a';
        if ( ((l = (unsigned char)(*(last++))) >= 'A') &&
                (l <= 'Z') )
            l -= 'A' - 'a';
    }
    while ( --count && f && (f == l) );
    return ( f - l );
}
