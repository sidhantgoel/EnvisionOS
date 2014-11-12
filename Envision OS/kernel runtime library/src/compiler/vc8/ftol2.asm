        .xlist
        include cruntime.inc
        .list

        CODESEG


		public __ftol2_sse
__ftol2_sse	proc near
		cmp	dword ptr ds:___sse2_available,	0
		jz	short __ftol2
__ftol2_sse	endp


		public __ftol2_pentium4
__ftol2_pentium4 proc near

var_8		= qword	ptr -8

		push	ebp
		mov	ebp, esp
		sub	esp, 8
		and	esp, 0FFFFFFF8h
		fstp	[esp+8+var_8]
		cvttsd2si eax, [esp+8+var_8]
		leave
		retn
__ftol2_pentium4 endp


		public __ftol2_sse_excpt
__ftol2_sse_excpt proc near

var_4		= word ptr -4

		cmp	dword ptr ds:___sse2_available,	0
		jz	short __ftol2
		sub	esp, 4
		fnstcw	[esp+4+var_4]
		pop	eax
		and	ax, 7Fh
		cmp	ax, 7Fh
		jz	short __ftol2_pentium4
__ftol2_sse_excpt endp


		public __ftol2
__ftol2		proc near

var_20		= dword	ptr -20h
var_10		= qword	ptr -10h
var_8		= dword	ptr -8

		push	ebp
		mov	ebp, esp
		sub	esp, 20h
		and	esp, 0FFFFFFF0h
		fld	st
		fst	[esp+20h+var_8]
		fistp	[esp+20h+var_10]
		fild	[esp+20h+var_10]
		mov	edx, [esp+20h+var_8]
		mov	eax, dword ptr [esp+20h+var_10]
		test	eax, eax
		jz	short integer_QnaN_or_zero

arg_is_not_integer_QnaN:
		fsubp	st(1), st
		test	edx, edx
		jns	short positive
		fstp	[esp+20h+var_20]
		mov	ecx, [esp+20h+var_20]
		xor	ecx, 80000000h
		add	ecx, 7FFFFFFFh
		adc	eax, 0
		mov	edx, dword ptr [esp+20h+var_10+4]
		adc	edx, 0
		jmp	short localexit

positive:
		fstp	[esp+20h+var_20]
		mov	ecx, [esp+20h+var_20]
		add	ecx, 7FFFFFFFh
		sbb	eax, 0
		mov	edx, dword ptr [esp+20h+var_10+4]
		sbb	edx, 0
		jmp	short localexit

integer_QnaN_or_zero:
		mov	edx, dword ptr [esp+20h+var_10+4]
		test	edx, 7FFFFFFFh
		jnz	short arg_is_not_integer_QnaN
		fstp	[esp+20h+var_8]
		fstp	[esp+20h+var_8]

localexit:
		leave
		retn
__ftol2		endp

_text		ends

extrn ___sse2_available:near 

		end
