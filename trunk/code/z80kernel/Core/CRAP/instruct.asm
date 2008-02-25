;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Pac Menu code - instruction screens
;
;  2004-01  Scott "Jerry" Lawrence
;
;  $Id: instruct.asm,v 1.10 2005/04/23 22:35:38 jerry Exp $
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module instruct

	C_instr = C_peach

instrinit:
	call	cls

	    ; set up the sprites to mspac bank
	ld	a, (gamesel)
	push	af			; set aside correct bank

	ld	a, #titlebank
	ld	c, #0x81
	out	(c), a			; switch to graphics bank 

	ld	(gamesel), a		; restore bank variable
	call	anisprite
	pop	af
	ld	(gamesel), a

	call	fullclear

	    ; increment the instruction page
	ld	a, (instrpage)
	inc	a
	and	#0x07
	ld	(instrpage), a
	ret

instrdrw:
	ld	a, (txtoff)
	cp	#TXTOFFLOOPS
	jr	c, .id00
	call	titl_anim		; animate floating sprites
	ret

.id00:
	ld	hl, #(.i_000)		; "INSTRUCTIONS"
	call	putstrBP

	    ; check for "on location"
	ld	a, (operOL)
	cp 	#0x01
	jr	z, .instOL

	    ; display proper text block - home use
	ld	a, (instrpage)
	cp	a, #0x00
	jp	z, 	.in_pause
	cp	a, #0x01
	jp	z, 	.in_menu
	cp	a, #0x02
	jp	z, 	.in_cheat
	cp	a, #0x03
	jp	z, 	.in_fast
	cp	a, #0x04
	jp	z, 	.in_mgfx	; firing is common sense
	cp	a, #0x05
	jp	z, 	.in_quad
	cp	a, #0x06
	jp	z, 	.in_cheat
	jp		.in_mgfx

	    ; display proper text block - on location use
.instOL:
	ld	a, (instrpage)
	cp	a, #0x00
	jp	z, 	.in_pause
	cp	a, #0x01
	jp	z, 	.in_menu
	cp	a, #0x02
	jp	z, 	.in_fast
	cp	a, #0x03
	jp	z, 	.in_quad
	cp	a, #0x04
	jp	z, 	.in_fire
	cp	a, #0x05
	jp	z, 	.in_mgfx
	cp	a, #0x06
	jp	z, 	.in_fast
	jp		.in_menu

.idret:
	call	retroTitle		; display the title
	ret



.i_000: 	.byte	C_yellow, 0xff, 0x02, 12
		.ascii  "INSTRUCTIONS"

.in_pause:
	ld	hl, #(.i1_001)		; PAUSE
	call	putstrBP
	ld	hl, #(.i1_002)		; PAUSE 0
	call	putstrBP
	ld	hl, #(.i1_003)		; PAUSE 1
	call	putstrBP
	ld	hl, #(.i1_004)		; PAUSE 1
	call	putstrBP
	jp	.idret

.i1_001:	.byte	C_cyan, 0xff, 0x06, 7
		.ascii	";PAUSE;"
.i1_002:	.byte	C_instr, 0xff, 0x0a, 17
		.ascii	"PRESS@P1@START@TO"
.i1_003:	.byte	C_instr, 0xff, 0x0c, 20
		.ascii	"PAUSE%@THEN@PRESS@P2"
.i1_004:	.byte   C_instr, 0xff, 0x0e, 16
		.ascii	"START@TO@RESUME%"

.in_menu:
	ld	hl, #(.i2_001)		; MENU
	call	putstrBP
	ld	hl, #(.i2_002)		; MENU 0
	call	putstrBP
	ld	hl, #(.i2_003)		; MENU 1
	call	putstrBP
	ld	hl, #(.i2_004)		; MENU 2
	call	putstrBP
	ld	hl, #(.i2_005)		; MENU 3
	call	putstrBP
	jp	.idret

.i2_001:	.byte	C_cyan, 0xff, 0x06, 6
		.ascii	";MENU;"
.i2_002:	.byte	C_instr, 0xff, 0x0a, 23
		.byte 	0x0c
		.ascii	"@AND@"
		.byte 	0x0d
		.ascii	"@SELECT@GAME%@@@"
.i2_003:	.byte	C_instr, 0xff, 0x0c, 23
		.byte	0x0A
		.ascii	"@AND@"
		.byte	0x0B
		.ascii	"@SELECT@VARIANT%"
.i2_004:	.byte	C_instr, 0xff, 0x0f, 23
		.ascii	"THEN@PRESS@START@BUTTON"
.i2_005:	.byte	C_instr, 0xff, 0x11, 22
		.ascii	"TO@PLAY@SELECTED@GAME%"

.in_cheat:
	ld	hl, #(.i3_001)		; CHEAT
	call	putstrBP
	ld	hl, #(.i3_002)		; 0
	call	putstrBP
	ld	hl, #(.i3_003)		; 1
	call	putstrBP
	ld	hl, #(.i3_004)		; 2
	call	putstrBP
	ld	hl, #(.i3_005)		; 2
	call	putstrBP
	jp	.idret

