;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;------------------------------------fat32.asm----------------------------------;;
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
;; Boot Loader code for fat32 partitions                                         ;;
;; It checks for the presence of osldr.exe and loads it                          ;;
;;                                                                               ;;
;;                                                                               ;;
;;                                                                               ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[bits 16]
[org 0x7C00]

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Boot Sector
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

start:      	jmp short begin
            	nop

%include "fat32bpb.inc"

%define LOADER_STACK     0x7C00 ;Stack
%define DATA_AREA        0x7E00 ;Area to Store Local Variable
%define LOADER_EXTRA	 0x8000 ;Area to Load Remaining boot sector
%define FAT_SECT         0x8400 ;Area to Store FAT
%define CLUS_AREA        0x8600 ;Area to Store Cluster
%define CONFIG_ADDR		 0xA000

%define SECTORS_EXTRA	 0x0002 ;Remaining boot sectors

%define LOADER_ADDR     0x100000

%define FAT_SECT_LOADED     (DATA_AREA+0)
%define CLUSTER_LOADED      (DATA_AREA+4)
%define LBA_FAT             (DATA_AREA+8)
%define LBA_DATA            (DATA_AREA+12)
%define EOF_MARKER          (DATA_AREA+16)
%define CLUSTER_SIZE        (DATA_AREA+20)
%define DIRENT_PER_CLUS     (DATA_AREA+24)
%define MAX_CLUSTER         (DATA_AREA+28)
%define LOCAL_VAR0          (DATA_AREA+32)
%define MIN_CLUSTER         2
%define IMAGE_DOS_MAGIC     0x5A4D      ; MZ
%define IMAGE_PE_MAGIC      0x00004550  ; PE00

begin:  jmp 0x0:_begin
_begin:
        cli
        xor ax,ax
        mov ss,ax
        mov sp,0x7C00
        mov es,ax
        mov ds,ax
        mov [bsDrvNum],dl
        sti
        
        ;Read Remaining Boot Sector
        mov eax,[bpbHidenSect]
        add eax,2
        mov ecx,SECTORS_EXTRA
        mov bx,LOADER_EXTRA
        call readsecs
        mov ax,WORD[LOADER_EXTRA+SECTORS_EXTRA*0x200-2]
        cmp ax,0AA55h
        je skip.stage2
        MOV	SI,MsgNoStage2
		CALL	putstr
		JMP	$
		
skip.stage2:
        ;Calculate Fat offset And Data Offset
        mov eax,[bpbHidenSect]
        xor ecx,ecx
        mov cx,[bpbRessect]
        add eax,ecx
        mov [LBA_FAT],eax
        xor ecx,ecx
		mov eax,[bsFatSz32]
		mov cl,[bpbFatCnt]
		mul cx
		mov ecx,[LBA_FAT]
		add ecx,eax
		mov [LBA_DATA],ecx
		
		;Read Fat Sector 0
		xor eax,eax
		call readfat
		
		;Store EOF Marker
		mov eax, [FAT_SECT+4]
		mov [EOF_MARKER],eax
		
		;Calculate Cluster Size and directory Entries per cluster
		xor eax,eax
		xor ecx,ecx
		mov ax,[bpbSectSize]
		mov cl,[bpbClustSize]
		mul cx
		mov [CLUSTER_SIZE],eax
		shr eax,5
		mov [DIRENT_PER_CLUS],eax
		
		;Calculate Max Cluster
		mov eax,[bsFatSz32]
		mov cx,[bpbSectSize]
		shr cx,2
		mul cx
		dec eax
		mov [MAX_CLUSTER],eax
		
		call enableA20
		
		mov esi, FileName0
		call searchfile
		mov edi, 0x100000
		call readfile
		mov edi, CONFIG_ADDR
		call ram_e820
		jc e820fail
		call callkernel
e820fail:
		MOV	SI,MsgMemory
		CALL	putstr
		jmp $
		
bootfail:	
		MOV	SI,MsgBad
		CALL	putstr
		JMP	$
        
putstr: 
        lodsb
        or al,al
        jz short putstrd
        mov ah,0x0E
        mov bx,0x0007
        int 0x10
        jmp putstr
putstrd:
        retn

readsecs:	
        push eax  ;eax=lba;ecx=count;es:bx=addr
		push ecx
		push ebx
		call readsec
		pop ebx
		pop ecx
		pop eax
		inc eax
		add bx,[bpbSectSize]
		dec ecx
		jnz readsecs
		ret

