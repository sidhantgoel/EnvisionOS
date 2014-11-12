        .xlist
        include cruntime.inc
        .list

        CODESEG

        public _outp, _outpw, _outpd

_outp   proc

        xor     eax,eax
        mov     dx,word ptr [esp + 4]
        mov     al,byte ptr [esp + 8]
        out     dx,al
        ret

_outp   endp


_outpw  proc

        mov     dx,word ptr [esp + 4]
        mov     ax,word ptr [esp + 8]
        out     dx,ax
        ret

_outpw  endp


_outpd  proc

        mov     dx,word ptr [esp + 4]
        mov     eax,[esp + 8]
        out     dx,eax
        ret

_outpd  endp

        end
