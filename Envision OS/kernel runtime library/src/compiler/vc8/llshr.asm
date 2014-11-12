        .xlist
        include cruntime.inc
        .list

		codeseg

public __allshr
__allshr:
		cmp	cl, 40h
		jnb	RETSIGN
		cmp	cl, 20h
		jnb	MORE32
		shrd eax, edx, cl
		sar	edx, cl
		retn
MORE32:
		mov	eax, edx
		sar	edx, 1Fh
		and	cl, 1Fh
		sar	eax, cl
		retn
RETSIGN:
		sar	edx, 1Fh
		mov	eax, edx
		retn

_TEXT	ends
		end