readsec:	
        xor ecx,ecx
		xor edx,edx
		mov cx,word [bpbTrackSect]
		div ecx
		mov ecx,edx
		xor edi,edi
		xor edx,edx
		mov di,word [bpbHeadCnt]
		div edi
		mov edi,edx
		xchg di,dx
		mov dx,di
		mov dh,dl
		inc cx
		mov ch,al
		shl cl,1
		shl cl,1
		shr ah,1
		rcr cl,1
		shr ah,1
		rcr cl,1
		mov dl,[bsDrvNum]
		mov ax,0201h
		int 13h
		jc bootfail
		ret

MsgBad      	db "DISK ERROR...",0Dh,0Ah,0
MsgFileNotFound db "MISSING SYSTEM FILES...",0Dh,0Ah,0
MsgNoStage2     db "PART2 MISSING...",0Dh,0Ah,0

padding		times 510-($-$$) db 0
bootsign	dw 0AA55h

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  FSInfo Sector
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

FSInfoSignature     dd  0x41615252
                    times 480 db 0
FSInfoSignature2    dd  0x61417272
                    dd  0x000FDF0F
                    dd  0x00000003
                    times 12 db 0
                    dw 0
                    dw 0AA55h
                    
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Boot Sector Part2
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

MsgMemory	db "MEMORY DETECTION ERROR...", 0Dh, 0Ah, 0

FileName0    	db "OSLDR   EXE"

enableA20:
        cli
        call    a20wait
        mov     al,0xAD
        out     0x64,al
        call    a20wait
        mov     al,0xD0
        out     0x64,al
        call    a20wait2
        in      al,0x60
        push    eax
        call    a20wait
        mov     al,0xD1
        out     0x64,al
        call    a20wait
        pop     eax
        or      al,2
        out     0x60,al
        call    a20wait
        mov     al,0xAE
        out     0x64,al
        call    a20wait
        sti
a20end: ret
a20wait:
.l0:    in      al,0x64
        test    al,2
        jz      .l2
        jmp     .l0
.l2:    ret
a20wait2:
.l0:    in      al,0x64
        test    al,1
        jnz     .l2
        jmp     .l0
.l2:    ret

ram_e820:
    push di
    add di, 4
	xor ebx, ebx				; ebx must be 0 to start
	xor bp, bp					; keep an entry count in bp
	mov edx, 0x0534D4150		; Place "SMAP" into edx
	mov eax, 0xe820
	mov [es:di + 20], dword 1	; force a valid ACPI 3.X entry
	mov ecx, 24					; ask for 24 bytes
	int 0x15
	jc short .failed			; carry set on first call means "unsupported function"
	mov edx, 0x0534D4150		; Some BIOSes apparently trash this register?
	cmp eax, edx				; on success, eax must have been reset to "SMAP"
	jne short .failed
	test ebx, ebx				; ebx = 0 implies list is only 1 entry long (worthless)
	je short .failed
	jmp short .jmpin
.e820lp:
	mov eax, 0xe820				; eax, ecx get trashed on every int 0x15 call
	mov [es:di + 20], dword 1	; force a valid ACPI 3.X entry
	mov ecx, 24					; ask for 24 bytes again
	int 0x15
	jc short .e820f				; carry set means "end of list already reached"
	mov edx, 0x0534D4150		; repair potentially trashed register
.jmpin:
	jcxz .skipent				; skip any 0 length entries
	cmp cl, 20					; got a 24 byte ACPI 3.X response?
	jbe short .notext
	test byte [es:di + 20], 1	; if so: is the "ignore this data" bit clear?
	je short .skipent
.notext:
	mov ecx, [es:di + 8]		; get lower dword of memory region length
	test ecx, ecx				; is the qword == 0?
	jne short .goodent
	mov ecx, [es:di + 12]		; get upper dword of memory region length
	jecxz .skipent				; if length qword is 0, skip entry
.goodent:
	inc bp						; got a good entry: ++count, move to next storage spot
	add di, 24
.skipent:
	test ebx, ebx				; if ebx resets to 0, list is complete
	jne short .e820lp
.e820f:
    pop di
    mov DWORD[di], 0
	mov [di], bp				; store the entry count
	ret							; test opcode cleared carry flag
.failed:
    pop di
	stc							; "function unsupported" error exit
	ret

searchfile:
        mov eax, [CLUSTER_SIZE]
        add eax, CLUS_AREA
        mov [LOCAL_VAR0],eax
        mov eax, [bsRootClus]
searchfile.0
        push esi
        call readclus
        pop esi
        
        mov edi,CLUS_AREA
