org 0x7C00

%include "iso9660fs.inc"

;----------------------------
start:
	cli

	xor ax,ax
	mov ds,ax					;set ds 2 0h
	mov sp,ax

	mov ax,9000h
	mov ss,ax					;set stack 2 90000h

	mov ax,1000h
	mov es,ax					;set es 2 10000h

	sti

	mov byte[bootdrv],dl		;save bootdrive

	call load_vol_desc
	test ax,ax
	jz .err_read

	call load_root_dir
	test ax,ax
	jz .err_read

	call load_loader
	test ax,ax
	jz .err_no_loader

	mov dl,byte[bootdrv]		;move bootdrive into dl for the loader

	jmp 1000h:0000h
;----------------------------
.err_read:
	mov si,msg_err_read
	call print

	jmp reboot
;----------------------------
.err_no_loader:
	mov si,msg_err_loader
	call print

	jmp reboot
;----------------------------

;----------------------------
reboot:
	mov si,msg_reboot
	call print

	xor ax,ax
	int 16h
	int 19h
;----------------------------

;----------------------------
print:
	pusha

	mov ah,0eh

.loop:
	lodsb
	test al,al
	jz .end
	int 10h
	jmp .loop

.end:
	popa
	ret
;----------------------------

;----------------------------
load_vol_desc:

.loop:
	mov si,dap
	mov ah,42h
	mov dl,[bootdrv]
	int 13h						;load the lba which stands in the dap into the location which stands also in the dap

	jc .end_err

	cmp byte[es:0],1
	je .end						;is this the primary volume descriptor, if yes jump
	cmp byte[es:0],0ffh
	je .end_err					;is this the last volume descriptor, if yes jump

	inc dword[dap.lba]			;next lba
	jmp .loop

.end:
	mov ax,1
	ret

.end_err:
	xor ax,ax
	ret
;----------------------------

;----------------------------
load_root_dir:
	mov si,iso9660vol_desc.root_dir_rec

	mov eax,dword[es:si+iso9660dir_desc.loc_data]	;the start lba of the root directory
	mov ecx,dword[es:si+iso9660dir_desc.data_len]	;the length of the root directory
	shr ecx,11										;length of root dir / 2048, because we need the # of blocks
	add cx,1
	call load_lba

.end:
	ret
;----------------------------

;----------------------------
load_loader:
	xor si,si
	shl ecx,11					;in ecx is the # of blocks of the root dir, but we need the size

.loop_search:
	cmp byte[es:si],0
	je .end_err					;is this no root dir entry, if yes jump
	cmp si,cx
	je .end_err					;is si = size of root dir, if yes jump

	mov di,si
	add di,iso9660dir_desc.file_ident				;move the addr of the act entry into di and let it point to the file name

	push cx
	push si

	movzx cx,byte[es:si+iso9660dir_desc.len_file_ident]			;get the size of the file indentifier
	mov si,osloader_img
	rep cmpsb

	mov al,[si]					;when the strings were equal al should be 0

	pop si
	pop cx

	test al,al
	jz .load

	movzx ax,byte[es:si+iso9660dir_desc.len_dir_rec]
	add si,ax					;get size of the act entry and add it to si, so that si points to the next entry

	jmp .loop_search

.load:
	mov eax,dword[es:si+iso9660dir_desc.loc_data]	;start lba of the file
	mov ecx,dword[es:si+iso9660dir_desc.data_len]	;length of the file
	shr ecx,11
	inc cx											;we need the blocks 2 load for the file and 2 be sure that we load the whole file we inc cx, cause who cares if we load one more block than the file is big?!
	call load_lba

.end:
	ret

.end_err:
	xor ax,ax
	ret
;----------------------------

;----------------------------
;	Input:
;	EAX - LBA
;	CX - # of lbas 2 load
load_lba:
	pusha

	mov [dap.blocks],cx			;save # of blocks 2 load
	mov [dap.lba],eax			;save lba to load

	mov si,dap
	mov ah,42h
	mov dl,[bootdrv]
	int 13h

	jc .end_err

.end:
	popa
	mov ax,1
	ret

.end_err:
	popa
	xor ax,ax
	ret
;----------------------------

align 4
;----------------------------
;	data

;	vars
dap:
	.size		db 10h
	.reserved	db 0
	.blocks		dw 1
	.offset		dw 0
	.segment	dw 1000h
	.lba		dd 10h,0
bootdrv			db 0

;	consts
osloader_img	db 'OSLOADER',0

;	msgs
msg_reboot		db 'Press any key to reboot',13,10,0
msg_err_read	db 'error occured while reading disk',13,10,0
msg_err_loader	db 'OS-Loader not found',13,10,0

times 510-($-$$) db 0
dw 0aa55h