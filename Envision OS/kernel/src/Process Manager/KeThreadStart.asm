;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;--------------------------------KeThreadStart.asm------------------------------;;
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

public _KeThreadStart

extern _KeDebugPrintf:proc
extern __hang:proc

.code _TEXT

_KeThreadStart:
    ;jmp $;
    mov eax,[esp]
    mov edx,[eax+4]
    mov ebx,[eax+8]
    push ebx
    call edx
    mov ecx,[esp]
    mov [ecx+10h],eax
    push eax
    push offset MSG_FINISHED
    call _KeDebugPrintf
    add esp,8
    call __hang
    retn

_TEXT ends

.data

MSG_FINISHED    DB    0Ah,"Thread Finished with Exit Code : %i",0

_DATA ends
end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;--------------------------------KeThreadStart.asm------------------------------;;
;;--------------------------Copyright © Sidhant Goel, 2007-----------------------;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
