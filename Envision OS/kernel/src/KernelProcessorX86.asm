;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;-----------------------------KernelProcessorX86.asm----------------------------;;
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

.686p
.model flat

public __read_cr0
public __read_cr2
public __read_cr3
public __read_cr4
public __set_cr0@4
public __set_cr2@4
public __set_cr3@4
public __set_cr4@4
public __hang
public __cpuid@4
public __lidt@4
;public __lldt@4
public __lgdt@4

.code _TEXT

__lidt@4:
	mov eax,[esp+4]
	lidt fword ptr [eax]
	retn 4
	
;__lldt@4:
;	mov eax,[esp+4]
;	lldt qword ptr [eax]
;	retn 4
	
__lgdt@4:
	mov eax,[esp+4]
	lgdt fword ptr [eax]
	retn 4

__cpuid@4:
    push edi
    mov edi,[esp+8]
    mov eax,[edi+00h]
    cpuid
    mov [edi+00h],eax
    mov [edi+04h],ebx
    mov [edi+08h],ecx
    mov [edi+0Ch],edx
    pop edi
    retn 4

__read_cr0:
    mov eax,cr0
    retn

__read_cr2:
    mov eax,cr2
    retn

__read_cr3:
    mov eax,cr3
    retn

__read_cr4:
    mov eax,cr4
    retn

__set_cr0@4:
    mov eax,[esp+4]
    mov cr0,eax
    retn 4

__set_cr2@4:
    mov eax,[esp+4]
    mov cr2,eax
    retn 4

__set_cr3@4:
    mov eax,[esp+4]
    mov cr3,eax
    retn 4

__set_cr4@4:
    mov eax,[esp+4]
    mov cr4,eax
    retn 4

__hang:
    jmp $

_TEXT ends

end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;-----------------------------KernelProcessorX86.asm----------------------------;;
;;--------------------------Copyright © Sidhant Goel, 2007-----------------------;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
