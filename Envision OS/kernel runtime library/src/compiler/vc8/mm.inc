;***
;mm.inc - macros to write memory model dependent code
;
;       Copyright (c) Microsoft Corporation.  All rights reserved.
;
;Purpose:
;       This file contains definitions of a number of macros which
;       make the writing of memory model dependent code for the
;       386 a little easier and more portable.
;
;*******************************************************************************

; general code & data size constants & macros

DAT_ADDR_SZ =   4
BDAT_ADDR_SZ =  2

TXT_ADDR_SZ =   4

LOWORD  equ     [0]
HIWORD  equ     [4]

; All Model Definitions

BPARGBAS equ    TXT_ADDR_SZ+4   ; offset from BP to first argument
