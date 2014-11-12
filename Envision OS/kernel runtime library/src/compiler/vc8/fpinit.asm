        .xlist
        include cruntime.inc
        .list
        
.data

		public __fltused
__fltused	dd 9875h
		public __ldused
__ldused	dd 9873h

_DATA		ends


.data?

		public ___fastflag
___fastflag		dd ?
		public __adjust_fdiv
__adjust_fdiv	dd ?
_BSS		ends


.const

		public __FPmtinit
__FPmtinit	dd offset __fpclear
		public __FPmtterm
__FPmtterm	dd offset __fpclear

CONST		ends


		codeseg
		
		public _fpinit
_fpinit     proc near
        finit
        call ___sse2_available_init
        retn
_fpinit endp
      
		public __fpclear
__fpclear	proc near
			retn
__fpclear	endp

		public ___setfflag
___setfflag	proc near

arg_0		= dword	ptr  4

		mov	ecx, [esp+arg_0]
		mov	eax, ds:___fastflag
		mov	ds:___fastflag,	ecx
		retn
___setfflag	endp


_TEXT		ends

extern ___sse2_available_init:near

		end

