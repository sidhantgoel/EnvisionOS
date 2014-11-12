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

extern _KeDebugPrintf:near

.code _TEXT

public _KeHandlerIRQ0
_KeHandlerIRQ0:
    pushad
    ;push offset MSG_00
    ;call _KeDebugPrintf
    ;add esp,4
    mov dx,20h
    mov al,20h
    out dx,al
    popad
    iretd
    

public _KeHandlerIRQ1
_KeHandlerIRQ1:
    pushad
    push offset MSG_01
    call _KeDebugPrintf
    add esp,4
    mov dx,20h
    mov al,20h
    out dx,al
    popad
    iretd
    

public _KeHandlerIRQ2
_KeHandlerIRQ2:
    pushad
    push offset MSG_02
    call _KeDebugPrintf
    add esp,4
    mov dx,20h
    mov al,20h
    out dx,al
    popad
    iretd
    
    
public _KeHandlerIRQ3
_KeHandlerIRQ3:
    pushad
    push offset MSG_03
    call _KeDebugPrintf
    add esp,4
    mov dx,20h
    mov al,20h
    out dx,al
    popad
    iretd
    
    
public _KeHandlerIRQ4
_KeHandlerIRQ4:
    pushad
    push offset MSG_04
    call _KeDebugPrintf
    add esp,4
    mov dx,20h
    mov al,20h
    out dx,al
    popad
    iretd
    
public _KeHandlerIRQ5
_KeHandlerIRQ5:
    pushad
    push offset MSG_05
    call _KeDebugPrintf
    add esp,4
    mov dx,20h
    mov al,20h
    out dx,al
    popad
    iretd
    
    
public _KeHandlerIRQ6
_KeHandlerIRQ6:
    pushad
    push offset MSG_06
    call _KeDebugPrintf
    add esp,4
    mov dx,20h
    mov al,20h
    out dx,al
    popad
    iretd
    
    
public _KeHandlerIRQ7
_KeHandlerIRQ7:
    pushad
    push offset MSG_07
    call _KeDebugPrintf
    add esp,4
    mov dx,20h
    mov al,20h
    out dx,al
    popad
    iretd


public _KeHandlerIRQ8
_KeHandlerIRQ8:
    pushad
    push offset MSG_08
    call _KeDebugPrintf
    add esp,4
    mov dx,0A0h
    mov al,20h
    out dx,al
    mov dx,20h
    out dx,al
    popad
    iretd
    
    
public _KeHandlerIRQ9
_KeHandlerIRQ9:
    pushad
    push offset MSG_09
    call _KeDebugPrintf
    add esp,4
    mov dx,0A0h
    mov al,20h
    out dx,al
    mov dx,20h
    out dx,al
    popad
    iretd
    
public _KeHandlerIRQ10
_KeHandlerIRQ10:
    pushad
    push offset MSG_10
    call _KeDebugPrintf
    add esp,4
    mov dx,0A0h
    mov al,20h
    out dx,al
    mov dx,20h
    out dx,al
    popad
    iretd
    
public _KeHandlerIRQ11
_KeHandlerIRQ11:
    pushad
    push offset MSG_11
    call _KeDebugPrintf
    add esp,4
    mov dx,0A0h
    mov al,20h
    out dx,al
    mov dx,20h
    out dx,al
    popad
    iretd
    
    
public _KeHandlerIRQ12
_KeHandlerIRQ12:
    pushad
    push offset MSG_12
    call _KeDebugPrintf
    add esp,4
    mov dx,0A0h
    mov al,20h
    out dx,al
    mov dx,20h
    out dx,al
    popad
    iretd
    
public _KeHandlerIRQ13
_KeHandlerIRQ13:
    pushad
    push offset MSG_13
    call _KeDebugPrintf
    add esp,4
    mov dx,0A0h
    mov al,20h
    out dx,al
    mov dx,20h
    out dx,al
    popad
    iretd
    
public _KeHandlerIRQ14
_KeHandlerIRQ14:
    pushad
    push offset MSG_14
    call _KeDebugPrintf
    add esp,4
    mov dx,0A0h
    mov al,20h
    out dx,al
    mov dx,20h
    out dx,al
    popad
    iretd
    
    
public _KeHandlerIRQ15
_KeHandlerIRQ15:
    pushad
    push offset MSG_15
    call _KeDebugPrintf
    add esp,4
    mov dx,0A0h
    mov al,20h
    out dx,al
    mov dx,20h
    out dx,al
    popad
    iretd
    
_TEXT ends

.data

MSG_00  db  0Ah,"IRQ 00 Fired",0
MSG_01  db  0Ah,"IRQ 01 Fired",0
MSG_02  db  0Ah,"IRQ 02 Fired",0
MSG_03  db  0Ah,"IRQ 03 Fired",0
MSG_04  db  0Ah,"IRQ 04 Fired",0
MSG_05  db  0Ah,"IRQ 05 Fired",0
MSG_06  db  0Ah,"IRQ 06 Fired",0
MSG_07  db  0Ah,"IRQ 07 Fired",0
MSG_08  db  0Ah,"IRQ 08 Fired",0
MSG_09  db  0Ah,"IRQ 09 Fired",0
MSG_10  db  0Ah,"IRQ 10 Fired",0
MSG_11  db  0Ah,"IRQ 11 Fired",0
MSG_12  db  0Ah,"IRQ 12 Fired",0
MSG_13  db  0Ah,"IRQ 13 Fired",0
MSG_14  db  0Ah,"IRQ 14 Fired",0
MSG_15  db  0Ah,"IRQ 15 Fired",0

_DATA ends

end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;---------------------------KernelExceptionHandlers.asm-------------------------;;
;;--------------------------Copyright © Sidhant Goel, 2007-----------------------;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
