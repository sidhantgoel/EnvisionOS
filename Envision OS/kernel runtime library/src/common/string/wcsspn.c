#include <string.h>

size_t __cdecl wcsspn(const wchar_t * string, const wchar_t * control)
{
    wchar_t *str = (wchar_t *) string;
    wchar_t *ctl;
    /* 1st char not in control string stops search */
    while (*str)
    {
        for (ctl = (wchar_t *)control; *ctl != *str; ctl++)
        {
            if (*ctl == (wchar_t)0)
            {
                /*
                 * reached end of control string without finding a match
                 */
                return (size_t)(str - string);
            }
        }
        str++;
    }
    /*
     * The whole string consisted of characters from control
     */
    return (size_t)(str - string);
}