.i3_001:	.byte	C_cyan, 0x0b, 0x06, 7
		.ascii 	";CHEAT;"
.i3_002: 	.byte	C_instr, 0xff, 0x0a, 17
		.ascii	"HOLD@P1@START@FOR"
.i3_003: 	.byte	C_instr, 0xff, 0x0c, 14
		.ascii	"INVINCIBILITY%"
.i3_004: 	.byte	C_instr, 0xff, 0x0e, 17
		.ascii	"HOLD@P2@START@FOR"
.i3_005: 	.byte	C_instr, 0xff, 0x10, 9
		.ascii	"SPEED;UP%"
	


.in_fast:
	ld	hl, #(.i4_001)		; FAST
	call	putstrBP
	ld	hl, #(.i4_002)		; 0
	call	putstrBP
	ld	hl, #(.i4_003)		; 1
	call	putstrBP
	ld	hl, #(.i4_004)		; 2
	call	putstrBP
	jp	.idret

.i4_001:	.byte 	C_cyan, 0xff, 0x06, 6
		.ascii	";FAST;"
.i4_002:	.byte	C_instr, 0xff, 0x0a, 15
		.ascii	"THE@FAST@OPTION"
.i4_003: 	.byte	C_instr, 0xff, 0x0c, 19
		.ascii	"INCREASES@THE@SPEED"
.i4_004: 	.byte	C_instr, 0xff, 0x0e, 19
		.ascii	"OF@YOUR@PAC;PERSON%" 	; GENDER-NEUTRAL PAC-PERSON!

.in_fire:
	ld	hl, #(.i5_001)		; FIRE
	call	putstrBP
	ld	hl, #(.i5_002)		; 0
	call	putstrBP
	ld	hl, #(.i5_003)		; 1
	call	putstrBP
	ld	hl, #(.i5_004)		; 2
	call	putstrBP
	jp	.idret

.i5_001:	.byte 	C_cyan, 0xff, 0x06, 8
		.ascii	";FIRING;"
.i5_004: 	.byte	C_instr, 0xff, 0x0a, 21
		.ascii	"USE@THE@START@BUTTONS"
.i5_002:	.byte	C_instr, 0xff, 0x0c, 19
		.ascii	"TO@FIRE@YOUR@WEAPON"
.i5_003: 	.byte	C_instr, 0xff, 0x0e, 13
		.ascii	"IN@SOME@GAMES"


.in_quad:
	ld	hl, #(.i6_001)		; QUADBLOK
	call	putstrBP
	ld	hl, #(.i6_002)		; 0
	call	putstrBP
	ld	hl, #(.i6_003)		; 1
	call	putstrBP
	jp	.idret

.i6_001:	.byte 	C_cyan, 0xff, 0x06, 10
		.ascii	";QUADBLOK;"
.i6_002:	.byte	C_instr, 0xff, 0x0a, 23
		.ascii	"P1@OR@UP@ROTATES@PIECE%"
.i6_003: 	.byte	C_instr, 0xff, 0x0d, 23
		.ascii 	"P2@START@CHANGES@MUSIC%"

.in_mgfx:
	ld	hl, #(.i7_001)		; multiple mspac graphics
	call	putstrBP
	ld	hl, #(.i7_002)
	call	putstrBP
	ld	hl, #(.i7_003)
	call	putstrBP
	ld	hl, #(.i7_004)		; left
	call	putstrBP
	ld	hl, #(.i7_005)		; right
	call	putstrBP
	ld	hl, #(.i7_006)		; up
	call	putstrBP
	ld	hl, #(.i7_007)		; down
	call	putstrBP
	jp	.idret

.i7_001:	.byte 	C_cyan, 0xff, 0x06, 16
		.ascii	";MSPAC@GRAPHICS;"
.i7_002:	.byte	C_instr, 0xff, 0x0a, 22
		.ascii	"WITH@START@BUTTON@DOWN"

.i7_003:	.byte	C_instr, 0xff, 0x0c, 16
		.ascii	"HOLD@JOYSTICK%%%"

.i7_004:	.byte	C_instr, 0xff, 0x0f, 22
		.ascii	"@"
		.byte	0x0c
		.ascii	"@FOR@R%I%P%@GRAPHICS"

.i7_005:	.byte	C_instr, 0xff, 0x10, 20
		.byte	0x0d
		.ascii	"@FOR@MS@YAK@PAC@@@@"

.i7_006:	.byte	C_instr, 0xff, 0x11, 20
		.byte	0x0a
		.ascii	"@FOR@AFTER@DARK@@@@"

.i7_007:	.byte	C_instr, 0xff, 0x12, 20
		.byte	0x0b
		.ascii	"@FOR@MINI@GRAPHICS@"
