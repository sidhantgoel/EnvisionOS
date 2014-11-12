;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;------------------------------------ptable.inc---------------------------------;;
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
;; Partition Table                 									             ;;
;;                                          								     ;;
;;                                                                               ;;
;;                                                                               ;;
;;                                                                               ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

part0:
    db 80h			; 80 = Active Partition
    db 01h			; Start Head
    dw 0001h	    ; (0-5) = Start Sector, (6-15) = Start Cylinder
    db 0Bh			; Partition Type
    db 3Fh			; End Head
    dw 07BFh		; (0-5) = End Sector, (6-15) = End Cylinder
    dd 0000003Fh	; Start LBA
    dd 001FFDC1h	; Sector Count
   
part1:
    db 00h			; 80 = Active Partition
    db 00h			; Start Head
    dw 0000h		; (0-5) = Start Sector, (6-15) = Start Cylinder
    db 00h			; Partition Type
    db 00h			; End Head
    dw 0000h		; (0-5) = End Sector, (6-15) = End Cylinder
    dd 00000000h	; Start LBA
    dd 00000000h	; Sector Count
   
part2:
    db 00h			; 80 = Active Partition
    db 00h			; Start Head
    dw 0000h		; (0-5) = Start Sector, (6-15) = Start Cylinder
    db 00h			; Partition Type
    db 00h			; End Head
    dw 0000h		; (0-5) = End Sector, (6-15) = End Cylinder
    dd 00000000h	; Start LBA
    dd 00000000h	; Sector Count
   
part3:
    db 00h			; 80 = Active Partition
    db 00h			; Start Head
    dw 0000h		; (0-5) = Start Sector, (6-15) = Start Cylinder
    db 00h			; Partition Type
    db 00h			; End Head
    dw 0000h		; (0-5) = End Sector, (6-15) = End Cylinder
    dd 00000000h	; Start LBA
    dd 00000000h	; Sector Count

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;-----------------------------------ptable.inc----------------------------------;;
;;--------------------------Copyright © Sidhant Goel, 2007-----------------------;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;