next.en:
        push esi
        push edi
        mov ecx,11
        cld
        rep cmpsb
        jnz nextf
        
        pop edi
        pop esi
        xor eax, eax
        mov ax, [edi+26]
        jmp searchfile.end
        
nextf:  pop edi
        pop esi
        add edi, 32
        cmp edi,[LOCAL_VAR0]
        jb next.en
        
        mov eax,[CLUSTER_LOADED]
        push esi
        call nextclus
        pop esi
        cmp eax,[MAX_CLUSTER]
        ja searchfile.fnf
        cmp eax, MIN_CLUSTER
        jb searchfile.fnf
        jmp searchfile.0

searchfile.fnf:
		MOV	SI,MsgFileNotFound
		CALL	putstr
		JMP	$
		
searchfile.end:
        ret

readfat:
        mov [FAT_SECT_LOADED],eax
		add eax,[LBA_FAT]
        mov bx,FAT_SECT
		call readsec
		ret
		
readclus:
        mov [CLUSTER_LOADED],eax
        sub eax,2
		xor ecx,ecx
		mov cl,[bpbClustSize]
		mul cx
		add eax,[LBA_DATA]
		mov bx,CLUS_AREA
		call readsecs
		ret
		
nextclus:
        xor ecx,ecx
        xor edx,edx
        mov cx,[bpbSectSize]
        shr ecx,2
        div ecx
        cmp [FAT_SECT_LOADED],eax
        je nextclus.0
        push edx
        call readfat
        pop edx
nextclus.0:
        mov eax, [FAT_SECT+edx*4]
        ret
        
readfile:
        cmp eax,[MAX_CLUSTER]
        ja readfile.0
        cmp eax, MIN_CLUSTER
        jb readfile.0
        push eax
        push edi
        call readclus
        mov esi, CLUS_AREA
        pop edi
        mov ecx, [CLUSTER_SIZE]
        call memcpy
        pop eax
        push edi
        call nextclus
        pop edi
        jmp readfile
readfile.0:
        ret
        
memcpy:
		cli      ; no interrupt
        push gs
        
        lgdt [gdt_desc]   ; load gdt register

        mov  eax, cr0   ; switch to pmode by
        or al,1         ; set pmode bit
        mov  cr0, eax
        mov  bx, 0x10   ; select descriptor 1
        mov  gs, bx   ; 8h = 1000b
        and al,0xFE     ; back to realmode
        mov  cr0, eax   ; by toggling bit again
        
cloop:  mov al, [gs:esi]
        mov [gs:edi], al
        inc esi
        inc edi
        dec ecx
        jnz cloop

		pop gs      ; get back old segment
        sti
        ret

callkernel:
        cli      ; no interrupt
        lgdt [gdt_desc]   ; load gdt register
        mov  eax, cr0   ; switch to pmode by
        or al,1         ; set pmode bit
        mov  cr0, eax
        jmp 08h:callkernel.32

[bits 32]

callkernel.32:
        mov ax, 10h
        mov ds,ax
        mov es,ax
        mov ss,ax
        mov fs,ax
        mov gs,ax
        cmp word[LOADER_ADDR],IMAGE_DOS_MAGIC
        jne callkernel.error
        mov eax, [LOADER_ADDR+60]
        cmp dword[LOADER_ADDR+eax], IMAGE_PE_MAGIC
        jne callkernel.error
        mov eax, [LOADER_ADDR+eax+40]
        add eax, LOADER_ADDR
        mov esp, 0x400000
        push CONFIG_ADDR
        call eax
        jmp $
        
callkernel.error:
        jmp $
        
gdt:                    ; Address for the GDT

gdt_null:               ; Null Segment
        dd 0
        dd 0

gdt_code:               ; Code segment, read/execute, nonconforming
        dw 0FFFFh
        dw 0
        db 0
        db 10011010b
        db 11001111b
        db 0

gdt_data:               ; Data segment, read/write, expand down
        dw 0FFFFh
        dw 0
        db 0
        db 10010010b
        db 11001111b
        db 0


gdt_end:                ; Used to calculate the size of the GDT



gdt_desc:                       ; The GDT descriptor
        dw gdt_end - gdt - 1    ; Limit (size)
        dd gdt                  ; Address of the GDT


        times (512+512+512*SECTORS_EXTRA-2)-($-$$) db 0
	    dw 0AA55h
	    
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;-------------------------------------fat32.asm---------------------------------;;
;;--------------------------Copyright © Sidhant Goel, 2007-----------------------;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;