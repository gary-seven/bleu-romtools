;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Pac Menu code - lightswitch rave
;
;  2004-02  Scott "Jerry" Lawrence
;
;  $Id: game_lsr.asm,v 1.3 2004/07/29 02:59:39 jerry Exp $
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module lsr

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; game parameters

	LSR_GOTIME	= 30

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; ram defs
	qbr		= USERRAM
	qbr_t0		= qbr 			; this in0
	qbr_l0		= qbr_t0 + 1		; last in0

	qbr_hs0		= qbr_l0 + 1		; hs word 0	....XXXX
	qbr_hs1		= qbr_hs0 + 4		; hs word 1	XXXX....

lsr_exit:
	call	ptStop			; stop music
	call	ptSilence		; stop audio

	; normally change to bank 0
	call	cls

	; set up a timeout to jump to the reset routine 
	ld	bc, #LSR_GOTIME		; set above
    	ld	de, #(lsr_etimer)
	call	atDelta

	; display text: GAME OVER, MAN... GAME OVER.
	ld	hl, #(.lsr_tGO)

	; infinite loop on color cycles here, the timeout above
	; will exit the loop
.lsr_ex000:
	ld	bc, #0x0a10
	ld	a, #(C_cyan)
	call	putstrB			; text in cyan
	ld	bc, #15
	call	sleep			; wait 1/4 second

	ld	bc, #0x0a10
	ld	a, #(C_yellow)
	call	putstrB			; text in yellow
	ld	bc, #15
	call	sleep			; wait 1/4 second

	ld	bc, #0x0a10
	ld	a, #(C_pink)
	call	putstrB			; text in pink
	ld	bc, #15
	call	sleep			; wait 1/4 second

	jr	.lsr_ex000		; jump back, kiss myself

lsr_e2timer:
	call	cls
	ret

lsr_etimer:
	rst	0x08
	halt

.lsr_tGO:
	.byte	9
	.ascii	"GAME@OVER"

lsr_run:
	; setup core stuff again, minimum
	; reload stack
	xor	a
	ld	(irqen), a
	di
	ld	sp, #(stack)
	ld	(watchdog), a
	im	1			; shouldn't need to do this
	call	clrspecial
	call	clrsprite
	call	clruser
	call	modeGAME		; sets up menu game mode
	call	cls
	xor	a
	ld	(watchdog), a
	ld	a, #0x01
	ld	(irqen), a
	ei

	    ; set up song playback
	ld	bc, #digdug_song
	ld	a, #10			; speed of 10
	call	ptInit
	call	ptPlay

;;;;;;;;;;;;;;;;;;;;


	; one-time setup stuff
lsr_ots:
	call	dip			; preload dip settings

	call	cls

	    ; game init setup per game configurations...
	ld	a, (gamecfg)
	cp	#0x01			; tournament mode
	cp	#0x00			; normal mode

	; main lsr loop
lsr_loop:
	call	coincheck		; check for coins
	call	credsdisp		; display credits
	call	lsrhsdisp		; display high score
	call	lsr_input		; check user input
	call	lsr_draw		; draw the screen
	call	lsr_hint		; draw the hint

	call	ptPoll			; song poll
	jp	lsr_loop		; do it again


lsr_input:
	ld	a, (p1_port)
	ld	hl, #(qbr_t0)
	ld	(hl), a			; qbr_t0 = p1_port   (thisthisin0)

	call	lsr_up
	call	lsr_down
	call	lsr_left
	call	lsr_right

	ld	a, (qbr_t0)
	ld	(qbr_l0), a		; last=this
	ret



lsr_up:
	push 	hl
	bit	#p1_up, (hl)
	jr	nz, .lsr_Pret
	ld	hl, #(qbr_l0)
	bit	#p1_up, (hl)
	jr	z, .lsr_Pret

	; do stuff here
	call	randomize
	ld	bc, #0x0100
    	ld	de, #(lsr_e2timer)
	call	at2Delta
	
.lsr_Pret:
	pop	hl
	ret

