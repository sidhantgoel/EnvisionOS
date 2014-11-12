;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;---------------------------KernelExceptionHandlers.asm-------------------------;;
;;                                      v 1.0                                    ;;
;;                                                                               ;;
;;                                                                               ;;
;;                                                                               ;;
;;                                                                               ;;
;;                                                                               ;;
;;                                                                               ;;
;; Created by: Sidhant Goel                                                      ;;
;; Created on: 21 march 2007                                                     ;;
;;                                                                               ;;
;;--------------------------------------Notes------------------------------------;;
;;                                                                               ;;
;;                                                                               ;;
;;                                                                               ;;
;;                                                                               ;;
;;                                                                               ;;
;;                                                                               ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.586
.model flat

extern _KeDebugPrintf:proc
extern _KePageFault@4:proc
extern __hang:proc

.code _TEXT

public _KeHandlerDE
_KeHandlerDE:
    pushad
    push offset MSG_DE
    call _KeDebugPrintf
    add esp,4
    call __hang
    popad
    iretd

public _KeHandlerDB
_KeHandlerDB:
    pushad
    push offset MSG_DB
    call _KeDebugPrintf
    add esp,4
    call __hang
    popad
    iretd

public _KeHandlerBP
_KeHandlerBP:
    pushad
    push offset MSG_BP
    call _KeDebugPrintf
    add esp,4
    popad
    iretd

public _KeHandlerOF
_KeHandlerOF:
    pushad
    push offset MSG_OF
    call _KeDebugPrintf
    add esp,4
    call __hang
    popad
    iretd

public _KeHandlerBR
_KeHandlerBR:
    pushad
    push offset MSG_BR
    call _KeDebugPrintf
    add esp,4
    call __hang
    popad
    iretd

public _KeHandlerUD
_KeHandlerUD:
    pushad
    push offset MSG_UD
    call _KeDebugPrintf
    add esp,4
    call __hang
    popad
    iretd

public _KeHandlerNM
_KeHandlerNM:
    pushad
    push offset MSG_NM
    call _KeDebugPrintf
    add esp,4
    call __hang
    popad
    iretd

public _KeHandlerDF
_KeHandlerDF:
    pushad
    push offset MSG_DF
    call _KeDebugPrintf
    add esp,4
    call __hang
    popad
    add esp,4
    iretd

public _KeHandlerTS
_KeHandlerTS:
    pushad
    push offset MSG_TS
    call _KeDebugPrintf
    add esp,4
    call __hang
    popad
    add esp,4
    iretd

public _KeHandlerNP
_KeHandlerNP:
    pushad
    push offset MSG_NP
    call _KeDebugPrintf
    add esp,4
    call __hang
    popad
    add esp,4
    iretd

public _KeHandlerSS
_KeHandlerSS:
    pushad
    push offset MSG_SS
    call _KeDebugPrintf
    add esp,4
    call __hang
    popad
    add esp,4
    iretd

public _KeHandlerGP
_KeHandlerGP:
    pushad
    push offset MSG_GP
    call _KeDebugPrintf
    add esp,4
    call __hang
    popad
    add esp,4
    iretd

public _KeHandlerPF
_KeHandlerPF:
    pushad
    push offset MSG_PF_EXT
    call _KeDebugPrintf
    add esp,4
    mov eax,cr2
    push eax
    call _KePageFault@4
    popad
    add esp,4
    iretd

public _KeHandlerMF
_KeHandlerMF:
    pushad
    push offset MSG_MF
    call _KeDebugPrintf
    add esp,4
    call __hang
    popad
    iretd

public _KeHandlerAC
_KeHandlerAC:
    pushad
    push offset MSG_AC
    call _KeDebugPrintf
    add esp,4
    call __hang
    popad
    add esp,4
    iretd

public _KeHandlerMC
_KeHandlerMC:
    pushad
    push offset MSG_MC
    call _KeDebugPrintf
    add esp,4
    call __hang
    popad
    iretd

public _KeHandlerXF
_KeHandlerXF:
    pushad
    push offset MSG_XF
    call _KeDebugPrintf
    add esp,4
    call __hang
    popad
    iretd
    
_TEXT ends

.data

MSG_DE  db  0Ah,"Divide Error Exception",0
MSG_DB  db  0Ah,"Debug Exception %X %X %X %X %X %X %X %X %X %X %X",0
MSG_BP  db  0Ah,"Breakpoint Exception",0
MSG_OF  db  0Ah,"Overflow Exception",0
MSG_BR  db  0Ah,"BOUND Range Exceed Exception",0
MSG_UD  db  0Ah,"Invalid Opcode Exception",0
MSG_NM  db  0Ah,"Device Not Available Exception",0
MSG_DF  db  0Ah,"Double Fault Exception",0
MSG_TS  db  0Ah,"Invalid TSS Exception",0
MSG_NP  db  0Ah,"Segment Not Present",0
MSG_SS  db  0Ah,"Stack Fault Exception",0
MSG_GP  db  0Ah,"General Protection Exception",0
MSG_PF  db  0Ah,"Page Fault Exception",0
MSG_MF  db  0Ah,"FPU Floating Point Error",0
MSG_AC  db  0Ah,"Alignment Check Exception",0
MSG_MC  db  0Ah,"Machine Check Exception",0
MSG_XF  db  0Ah,"SIMD Floating Point Exception",0

MSG_PF_EXT db 0Ah,"PF_EXT %X %X %X %X %X %X %X %X %X %X %X",0

_DATA ends

end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;---------------------------KernelExceptionHandlers.asm-------------------------;;
;;--------------------------Copyright © Sidhant Goel, 2007-----------------------;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
