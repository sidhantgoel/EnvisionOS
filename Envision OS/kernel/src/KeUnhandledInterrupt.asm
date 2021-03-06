;**********************************************************************************
;*----------------------------KeUnhandledInterrupt.asm---------------------------**
;*                                      v 1.0                                    **
;*                                                                               **
;*                                                                               **
;*                                                                               **
;*                                                                               **
;*                                                                               **
;*                                                                               **
;* Created by: Sidhant Goel                                                      **
;* Created on: 21 march 2007                                                     **
;*                                                                               **
;*--------------------------------------Notes------------------------------------**
;*                                                                               **
;*                                                                               **
;*                                                                               **
;*                                                                               **
;*                                                                               **
;*                                                                               **
;**********************************************************************************

.586
.model flat

public _KeUnhandledInterrupt

extern _KeDebugPrintf:proc

.code _TEXT

_KeUnhandledInterrupt:
    pushad
    push offset MSG_UI
    call _KeDebugPrintf
    add esp,4
    popad
    iretd

_TEXT ends

.data
MSG_UI  db  0Ah,"Unhandled Interrupt",0

_DATA ends
end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;----------------------------KeUnhandledInterrupt.asm---------------------------;;
;;--------------------------Copyright � Sidhant Goel, 2007-----------------------;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