lsr_down:
	push 	hl
	bit	#p1_down, (hl)
	jr	nz, .lsr_Pret
	ld	hl, #(qbr_l0)
	bit	#p1_down, (hl)
	jr	z, .lsr_Pret

	; do stuff here
	call	randomize
	jr	.lsr_Pret

lsr_left:
	    ; debounce
	push 	hl
	bit	#p1_left, (hl)
	jr	nz, .lsr_Pret
	ld	hl, #(qbr_l0)
	bit	#p1_left, (hl)
	jr	z, .lsr_Pret

	; do stuff here
	call	randomize
	jr	.lsr_Pret

lsr_right:
	    ; debounce
	push 	hl
	bit	#p1_right, (hl)
	jr	nz, .lsr_Pret
	ld	hl, #(qbr_l0)
	bit	#p1_right, (hl)
	jr	z, .lsr_Pret

	; do stuff here
	call	randomize
	jr	.lsr_Pret


	; high score
lsrhsdisp:
	ld	a, #C_white
	ld	bc, #0x0900
	ld	hl, #(.lsrThs)
	call	putstrA

	ld	hl, #qbr_hs0
	ld	e, #C_white
	ld	bc, #0x03ee
	call	prtABCDword

	ret
	
.lsrThs:
	.byte 	10
	.ascii	"HIGH@SCORE"


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; screen switch draw routines

lsr_draw:
	ld	a, (p1_port)
	bit	#p1_up, a
	jr	z, .lsrd0

	ld	a, #0x10
	jr	.lsrd99

.lsrd0:
	ld	a, #0x1d

.lsrd99:
	push	af
	cp	#0x1d
	jr	z, .lsrd80
	
	ld	a, #C_blue
	jr	.lsrd88
.lsrd80:
	ld	a, #C_cyan
.lsrd88:
	ld	bc, #0x0304
	ld	hl, #(lsr_Ttitle)
	call	putstrB
	pop	af

	ld	hl, #(lsr_TTOP)
	ld	bc, #0x0708
	call	putstrB

	ld	hl, #(lsr_T00)
	inc	c
	call	putstrB

	ld	hl, #(lsr_Tsc0)		; top screw
	inc	c
	call	putstrB
	ld	hl, #(lsr_Tsc1)		; top screw 2
	inc	c
	call	putstrB

	ld	hl, #(lsr_T00)
	inc	c
	call	putstrB

	; draw the switch up or down based on joystick position
	cp	#0x1d
	jr	z, .lsrd70

	; off graphic
	ld	hl, #(lsr_T00)
	inc	c
	call	putstrB
	ld	hl, #(lsr_THH)
	inc	c
	call	putstrB
	ld	hl, #(lsr_TFF)
	inc	c
	call	putstrB
	ld	hl, #(lsr_TFF)
	inc	c
	call	putstrB
	ld	hl, #(lsr_TII)
	inc	c
	call	putstrB
	ld	hl, #(lsr_TDD)
	inc	c
	call	putstrB
	ld	hl, #(lsr_TDD)
	inc	c
	call	putstrB
	ld	hl, #(lsr_TJJ)
	inc	c
	call	putstrB
	ld	hl, #(lsr_TKK)
	inc	c
	call	putstrB
	ld	hl, #(lsr_TLL)
	inc	c
	call	putstrB

	jr	.lsrd77
.lsrd70:
	; on graphic
	ld	hl, #(lsr_TAA)
	inc	c
	call	putstrB
	ld	hl, #(lsr_TBB)
	inc	c
	call	putstrB
	ld	hl, #(lsr_TCC)
	inc	c
	call	putstrB
	ld	hl, #(lsr_TDD)
	inc	c
	call	putstrB
	ld	hl, #(lsr_TDD)
	inc	c
	call	putstrB
	ld	hl, #(lsr_TEE)
	inc	c
	call	putstrB
	ld	hl, #(lsr_TFF)
	inc	c
	call	putstrB
	ld	hl, #(lsr_TFF)
	inc	c
	call	putstrB
	ld	hl, #(lsr_TGG)
	inc	c
	call	putstrB
	ld	hl, #(lsr_T00)
	inc	c
	call	putstrB


