        .xlist
        include cruntime.inc
        .list

		codeseg
	
public __aullshr
__aullshr:
		cmp	cl, 40h
		jnb	RETSIGN
		cmp	cl, 20h
		jnb	MORE32
		shrd eax, edx, cl
		shr	edx, cl
		retn
MORE32:
		mov	eax, edx
		xor edx, edx
		and	cl, 1Fh
		shr	eax, cl
		retn
RETSIGN:
		xor eax, eax
		xor edx, edx
		retn

_TEXT	ends
		end

