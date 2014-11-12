        .xlist
        include cruntime.inc
        .list

        CODESEG

        public _inp, _inpw, _inpd

_inp    proc

        xor     eax,eax
        mov     dx,word ptr [esp + 4]
        in      al,dx
        ret

_inp    endp


_inpw   proc

        mov     dx,word ptr [esp + 4]
        in      ax,dx
        ret

_inpw   endp

_inpd   proc

        mov     dx,word ptr [esp + 4]
        in      eax,dx
        ret

_inpd   endp

        end
