;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;-----------------------------------fat32bpb.inc--------------------------------;;
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
;; BIOS Parameter Block of FAT32 Partitions                                      ;;
;;                                         									     ;;
;;                                                                               ;;
;;                                                                               ;;
;;                                                                               ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

bsOEM        	db 'ENVIS1.0'
bpbSectSize  	dw 512
bpbClustSize 	db 2
bpbRessect   	dw 32
bpbFatCnt    	db 2
bpbRootEntCnt 	dw 0
bpbTotalSect 	dw 0
bpbMedia     	db 0xF8
bpbFatSize   	dw 0
bpbTrackSect 	dw 63
bpbHeadCnt   	dw 64
bpbHidenSect 	dd 63
bpbHugeSect  	dd 2096577

bsFatSz32   	dd 8128
bsExtFlags  	dw 0
bsFSVer     	dw 0
bsRootClus  	dd 2
bsFSInfo    	dw 1
bsBkBootSec 	dw 6

                dd 0 ;Reserved
		        dd 0
		        dd 0

bsDrvNum   	    db 80h
bsReserved1	    db 0
bsBootSig	    db 29h
bsVolID		    dd 1CE1485Bh
bsVolLab	    db 'NO NAME    '
bsFilSysType	db 'FAT32   '

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;-----------------------------------fat32bpb.inc--------------------------------;;
;;--------------------------Copyright © Sidhant Goel, 2007-----------------------;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;