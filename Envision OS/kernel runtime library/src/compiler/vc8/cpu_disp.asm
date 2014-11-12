        .xlist
        include cruntime.inc
        .list

        CODESEG

		public __get_sse2_info
__get_sse2_info	proc near

var_18		= dword	ptr -18h
var_14		= dword	ptr -14h
var_10		= dword	ptr -10h
var_C		= dword	ptr -0Ch
var_8		= dword	ptr -8
var_4		= dword	ptr -4

		push	ebp
		mov	ebp, esp
		sub	esp, 18h
		xor	eax, eax
		push	ebx
		mov	[ebp+var_4], eax
		mov	[ebp+var_C], eax
		mov	[ebp+var_8], eax
		push	ebx
		pushf
		pop	eax
		mov	ecx, eax
		xor	eax, 200000h
		push	eax
		popf
		pushf
		pop	edx
		sub	edx, ecx
		jz	short $DONE$23192
		push	ecx
		popf
		xor	eax, eax
		cpuid
		mov	[ebp+var_C], eax
		mov	[ebp+var_18], ebx
		mov	[ebp+var_14], edx
		mov	[ebp+var_10], ecx
		mov	eax, 1
		cpuid
		mov	[ebp+var_4], edx
		mov	[ebp+var_8], eax

$DONE$23192:
		pop	ebx
		test	[ebp+var_4], 4000000h
		jz	short loc_CB
		;call	_has_osfxsr_set
		;test	eax, eax
		;jz	short loc_CB
		xor	eax, eax
		inc	eax
		jmp	short loc_CD

loc_CB:
		xor	eax, eax

loc_CD:
		pop	ebx
		leave
		retn
__get_sse2_info	endp


		public ___sse2_available_init
___sse2_available_init proc near
		call	__get_sse2_info
		;mov	dword ptr ds:___sse2_available,	eax
		mov	dword ptr ds:___sse2_available,	0
		xor	eax, eax
		retn
___sse2_available_init endp

_text		ends

.data?

___sse2_available   dd  ?
public ___sse2_available

_bss   ends

        end
