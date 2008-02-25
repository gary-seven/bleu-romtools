;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Pac Menu code - self test code
;
;  2004-01  Scott "Jerry" Lawrence
;
;  $Id: test.asm,v 1.31 2005/04/23 20:30:47 jerry Exp $
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module selftest

selfTest:
runTests:
	    ; check for test switch, return if not set
	ld	hl, #(svc_port)
	bit	#service, (hl)
	jr	z, doTests

	    ; return -- make sure we weren't testing
	ld	a, (test1)
	cp	#0x00
	ret	z
	    ; first time through...
	call	clrsprite
	xor	a
	out	(#gfxport),a 	; standard pac graphics bank (make sure)
	ld	(test1), a	; clear the flag
	ld	a, #0xff	
	ld	(test2), a	; set for oneshot test when returning
	call	cls		; clear the screen if we were...
	ret

    t_none	= 0x00
    t_converg	= 0x01
    t_dots	= 0x02
    t_red	= 0x03
    t_green	= 0x04
    t_blue	= 0x05
    t_input	= 0x06
    t_dips	= 0x07
    t_init	= 0x08
    t_gfxbank	= 0x09

    maxtests 	= 0x0a		; should be (last test number)+1
    t_default	= t_init	; where we start out

doTests:

	ld	a, (test1)
	cp	#t_none
	jr	nz, checktest

	call	clrsprite	; clear sprite registers
	xor	a
	ld	(sounden), a	; mute

	ld	a, #t_default
	ld	(test1), a
	xor	a
	ld	(test2), a

checktest:
	ld	a, (test1)
	cp	#t_input
	jp	z, testinp		; input test
	cp	#t_dips
	jp	z, testdips		; dipswitch test
	cp	#t_init
	jp	z, testinit		; ram init routines
	cp	#t_gfxbank
	jp	z, testgfxbank		; graphics bank tests

	    ; do a one-shot check
	ld	a, (test2)
	cp	#0x00
	jp	nz, .testInChk
	inc	a
	ld	(test2), a

	    ; perform the one-shot tests
	ld	a, (test1)
	cp	#t_converg
	jp	z, convergence		; convergence test
	cp	#t_dots
	jp	z, dots			; dots test
	cp	#t_red
	jp	z, testred		; red screen
	cp	#t_green
	jp	z, testgreen		; green screen
	cp	#t_blue
	jp	z, testblue		; blue screen

	; fall through - should never get here, but what the heck.
	call	cls

.testInChk:
	ld	hl, #(start_port)	; check for input 
	bit	#start1, (hl)		; P1 and P2 start to advance
	jp 	nz, runTests
	bit	#start2, (hl)
	jp 	nz, runTests

	    ; perform the test advance 
	xor	a
	ld	(test2), a		; reset assisting flags
	ld	a, (test1)		; inc a, return it
	inc	a
	cp	#(maxtests)		; if a > maxtests, a=0
	jr	nz, .ct00
	ld	a, #0x01
.ct00:
	ld	(test1), a

.ctdb:	    ; debounce
	ld	hl, #(start_port)	; check for input 
	bit	#start1, (hl)
	jp	z, .ctdb
	bit	#start2, (hl)
	jp	z, .ctdb

	    ; final cleanup 
	xor	a
	out	(#gfxport),a 		; reset to mspac-test bank
	call	clrsprite		; clear sprites

	jp	runTests


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	convergence test
;
;	display the standard crosshatch test

	.t_NE	= 0x3c
	.t_SE	= 0x3d
	.t_NW	= 0x3e
	.t_SW	= 0x3f
		; 0xc0	- solid
		; 0x15  - megapellet
	.t_REG	= 0xc0	; 0x15

convergence:
	; set white
	ld	hl, #(colram)
        ld      a, #0x0f        ; clear the screen to 0x0f
        ld      b, #0x04        ; need to set 256 bytes 4 times.
        call    memsetN         ; do it.

	ld	hl, #(vidram + 0x40)

	    ; draw the main body
	ld	b, #0x0e
.ct1:
	push	bc
	ld	d, #(.t_NE)
	ld	e, #(.t_SE)
	call	.ctDRAW
	ld	d, #(.t_NW)
	ld	e, #(.t_SW)
	call	.ctDRAW
	pop	bc
	djnz	.ct1

	    ; draw the top and bottom bars
	ld	a, #(C_red)		; use the white pellets
	ld	hl, #(.ctBLANK)
	ld	bc, #0x0000
	call	putstrA
	ld	bc, #0x0003
	call	putstrA

	    ; draw the titles
	ld	hl, #(.ctTITLE)
.T0ITLEDRAW:
	ld	b, #0x00
.TITLEDRAW:
	ld	c, #0x02
	ld	a, #(C_white)
	call	putstrA
	ld	hl, #(.ctPRESS)
	ld	bc, #0x0001
	call	putstrA

.TDSKIP:
	jp	.testInChk

.ctPRESS:
	.byte	28
	.ascii	"@PRESS@P1;P2@START@FOR@NEXT@"

.ctTITLE:
	.byte	28
	.ascii  "@@@@@@CONVERGENCE@TEST@@@@@@"

.ctBLANK:
	.byte	28
	.byte	0x11	; 0x15 
	.ascii	"@@@@@@@@@@@@@@@@@@@@@@@@@@"
	.byte	0x11	; 0x15


; draw the pair of bytes
.ctDRAW:
	ld	b, #0x10
.ctD0:
	ld	(hl), d
	inc	hl
	ld	(hl), e
	inc	hl
	djnz	.ctD0
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	dot test

	.t_DOTS = 0x60


dots:
	; set white
	ld	hl, #(colram)
        ld      a, #0x01        ; color 0x01
        ld      b, #0x04        ; need to set 256 bytes 4 times.
        call    memsetN         ; do it.

	ld	hl, #(vidram)
	ld	a, #.t_DOTS	; dot
	ld	b, #0x04	; need to set 256 bytes 4 times.
	call	memsetN

	ld	hl, #(.dTITLE)
	jp	.T0ITLEDRAW

.dTITLE:
	.byte	28
	.ascii  "@@@@@@@SCREEN@DOT@TEST@@@@@@"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	purity test: red, green, blue


	;      	white	yellow	cyan	green	purple	red	blue
	; 0x15 	0x01	0x16	0x18	0x11	0x10	0x0f	0x09
	; 0xc0	0x12	(0x14)	0x16	0x17		0x09	0x01

	.t_white		= 0x12
	.t_yellow	= 0x14
	.t_cyan		= 0x16
	.t_green		= 0x17
	.t_purple	= 0x18
	.t_red		= 0x09
	.t_blue		= 0x01

testred:
	ld	a, #.t_red
	jp	purityFin

testgreen:
	ld	a, #.t_green
	jp	purityFin

testblue:
	ld	a, #.t_blue

purityFin:
	ld	hl, #(colram)
	ld	b, #(0x04)
	call	memsetN

.purF0:
	ld	hl, #(vidram)
	ld	a, #.t_REG
	ld	b, #(0x04)
	call	memsetN

.egg:
    ; egg test - joystick 1 to the right
	ld	hl, #(p1_port)
	bit	#p1_right, (hl)
	jp	nz, .overEgg
	
	call 	egg_pic0		; draw the easter egg

	ld	hl, #(.ctCBLANK)	; blank
	ld	b, #11
	jp	.TDSKIP

.overEgg:
    ; draw the proper text
	ld	a, (test1)
	cp	#t_red
	jp	z, .pfr
	cp	#t_green
	jp	z, .pfg

    ; draw text title
.pfb:
	ld	hl, #(.ctBLUE)		; blue
	ld	b, #11
	jp	.TITLEDRAW

.pfr:
	ld	hl, #(.ctRED)		; red
	ld	b, #12
	jp	.TITLEDRAW

.pfg:
	ld	hl, #(.ctGREEN)		; green
	ld	b, #11
	jp	.TITLEDRAW


.ctRED:
	.byte	5
	.ascii	"@RED@"

.ctGREEN:
	.byte	7
	.ascii	"@GREEN@"

.ctBLUE:
	.byte	6
	.ascii	"@BLUE@"

.ctCBLANK:
	.byte	7
	.ascii	"@@@@@@@"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	testinp: test input controls

testinp:
	    ; do a one-shot check
	ld	a, (test2)
	cp	#0x00
	jp	nz, .testIn0
	ld	a, #0x01
	ld	(test2), a
	call	cls			; one shot clear screen

.testIn0:
	    ; display controls
	ld	a, #(C_cyan)
	ld	hl, #(.ctI_p1)
	ld	bc, #0x0404
	call	putstrB
	ld	hl, #(.ctI_p2)
	ld	b, #0x10
	call	putstrB

	    ; joystick centers
	ld	a, #0x09		; dark blue
	ld	hl, #(.ctI_C)
	ld	bc, #0x0708
	call	putstrB
	ld	b, #0x13
	call	putstrB

	    ; draw inputs based on real input
	ld	hl, #(p1_port)

	ld	bc, #0x0707		; P1 up
	bit	#p1_up, (hl)
	call	.ti_DRWDOT

	ld	c, #0x09		; P1 down
	bit	#p1_down, (hl)
	call	.ti_DRWDOT

	ld	bc, #0x0608		; P1 left
	bit	#p1_left, (hl)
	call	.ti_DRWDOT

	ld	b, #0x08		; P1 right
	bit	#p1_right, (hl)
	call	.ti_DRWDOT

	ld	hl, #(p2_port)

	ld	bc, #0x1307		; P2 up
	bit	#p2_up, (hl)
	call	.ti_DRWDOT

	ld	c, #0x09		; P2 down
	bit	#p2_down, (hl)
	call	.ti_DRWDOT

	ld	bc, #0x1208		; P2 left
	bit	#p2_left, (hl)
	call	.ti_DRWDOT

	ld	b, #0x14		; P2 right
	bit	#p2_right, (hl)
	call	.ti_DRWDOT

	    ; start 1
	ld	hl, #(start_port)
	bit	#start1, (hl)
	ld	hl, #(.ct_st)
	ld	bc, #0x050b
	call	.tdraw

	    ; start 2
	ld	hl, #(start_port)
	bit	#start2, (hl)
	ld	hl, #(.ct_st)
	ld	b, #0x11
	call	.tdraw

	    ; coin 1
	ld	hl, #(coin_port)
	bit	#coin1, (hl)
	ld	hl, #(.ct_c1)
	ld	bc, #0x050d
	call	.tdraw

	    ; coin 2
	ld	hl, #(coin_port)
	bit	#coin2, (hl)
	ld	hl, #(.ct_c2)
	ld	bc, #0x110d
	call	.tdraw

	    ; coin 3 (SVC)
	ld	hl, #(coin_port)
	bit	#coin3, (hl)
	ld	hl, #(.ct_c3)
	ld	bc, #0x0a12
	call	.tdraw

	    ; rack
	ld	hl, #(rack_port)
	bit	#racktest, (hl)
	ld	hl, #(.ct_rack)
	ld	bc, #0x0814
	call	.tdraw

	    ; SVC
	ld	hl, #(svc_port)
	bit	#service, (hl)
	ld	hl, #(.ct_svc)
	ld	bc, #0x0a16
	call	.tdraw

	    ; upright/cocktail
	ld	hl, #(cab_port)
	bit	#cabinet, (hl)
	ld	hl, #(.ct_cct)
	ld	bc, #0x0a18
	call	.tdraw

	    ; titles
	ld	hl, #(.ctINPUT)
	ld	b, #0x04
	jp	.TITLEDRAW


.tdraw:
	jr	nz, .td0
	ld	a, #(C_white)
	jr	.td1
.td0:
	ld	a, #(C_blue)
.td1:
	call	putstrB
	ret


.ti_DRWDOT:
	push	hl
	jr	nz, .ti_dd00
	ld	a, #0x01
	ld	hl, #(.ctI_M)
	jr	.ti_dd01
.ti_dd00:
	ld	a, #0x09
	ld	hl, #(.ctI_O)
.ti_dd01:
	call	putstrB
	pop	hl
	ret


.ctINPUT:
	.byte	20
	.ascii	"CONTROLLER@PORT@TEST"

.ctI_p1:
	.byte	8
	.ascii	"PLAYER@1"

.ctI_p2:
	.byte	8
	.ascii	"PLAYER@2"

.ctI_C:
	.byte	0x01, 0x15

.ctI_O:
	.byte	0x01, 0x10

.ctI_M:
	.byte	0x01, 0x15

.ct_st:
	.byte	5
	.ascii	"START"

.ct_c1:
	.byte	5
	.ascii	"COIN1"

.ct_c2:
	.byte	5
	.ascii	"COIN2"

.ct_c3:
	.byte	8
	.ascii	"SVC@COIN"

.ct_rack:
	.byte	12
	.ascii	"RACK@ADVANCE"

.ct_svc:
	.byte	8
	.ascii	"SVC@MODE"

.ct_cct:
	.byte	8
	.ascii	"COCKTAIL"


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	testdips: display dip settings

testdips:
	    ; do a one-shot check
	ld	a, (test2)
	cp	#0x00
	jp	nz, .testDP0
	ld	a, #0x01
	ld	(test2), a
	call	cls			; one shot clear screen

.testDP0:
; & 0x03
; 0x03	2 c per cred
; 0x01	1 c per c
; 0x02  1 c per 2 creds
; 0x00	free play

	    ; credits
	ld	hl, #(.ctC)
	ld	bc, #0x0804
	ld	a, #(C_white)
	call	putstrB

	ld	a, (dsw0)
	and	#0x03

	cp	#0x00
	ld	hl, #(.ctFP)
	jr	z, .tdpcFF

	cp	#0x01
	ld	hl, #(.ct11)
	jr	z, .tdpcFF

	cp	#0x02
	ld	hl, #(.ct12)
	jr	z, .tdpcFF

	ld	hl, #(.ct21)

.tdpcFF:
	ld	a, #(C_yellow)
	ld	b, #0x0e
	call	putstrB
	
; & 0x0c
; 0x00	1 life
; 0x04	2 lives
; 0x08	3
; 0x0c	5
	    ; lives
	ld	hl, #(.ctL)
	ld	bc, #0x0806
	ld	a, #(C_white)
	call	putstrB

	ld	a, (dsw0)
	and	#0x0c

	cp	#0x00
	ld	hl, #(.ctL1)
	jr	z, .tdlFF

	cp	#0x04
	ld	hl, #(.ctL2)
	jr	z, .tdlFF
	
	cp	#0x08
	ld	hl, #(.ctL3)
	jr	z, .tdlFF
	
	ld	hl, #(.ctL5)
	
.tdlFF:
	ld	a, #(C_yellow)
	ld	b, #0x0e
	call	putstrB


; & 0x30
; 0x00	10k
; 0x10	15k
; 0x20	20k
; 0x30	none
	    ; extra life
	ld	hl, #(.ctB)
	ld	bc, #0x0808
	ld	a, #(C_white)
	call	putstrB

	ld	a, (dsw0)
	and	#0x30

	cp	#0x00
	ld	hl, #(.ctB10)
	jr	z, .tdbFF

	cp	#0x10
	ld	hl, #(.ctB15)
	jr	z, .tdbFF

	cp	#0x20
	ld	hl, #(.ctB20)
	jr	z, .tdbFF

	ld	hl, #(.ctBN)

.tdbFF:
	ld	a, #(C_yellow)
	ld	b, #0x0e
	call	putstrB

; & 0x40
; 0x00 normal
; 0x40 hard
	    ; difficulty

	ld	hl, #(.ctD)
	ld	bc, #0x030a
	ld	a, #(C_white)
	call	putstrB

	ld	a, (dsw0)
	and	#0x40

	cp	#0x00
	ld	hl, #(.ctDH)
	jr	z, .tddFF

	ld	hl, #(.ctDN)

.tddFF:
	ld	a, #(C_yellow)
	ld	b, #0x0e
	call	putstrB

; & 0x80
; 0x00 normal
; 0x80 alt ghost names

	    ; ghost names
	ld	hl, #(.ctG)
	ld	bc, #0x020c
	ld	a, #(C_white)
	call	putstrB

	ld	a, (dsw0)
	and	#0x80

	cp	#0x00
	ld	hl, #(.ctGN)
	jr	z, .tdgFF
	
	ld	hl, #(.ctGA)

.tdgFF:
	ld	a, #(C_yellow)
	ld	b, #0x0e
	call	putstrB


	    ; titles
	ld	hl, #(.ctDIPS)
	ld	b, #0x04
	jp	.TITLEDRAW


.ctDIPS:
	.byte	19
	.ascii	"DIP@SWITCH@SETTINGS"

.ctC:
	.byte	5
	.ascii	"COINS"
.ct21:
	.byte	13
	.ascii	"2@PER@CRED@@@"
.ct11:
	.byte	13
	.ascii	"1@PER@CRED@@@"
.ct12:
	.byte	13
	.ascii	"1@PER@2@CREDS"
.ctFP:
	.byte	13
	.ascii	"FREE@PLAY@@@@"

.ctL:
	.byte	5
	.ascii	"LIVES"
.ctL1:
	.byte	1
	.ascii	"1"
.ctL2:
	.byte	1
	.ascii	"2"
.ctL3:
	.byte	1
	.ascii	"3"
.ctL5:
	.byte	1
	.ascii	"5"

.ctB:
	.byte	5
	.ascii	"BONUS"
.ctB10:
	.byte	5
	.ascii	"10000"
.ctB15:
	.byte	5
	.ascii	"15000"
.ctB20:
	.byte	5
	.ascii	"20000"
.ctBN:
	.byte	5
	.ascii	"NONE@"

.ctD:
	.byte	10
	.ascii	"DIFFICULTY"
.ctDN:
	.byte	6
	.ascii	"NORMAL"
.ctDH:
	.byte	6
	.ascii	"HARD@@"

.ctG:
	.byte	11
	.ascii	"GHOST@NAMES"
.ctGN:
	.byte	9
	.ascii	"NORMAL@@@"
.ctGA:
	.byte	9
	.ascii	"ALTERNATE"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	testinit: pp30 ram init mechanism

testinit:
	    ; do a one-shot check
	ld	a, (test2)
	cp	#0x00
	jp	nz, .testiDP0
	ld	a, #0x01
	ld	(test2), a
	call	cls			; one shot clear screen
.testiDP0:

	; coins:
	; 1p games played:
	; 2p games played:
	; hold up and press P2 start to clear ram

	ld	hl, #(.tiCREDS)
	call	putstrBP
	ld	bc, #0x0f04
	call	xy2offsB
	push	hl
	pop	bc
	ld	a, (credmem)
	cp	#0xff
	jr	z, .tDP000

	ld	d, a
	ld	e, #C_yellow
	call	prtBCDbyte
	jr	.tDP001

.tDP000:
	ld	hl, #(.tiFREEP)
	call	putstrBP

.tDP001:
	ld	hl, #(.tiCOINS)
	call	putstrBP
	ld	bc, #0x1106
	ld	hl, #(opercoins)
	ld	e, #C_yellow
	call	prtBCDword

	ld	hl, #(.tiP1P)
	call	putstrBP
	ld	bc, #0x1108
	ld	hl, #(oper1games)
	call	prtBCDword

	ld	hl, #(.tiP2P)
	call	putstrBP
	ld	bc, #0x110a
	ld	hl, #(oper2games)
	call	prtBCDword

	ld	hl, #(.tiBUYIN)
	call	putstrBP
	ld	bc, #0x110c
	ld	hl, #(operbuyin)
	call	prtBCDword

	ld	hl, #(.tiPONS)
	call	putstrBP
	ld	bc, #0x110e
	ld	hl, #(operpups)
	call	prtBCDword

	ld	hl, #(.tiOL)
	call	putstrBP
	ld	hl, #(.tiOLctrl)
	call	putstrBP
	ld	a, (operOL)
	cp	#0x00
	jr	nz, ocOL
	ld	hl, #(.tiOLN)
	call	putstrBP
	jr	ocCONT
ocOL:
	ld	hl, #(.tiOLY)
	call	putstrBP
	

ocCONT:
	ld	hl, #(.tiSIG)
	call	putstrBP

	ld	a, #SIGLEN
	ld	(sigstr), a		; force the length to be correct

        ld      a, #(C_yellow)
        ld      hl, #(sigstr)		; display the actual RAM signature
        ld      bc, #0x0e14
        call    putstrB

	xor	a
	out	(#gfxport),a 	; standard pac graphics bank (make sure)

	ld	hl, #(.tiMSG1)		; press P2/Left....
	call	putstrBP
	ld	hl, #(.tiMSG2)
	call	putstrBP
	ld	hl, #(.tiMSG3)
	call	putstrBP
	ld	hl, #(.tiMSG4)
	call	putstrBP

	    ; check for reset (left and 2p)
	ld	hl, #(start_port)
	bit	#start2, (hl)
	jr	nz, .ti000		; p2 not pressed, skip
	ld	hl, #(p1_port)
	bit	#p1_left, (hl)
	jr	nz, .ti000		; left not pressed, skip

	call 	.prtRelease 		; update the screen...
	ld	hl, #(p1_port)		; wait for l to be released
.ti002:
	bit	#p1_left, (hl)
	jr	z, .ti002
	call	cls			; clear screen
	call	.tiRESET		; perform the reset

.ti000:
	    ; check for input to toggle the on location flag
	ld	hl, #(p1_port)
	bit	#p1_right, (hl)
	jr	nz, .ti999		; right not pressed, skip

	call 	.prtRelease 		; update the screen...
	ld	hl, #(p1_port)
.ti001:
	bit	#p1_right, (hl)
	jr	z, .ti001		; wait for release
	call	cls			; clear screen
	ld	a, (operOL)
	xor	#0x01			; cpl would work for all bits
	ld	(operOL), a
	;jr	.ti999

.ti999:
	    ; titles
	ld	hl, #(.tiTITLE)
	ld	b, #0x00
	jp	.TITLEDRAW

.prtRelease:
	call	cls			; clear screen
	ld	hl, #(.tiRLS)
	call	putstrBP		; release...
	ret


.tiTITLE:
	.byte	28
	.ascii  "@@@@@@@BOOKKEEPING@RAM@@@@@@"

.tiCREDS:
	.byte	C_white, 0x01, 0x04, 12
	.ascii	"CREDITS@LEFT"

.tiFREEP:
	.byte	C_yellow, 0x0e, 0x04, 2
	.ascii 	";;"

.tiCOINS:
	.byte	C_white, 0x03, 0x06, 10
	.ascii	"COIN@DROPS"

.tiP1P:
	.byte	C_white, 0x05, 0x08, 8
	.ascii	"1P@GAMES"

.tiP2P:
	.byte	C_white, 0x05, 0x0a, 8
	.ascii	"2P@GAMES"

.tiBUYIN:
	.byte	C_white, 0x06, 0x0c, 7
	.ascii 	"BUY;INS"

.tiPONS:
	.byte	C_white, 0x04, 0x0e, 9
	.ascii	"POWER;ONS"

.tiOL:
	.byte	C_white, 0x02, 0x10, 11
	.ascii	"ON@LOCATION"

.tiOLY:
	.byte	C_yellow, 0x0e, 0x10, 3
	.ascii	"YES"

.tiOLN:
	.byte	C_yellow, 0x0e, 0x10, 3
	.ascii	"NO@"

.tiOLctrl:
	.byte	C_cyan, 0x014, 0x10, 7
	.byte	0x0d
	.ascii	"RIGHT"
	.byte	0x0d

.tiSIG:
	.byte	C_white, 0x03, 0x14, 10
	.ascii	"RELEASE@ID"

.tiMSG1:
	.byte	C_orange, 0xFF, 0x17, 22
	.ascii	"HOLD@P2@START@AND@MOVE"
.tiMSG2:
	.byte	C_orange, 0xFF, 0x19, 26
	.ascii	"JOYSTICK@LEFT@TO@CLEAR@ALL"
.tiMSG3:
	.byte	C_orange, 0xFF, 0x1b, 25
	.ascii	"STORED@VALUES@AND@SCORES%"

.tiMSG4:
	.byte	C_orange, 0xFF, 0x1d, 23
	.ascii	"THEN@RESET@THE@MACHINE%"

.tiRLS:
	.byte	C_yellow, 0xFF, 0x10, 21
	.ascii	"RELEASE@THE@CONTROLS%"

    ; .tiRESET
    ;   this resets all of the user values
.tiRESET:
	ld	bc, #0x0000
	ld	(opercoins), bc
	ld	(oper1games), bc
	ld	(oper2games), bc
	xor	a
	ld	(operOL), a

	xor	a
	ld	b, #0x10		; all of it
	ld	hl, #pp30ram
	call	memsetN

;	ld	b, #0x04
;	ld	hl, #pp30ram2
;	call	memsetN

	    ; initialize the signature ram
	ld	ix, #(SIGROM)
	ld	iy, #(signature)
	ld	b, #SIGLEN
.sg1:
	ld	a, (ix)
	ld	(iy), a
	inc	ix
	inc	iy
	djnz	.sg1

	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	testgfxbank: pp30 graphics bank switch tests
testgfxbank:
	    ; do a one-shot check
	ld	a, (test2)
	cp	#0x00
	jp	nz, .testgf0
	ld	a, #0x01
	ld	(test2), a
	call	cls			; one shot clear screen

	xor	a
	ld	(test3), a

	    ; draw stuff to the screen
	ld	hl, #(colram + 256)
        ld      a, #C_red
        ld      b, #0x03        ; need to set 256 bytes 4 times.
        call    memsetN         ; do it.

	    ; blank the top and bottom of the screen
	ld	hl, #colram
	ld	a, #0x00
	ld	b, #0x40
	call	memset256

	ld	hl, #colram + 0x3c0
	ld	b, #0x40
	call	memset256

	    ; fill the screen with character 00-ff lots of times
	ld	hl, #(vidram)
	ld	b, #4
.tg001:
	push	bc
	ld	b, #0x00
.tg002:
	ld	a, b
	ld	(hl), a
	inc	hl
	djnz	.tg002

	pop	bc
	djnz	.tg001

	; main (non one-shot) routine:
.testgf0:
	ld	hl, #(p1_port)		; p1 left and right to change bank
	bit	#p1_left, (hl)
	jr	nz, .tg0JR		; not left, check R

	    ; left pressed...
.tg0J0:					; debounce
	bit	#p1_left, (hl)
	jr	z, .tg0J0		; jump back while pressed

	    ; dec the gfx bank
	ld	a, (test3)
	dec	a
	and	#0x0f
	ld	(test3), a
	out	(#gfxport), a

.tg0JR:
	bit	#p1_right, (hl)
	jr	nz, .tg0JP		; done... print stuff

	    ; right pressed...
.tg0J1:					; debounce
	bit	#p1_right, (hl)
	jr	z, .tg0J1		; jump back while pressed

	    ; inc the gfx bank
	ld	a, (test3)
	inc	a
	and	#0x0f
	ld	(test3), a
	out	(#gfxport), a

.tg0JP:
	    ; print bank number
	ld	a, (test3)
	and	#0x0f
	ld	d, a
	ld 	e, #C_yellow
	ld	bc, #0x01f5
	call	prtBCDbyte

	ld	hl, #(.tgINFO1)
	call	putstrBP		; Joystick left and right
	ld	hl, #(.tgINFO2)
	call	putstrBP		; to change gfx rom.

	    ; draw some sprites 64 total
	ld	bc, #0x30e0		; XY position
	ld	hl, #0x0000		; sprite offset = 0
	ld	de, #(sprite0)
	call	.tgSPRITE

	ld	bc, #0x30b0
	ld	hl, #0x0040
	ld	de, #(sprite1)
	call	.tgSPRITE

	ld	bc, #0x3050
	ld	hl, #0x0080
	ld	de, #(sprite2)
	call	.tgSPRITE

	ld	bc, #0x3020
	ld	hl, #0x00c0
	ld	de, #(sprite3)
	call	.tgSPRITE

	    ; titles
	ld	hl, #(.ctGFX)
	ld	b, #0x04
	jp	.TITLEDRAW

.ctGFX:
	.byte	20
	.ascii	"GFX@BANK@SWITCH@TEST"

.tgINFO1:
	.byte	C_yellow, 0xff, 0x10, 23
	.ascii	"JOYSTICK@LEFT@AND@RIGHT"

.tgINFO2:
	.byte	C_yellow, 0xff, 0x12, 23
	.ascii	"TO@CHANGE@GRAPHICS@ROM%"

.tgSPRITE:
	ld	iy, #(spritecoords)
	add	iy, de
	ld	spriteX(iy), b
	ld	spriteY(iy), c

	ld	ix, #(sprtbase)
	add	ix, de
	ld	bc, (timer)
	ld	a, c
	rra
	rra
	and	#0x3c			; clear bottom two bits (flip)
	add	l
	ld	sprtIndex(ix), a
	ld	a, #0x01		; color (red)
	ld	sprtColor(ix), a
	ret

.if( 0 )
.tgTAB:
	.word	.tg_mspac	; 0
	.word	.tg_pac		; 1
	.word	.tg_plus	; 2
	.word	.tg_piranha	; 3
	.word	.tg_wavy	; 4
	.word	.tg_magfx	; 5
	.word	.tg_aa		; 6
	.word	.tg_bf		; 7
	.word	.tg_lw		; 8
	.word	.tg_tnt		; 9
	.word	.tg_eggor	; A
	.word	.tg_pengo	; B
	.word 	.tg_crush	; C
	.word	.tg_jump	; D
	.word	.tg_un		; E
	.word	.tg_un		; F
	
.tg_mspac:
	.byte	C_yellow, 0xff, 0x17, 8
	.ascii	"MSPACMAN"
.endif
