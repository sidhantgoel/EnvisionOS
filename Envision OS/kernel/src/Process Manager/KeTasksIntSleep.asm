;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;-------------------------------KeTasksIntSleep.asm-----------------------------;;
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

public _KeTasksIntSleep
extern _KeTasksNextThread@4:proc

.code _TEXT

_KeTasksIntSleep:
    cli
    pushad
    push ds
    push es
    push fs
    push gs
    mov eax,cr3
    push eax
    push esp
    call _KeTasksNextThread@4
    mov esp,eax
    pop eax
    mov cr3,eax
    pop gs
    pop fs
    pop es
    pop ds
    popad
    iretd

_TEXT ends
end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;-------------------------------KeTasksIntSleep.asm-----------------------------;;
;;--------------------------Copyright © Sidhant Goel, 2007-----------------------;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