.lsrd77:
	ld	hl, #(lsr_T00)
	inc	c
	call	putstrB

	ld	hl, #(lsr_Tsc0)		; top screw
	inc	c
	call	putstrB
	ld	hl, #(lsr_Tsc1)		; top screw 2
	inc	c
	call	putstrB

	ld	hl, #(lsr_T00)
	inc	c
	call	putstrB

	ld	hl, #(lsr_TBOT)
	inc	c
	call	putstrB


	ret

lsr_Ttitle:
	.byte	22
	.ascii	"THE@CHEATS@LIGHTSWITCH"

lsr_TTOP:
	.byte	14
	.byte	0xd1, 0xda, 0xda, 0xda, 0xda, 0xda, 0xda
	.byte	0xda, 0xda, 0xda, 0xda, 0xda, 0xda, 0xd0

lsr_TBOT:
	.byte	14
	.byte	0xd5, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd
	.byte	0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xd4
;  d0 d2 d2 d2 d2 d2 d2 d2 d2 d2 d2 d2 d2 d2 d2 d2 d2 d2 d2 d4
;  db __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ dc
;  db __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ dc
;  db __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ dc
;  db __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ dc
;  db __ __ __ __ __ ec d3 d3 d3 d3 d3 d3 ee __ __ __ __ __ dc
;  db __ f6 f8 __ e2 d8 d6 c0 c0 e0 __ __ da __ __ f6 f8 __ dc
;  da __ f7 f9 __ e3 d9 d7 c1 c1 e1 __ __ db __ __ f7 f9 __ dd
;  da __ __ __ __ __ ed d2 d2 d2 d2 d2 d3 ef __ __ __ __ __ dd
;  da __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ dd
;  da __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ dd
;  da __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ dd
;  da __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ dd
;  d1 d3 d3 d3 d3 d3 d3 d3 d3 d3 d3 d3 d3 d3 d3 d3 d3 d3 d3 d5
; 
;                 AA BB CC DD DD EE FF FF GG 00
;
;                 00 HH FF FF II DD DD JJ KK LL
;
;  d0 d2 d2 d2 d2 d2 d2 d2 d2 d2 d2 d2 d2 d2 d2 d2 d2 d2 d2 d4
;  db __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ dc
;  db __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ dc
;  db __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ dc
;  db __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ dc
;  db __ __ __ __ __ ec d3 d3 d3 d3 d3 d3 ee __ __ __ __ __ dc
;  db __ f6 f8 __ __ dc __ __ e2 c0 c0 d8 d6 e0 __ f6 f8 __ dc
;  da __ f7 f9 __ __ dc __ __ e3 c0 c0 d9 d7 e1 __ f7 f9 __ dd
;  da __ __ __ __ __ ed d2 d2 d2 d2 d2 d3 ef __ __ __ __ __ dd
;  da __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ dd
;  da __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ dd
;  da __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ dd
;  da __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ dd
;  d1 d3 d3 d3 d3 d3 d3 d3 d3 d3 d3 d3 d3 d3 d3 d3 d3 d3 d3 d5


lsr_T00:
	.byte	14
	.byte	0xd3, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40
	.byte	0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0xd2

lsr_Tsc0:
	.byte	14
	.byte	0xd3, 0x40, 0x40, 0x40, 0x40, 0x40
	.byte	0xf7, 0xf6
	.byte	0x40, 0x40, 0x40, 0x40, 0x40, 0xd2

lsr_Tsc1:
	.byte	14
	.byte	0xd3, 0x40, 0x40, 0x40, 0x40, 0x40
	.byte	0xf9, 0xf8
	.byte	0x40, 0x40, 0x40, 0x40, 0x40, 0xd2

lsr_TAA:
	.byte	14
	.byte	0xd3, 0x40, 0x40, 0x40, 0x40, 0x40
	.byte	0xe3, 0xe2
	.byte	0x40, 0x40, 0x40, 0x40, 0x40, 0xd2
	
