;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Pac Menu code - game hook code
;
;  2004-01,02  Scott "Jerry" Lawrence
;
;  $Id: gamehook.asm,v 1.13 2004/10/28 19:10:21 jerry Exp $
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module gamehook


.stTXT:
	.byte	C_white, 0xff, 0x10, 10
	.ascii	"START@GAME"

.1startit:			; 1p start hook
	ld	a, #1
	ld	(started), a
	ld	bc, (oper1games)
	call	incBCDbc
	ld	(oper1games), bc	; adjust bookkeeping
	jr 	.12startit

.2startit:			; 2p start hook
	ld	a, #2
	ld	(started), a
	ld	bc, (oper2games)
	call	incBCDbc
	ld	(oper2games), bc	; adjust bookkeeping

.12startit:
	call	tt_MENU
	call	fullclear

	xor	a
	ld	(played), a		; reset the "game has been played" flag

	ld	a, (ogamesel)
	ld	(gamesel), a		; restore this just in case


	ld	a, (gamesel)
	rr	a			; a = a / 2
	and	#0x7f			; trim off rotated bit

	cp	#Rlsr
	jp	z, lsr_run		; lightswitch rave!

	cp	#Rquad
	jp	z, qb_run		; jump to quadblock entry

	cp	#Rmspac
	cp	#Rmspacatk
	cp	#RmodeJ
	cp	#Rpac
	cp	#Rpengman
	cp	#Raarmada

	    ; there should be no fallthrough...

.if DEBUG
	ld	a, (gamesel)
	rr	a			; a = a / 2
	ld	d, a
	ld	e, #(C_white)
	ld	bc, #0x3c2
	call	prtBCDbyte

	ld	a, (gamecfg)
	ld	d, a
	ld	e, #(C_blue)
	ld	bc, #0x3e2
	call	prtBCDbyte
.endif

;	ld	hl, #(.stTXT)
;	call	putstrBP
;	ld	bc, #60
;	call	sleep

	ld	a, (gamebank)
	ld	b, a
	ld	a, (gfxbank)
	ld	c, a
	ld	hl, #0x0000		; base for now.
	jp	TR_GAME			; change the bank!
            ; at this point, we're actully running from a different rom bank
	halt
