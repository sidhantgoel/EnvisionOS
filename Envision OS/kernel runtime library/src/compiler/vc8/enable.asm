        .xlist
        include cruntime.inc
        .list

        CODESEG

        public  _enable, _disable

_enable proc

        sti
        ret

_enable endp

        align   4

_disable proc

        cli
        ret

_disable endp


        end
