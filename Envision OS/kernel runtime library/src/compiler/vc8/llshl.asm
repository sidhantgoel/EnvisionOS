.xlist
include cruntime.inc
include mm.inc
.list

        CODESEG

_allshl PROC NEAR

;
; Handle shifts of 64 or more bits (all get 0)
;
        cmp     cl, 64
        jae     short RETZERO

;
; Handle shifts of between 0 and 31 bits
;
        cmp     cl, 32
        jae     short MORE32
        shld    edx,eax,cl
        shl     eax,cl
        ret

;
; Handle shifts of between 32 and 63 bits
;
MORE32:
        mov     edx,eax
        xor     eax,eax
        and     cl,31
        shl     edx,cl
        ret

;
; return 0 in edx:eax
;
RETZERO:
        xor     eax,eax
        xor     edx,edx
        ret

_allshl ENDP

        end
