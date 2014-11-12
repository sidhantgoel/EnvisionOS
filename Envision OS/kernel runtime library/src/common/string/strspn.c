#include <string.h>

size_t __cdecl strspn(const char * string, const char * control)
{
        const unsigned char *str = string;
        const unsigned char *ctrl = control;

        unsigned char map[32];
        int count;

        /* Clear out bit map */
        for (count=0; count<32; count++)
                map[count] = 0;

        /* Set bits in control map */
        while (*ctrl)
        {
                map[*ctrl >> 3] |= (1 << (*ctrl & 7));
                ctrl++;
        }

        /* 1st char NOT in control map stops search */
        if (*str)
        {
                count=0;
                while (map[*str >> 3] & (1 << (*str & 7)))
                {
                        count++;
                        str++;
                }
                return(count);
        }
        return(0);
}
