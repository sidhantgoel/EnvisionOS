;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;-------------------------------------mbr.asm-----------------------------------;;
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
;; A 512 byte Master Boot Record												 ;;
;; that simply boots the active partition.										 ;;
;;                                                                               ;;
;;                                                                               ;;
;;                                                                               ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[bits 16]
[org 0]

; 

%define LOAD		7C00h		; Load address
%define EXEC		7A00h 		; Execution address
%define STACK		7A00h		; Stack Address
%define EXEC_SEG	7A0h		; Execution Segment

%define PT_OFF		1BEh		; Partition table
%define MAGIC		0AA55h		; Magic: bootable
%define MAGIC_OFF	1FEh		; Magic Offset

; Setup the segment registers for flat addressing and setup the stack.
start:	cli					; Disable Interrupts
		xor ax,ax			; Set up segment registers
		mov es,ax			
		mov ds,ax			
		mov ss,ax			
		mov sp,STACK		; Set up stack
		
; Relocate ourself to a lower address.
		mov si,LOAD
		mov di,EXEC
		mov cx,200h
		rep movsb			; Relocate code
		
; Jump to the relocated code.
		jmp word EXEC_SEG:main


; Scan the partition table looking for an active entry.
main:	sti
		mov [EXEC+drive],dl
		
		mov cx,4
		mov si,EXEC+PT_OFF
		
main.0:	cmp [si],byte 80h
		je main.1
		dec cx
		jz err_os
		add si,10h
		jmp main.0
		
main.1:	mov [EXEC+act_pe],si
	
		;Load Kernel from Disk to RAM
		;-Interrupt 0x13 (BIOS Common Disk Access), Function 0x02 (Read Sectors into Memory)
		;-AH = BIOS Interrupt Function
		;-AL = Number of 512-byte Sectors to load
		;-CX = Cylinder & Sector Offsets
		;-DX = Head Offset & Drive Number
		;-ES:BX = Segment:Offset of Address Destination in RAM
		
		mov si,[EXEC+act_pe]

		mov ah,0x02       ;BIOS Interrupt Function 0x02 (Read Sectors into Memory)
		mov al,0x01       ;Load 1 sector from floppy
		mov bx,0x7C00     ;Set BX offset to our desired load location, ES already set from above
		mov cx,[si+2]     ;Set Cylinder Number, Sector Number
		mov dh,[si+1]     ;Set Head Number
		mov dl,[EXEC+drive]
		int 0x13          ;Execute BIOS Interrupt
		
		jc err_hd
		
		mov ax,[LOAD+MAGIC_OFF]
		cmp ax,MAGIC
		jne err_os

		jmp 0x0000:0x7C00	;Jump to newly loaded Kernel
							;Sets CS to 0x0000 and the IP to 0x7C00, execution continues at that address

		jmp err_os

err_os: mov si,EXEC+msg.os
		call putstr
		jmp $
			
err_hd:	mov si,EXEC+msg.hd
		call putstr
		jmp $
		
putstr.0:
		mov bx,7h	 		; Page:attribute
		mov ah,0eh			; BIOS: Display
		int 10h				;  character
putstr: lodsb
		test al,al			; End of string?
		jnz putstr.0		; No
putstr.1:
		retn				; Return
		

; Messages
msg.hd	db "DISK ERROR",0Dh,0Ah,0
msg.os 	db "OS MISSING",0Dh,0Ah,0

; Data
drive	db	00h
act_pe	dw	00h

; Padding
		times PT_OFF-($-$$) db 00h

; Partition Table
%include "ptable.inc"

		dw MAGIC			; Magic number

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;-------------------------------------mbr.asm-----------------------------------;;
;;--------------------------Copyright © Sidhant Goel, 2007-----------------------;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