lsr_TBB:
	.byte	14
	.byte	0xd3, 0x40, 0x40, 0x40, 0x40
	.byte	0xed, 0xd9, 0xd8, 0xec
	.byte	0x40, 0x40, 0x40, 0x40, 0xd2

lsr_TCC:
	.byte	14
	.byte	0xd3, 0x40, 0x40, 0x40, 0x40
	.byte	0xd2, 0xd7, 0xd6, 0xd3
	.byte	0x40, 0x40, 0x40, 0x40, 0xd2

lsr_TDD:
	.byte	14
	.byte	0xd3, 0x40, 0x40, 0x40, 0x40
	.byte	0xd2, 0xc1, 0xc0, 0xd3
	.byte	0x40, 0x40, 0x40, 0x40, 0xd2

lsr_TEE:
	.byte	14
	.byte	0xd3, 0x40, 0x40, 0x40, 0x40
	.byte	0xd2, 0xe1, 0xe0, 0xd3
	.byte	0x40, 0x40, 0x40, 0x40, 0xd2

lsr_TFF:
	.byte	14
	.byte	0xd3, 0x40, 0x40, 0x40, 0x40
	.byte	0xd2, 0x40, 0x40, 0xd3
	.byte	0x40, 0x40, 0x40, 0x40, 0xd2

lsr_TGG:
	.byte	14
	.byte	0xd3, 0x40, 0x40, 0x40, 0x40
	.byte	0xef, 0xdb, 0xda, 0xee
	.byte	0x40, 0x40, 0x40, 0x40, 0xd2

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

lsr_THH:
	.byte	14
	.byte	0xd3, 0x40, 0x40, 0x40, 0x40
	.byte	0xed, 0xdc, 0xdc, 0xec
	.byte	0x40, 0x40, 0x40, 0x40, 0xd2

lsr_TII:
	.byte	14
	.byte	0xd3, 0x40, 0x40, 0x40, 0x40
	.byte	0xd2, 0xe3, 0xe2, 0xd3
	.byte	0x40, 0x40, 0x40, 0x40, 0xd2

lsr_TJJ:
	.byte	14
	.byte	0xd3, 0x40, 0x40, 0x40, 0x40
	.byte	0xd2, 0xd9, 0xd8, 0xd3
	.byte	0x40, 0x40, 0x40, 0x40, 0xd2

lsr_TKK:
	.byte	14
	.byte	0xd3, 0x40, 0x40, 0x40, 0x40
	.byte	0xef, 0xd7, 0xd6, 0xee
	.byte	0x40, 0x40, 0x40, 0x40, 0xd2

lsr_TLL:
	.byte	14
	.byte	0xd3, 0x40, 0x40, 0x40, 0x40
	.byte	0x40, 0xe1, 0xe0, 0x40
	.byte	0x40, 0x40, 0x40, 0x40, 0xd2


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
lsr_hint:
	    ; no hint in tournament mode...
	ld	a, (gamecfg)
	cp	#0x01
	ret	z

	    ; check on flag first
	ld	bc, (ptT3Cache)
	ld	a, b
	cp	#0x00
	ld	hl, #(lsrH10)
	jr	z, lsnoton
	ld	hl, #(lsrH11)
lsnoton:
	call	putstrBP

	    ; check the off flag next
	ld	bc,	(ptT3Cache)
	ld	a, c
	cp	#0x00
	ld	hl, #(lsrH00)
	jr	z, lsnotoff
	ld	hl, #(lsrH01)
lsnotoff:
	call	putstrBP

	ret

lsrH11:
	.byte	#C_pink, 0x02, 0x1e, 2
	.ascii	"ON"

lsrH10:
	.byte	#C_red, 0x02, 0x1e, 2
	.ascii	";;"


lsrH01:
	.byte	#C_pink, 0x17, 0x1e, 3
	.ascii	"OFF"

lsrH00:
	.byte	#C_red, 0x17, 0x1e, 3
	.ascii	";;;"
