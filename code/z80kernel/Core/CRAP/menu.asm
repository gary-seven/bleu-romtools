;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Pac Menu code - menu code
;
;  2004-01,02  Scott "Jerry" Lawrence
;
;  $Id: menu.asm,v 1.93 2005/04/24 01:12:11 jerry Exp $
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module menu.asm

; for when the ram has changed in the firmware; 
; be sure to increment this, or change it. 
;  Only the first 12 bytes are used.

SIGROM:                                         ; 12 byte signature
        .ascii  "YORGLE@0424B"
	;        ----____----

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  Mode switching calls and callbacks.

; (thismode) modes
	M_1MENU		= 0x00
	M_MENU		= 0x01
	M_1TITLE	= 0x02
	M_TITLE		= 0x03
	M_1INSTR	= 0x04
	M_INSTR		= 0x05

	M_GAME		= 0xff

modeGAME:
	call	atClear
	ld	a, #M_GAME
	ld	(thismode), a
	ret

modeNORM:
	ld	a, #M_1MENU
	ld	(thismode), a
	ret


	; force the menu to redisplay (turns off title/instruct/screensaver)
	; call this to break out of the screensaver immediately
	; queues up the screensaver to be re-enabled
tt_MENU:
	    ; check if we're in menumode
	ld	a, (thismode)		; see if we're in the menu now
	cp	#M_MENU
	jr	z, tt_scr		; already in menu, refresh the timeout
	    ; nope; switch
	call	atClear			; reset the clear mechanism
	ld	a, #M_1MENU
	ld	(thismode), a		; set up for menu one-shot
	ld	a, (gfxbank)		; restore graphics bank
	out	(#gfxport),a
	ret

	; initialize for screensaver from current time
	; queues up the screensaver to start
tt_scr:
	ld	bc, #to_M2SCR
	ld	de, #(.CBscr)
	call	atDelta			; set the screen saver timeout
	ret

	; screensaver callback.
	; resets some mode flags, turns on title mode
.CBscr:
	xor	a
	ld	(watchdog), a
	ld	(txtoff), a		; reset text off flag
	ld	a, #M_1TITLE
	ld	(thismode), a 		; queue up the mode change
	call	tt_stoi			; queue up the instruction change
	ret

	; switch from title screen to instructions
tt_stoi:
	ld	bc, #to_SCR2I
	ld	de, #(.CBstoi)
	call	atDelta			; queue up the mode change
	ret

	; screen -> instructions callback.  
.CBstoi:
	ld	a, #M_1INSTR
	ld	(thismode), a		; change to the instruction screen
	ret


    ; switch from instrucitons to title screen
tt_itos:
	ld	bc, #to_I2SCR
	ld	de, #(.CBitos)
	call	atDelta			; queue up the mode change
	ret

.CBitos:
	ld	a, #M_1TITLE
	ld	(thismode), a		; change to the title screen
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; main routine

main::
menu:
	cp 	#SOFTBOOT		; check for soft boot
	jp	z, softmain		; we're doing a reboot, not powerup

    ; first, we check the RAM signature, to see if it has gotten
    ; corrupted.
	xor	a
	ld	(watchdog), a

	ld	ix, #(SIGROM)
	ld	iy, #(signature)
	ld	b, #SIGLEN
.sg0:
	ld	a, (ix)			; a = ROM SIGNATURE[b]
	ld	d, a			; d = a
	ld	a, (iy)			; a = RAM SIGNATURE[b]
	cp	d			; (d == a)?
	jr	nz, resSig		; not the same, reset it all.
	inc	ix			; ix++
	inc	iy			; iy++
	djnz	.sg0			; if b-->0, loop
	
	jr	hardBook		; skip to the rest of hard boot

resSig:
	;call	.tiRESET		; clear all ram, initialize it
	xor	a
	ld	(watchdog), a

sigrecheck:
	ld	ix, #(SIGROM)
	ld	iy, #(signature)
	ld	b, #SIGLEN
.sg2:
	ld	a, (ix)			; a = ROM SIGNATURE[b]
	ld	d, a			; d = a
	ld	a, (iy)			; a = RAM SIGNATURE[b]
	cp	d			; (d == a)?
	jr	nz, resERROR		; not the same, fail on error
	inc	ix			; ix++
	inc	iy			; iy++
	djnz	.sg2			; if b-->0, loop

	    ; tell the user
	xor	a
	ld	(watchdog), a
	call	cls
	ld	hl, #(.TRESRAM)		; tell the user the ram was formatted
	call	putstrBP
	ld	bc, #120		; 2 seconds
	call	sleep			; I SLEEP NOW

	jr	hardBook		; resume normal boot

resERROR:
	call	cls
	ld	hl, #(.TRESERR)		; display error text
	call	putstrBP
.resLP:
	xor	a
	ld	(watchdog), a
	jr	.resLP			; loop forever.

.TRESRAM:
	.byte	C_yellow, 0xff, 0x10, 15
	.ascii	"NVRAM@FORMATTED"

.TRESERR:
	.byte	C_yellow, 0xff, 0x10, 18
	.ascii	"NVRAM@FORMAT@ERROR"

hardBook:
	xor	a
	ld	(watchdog), a

	    ; adjust bookkeeping - increment powerups
	ld	bc, (operpups)
	call	incBCDbc
	ld	(operpups), bc

	xor	a
	ld	(watchdog), a

	    ; play a tune
	;call	fansnd

	    ; current mode variable
	xor	a
	ld	(watchdog), a
	ld	(thismode), a
	ld	(instrpage), a

	    ; force test mode to reset properly
	;ld	(test1), a		; don't clear test mode
	ld	(test2), a		; force test-mode oneshot
	ld	(test3), a		; force gfx bank 0, to sync up

	    ; next mode trigger variables
	call	modeNORM

.if 0
	    ;; Initialize the PP hardware...
	    ; default to pac-man graphics set
	xor	a
	ld	(watchdog), a
	out	(#gfxport),a

	    ; initialzie the menu variables
	xor	a
	ld	(watchdog), a
	ld	(gamesel), a
	ld	(ogamesel), a
	ld	(gamecfg), a

	    ; initalize test mode settings
	ld	(test1), a
	ld	a, #0xff
	ld	(test2), a
.endif


softmain:
	push	af
	    ;clear the screen
	xor	a
	ld	(watchdog), a
	call	cls

	    ; normal mode
	call	modeNORM
	call	tt_MENU			; disable screen saver

	    ; initialize the timer variables
	xor	a
	ld	(watchdog), a
	ld	(timer), a
	ld	(timer+1), a
	call 	dip
	call	tt_scr

	ld	a, (gfxbank)		; restore graphics bank
	out	(#gfxport),a

	pop	af

	cp 	#SOFTBOOT		; check for soft boot
	jp	mainloop

	; XXXXXX REMOVE THIS CRAP!
	jp	nz, mainloop		; we're doing a hard boot, skip

	ld	hl, #(.GAMEOVER)
	ld	bc, #0x0a10
	ld	a, #(C_yellow)
	call	putstrB
	ld	bc, #20
	call	sleep

	jp	mainloop

.GAMEOVER:
        .byte   9
        .ascii  "GAME@OVER"



; dip
;	initialize the ram variables from the dip switches
;	- this sets up the difficulty variable, coinxcc, credxcc
dip:
	    ; initialize difficulty from dip switch
	ld	a, (dsw0)	; dips
	and	#0x40
	cp	#0x00
	ld	a, #diffNORM	; ((dsw0 & 0x40) == 0)? NORM : HARD
	jr	nz, .dip1
	ld	a, #diffHARD
.dip1:
	ld	(difficulty), a

	    ; initialize coin info from dip switch
	xor 	a
	ld	(watchdog), a
	;ld	(credmem), a	; zero credits NAH!
	ld	(extrcoin), a	; zero extra coins
	ld	a, (dsw0)	; dips
	ld	b, a
	and	#0x03		; mask free play
	jr	nz, .dip2	; nope.  not freeplay
	ld	hl, #(credmem)
	ld	(hl), #(0xff)	; FREE PLAY flag
.dip2:
	ld	c, a
	rra
	adc	a, #0x00
	ld	(coinxcc), a	; coin part of coins-per-creds
	and	#0x02
	xor	c
	ld	(credxcc), a	; credit part of coins-per-creds
	ret



mainloop:
	call	selfTest		; this will run the tests, and hang
					; in there while running the tests

	; when we've returned here, we're not in test mode
.if	DEBUG
	ld	hl, #timer
	ld	e, #(C_yellow)
	ld	bc, #0x27
	call	prtABCDword

	; print the hs save addr
	ld	hl, #hsRamAddr
	ld	e, #(C_yellow)
	ld	bc, #0x007
	call	prtABCDword
.endif

	    ; do different things based on mode
	ld	a, (thismode)

	cp	#M_1MENU
	jr	z, mode1menu
	cp	#M_MENU
	jr	z, mode_menu

	cp	#M_1TITLE
	jr	z, mode1title
	cp	#M_TITLE
	jr	z, mode_title

	cp	#M_1INSTR
	jr	z, mode1instr
	cp	#M_INSTR
	jr	z, mode_instr
	; put more modes here.

mode1instr:
	call	instrinit
	ld	a, #M_INSTR
	ld	(thismode), a
	call	tt_itos
	jr	mode_common

mode_instr:
	call	instrdrw	; render the screen (instructions)
	call	inputcheck	; check for user input (run here to kill glitch)
	jr	mode_common

mode1title:
	call	titleinit
	ld	a, #M_TITLE
	ld	(thismode), a
	call	tt_stoi
	jr	mode_common

mode_title:
	call	titledrw	; render the screen (title, sprites)
	call	inputcheck	; check for user input (run here to kill glitch)
	jr	mode_common

mode1menu:
	ld	a, (ogamesel)
	ld	(gamesel), a		; restore the game selector

	ld	a, (gfxbank)		; restore graphics bank
	out	(#gfxport), a
					; XXXX SHOULD RESTORE BANK NUMBER TOO
	call	cls

	ld	a, #M_MENU
	ld	(thismode), a
	call	tt_scr
	jr	mode_common

mode_menu:
	call 	anisprite	; animate the sprites
	call	inputcheck	; check for user input
	call	headinfo	; display all of the screen info
	call	creddisp	; display credits

mode_common:
	    ; common stuff
	call	coincheck	; check for coin drops
	call	startgame	; check for start game

	    ; hug a refresh, do it again...
	ld	bc, #1
	call	sleep
        jp      mainloop




	; check to see if the bank selected in 'a' is valid or not.
	; if it is valid, it set z
isValid:
	ld 	e, a			; store a aside
	ld	d, #0x00
	push	de			; push 0x00AA onto stack
	push	hl
	ld	hl, #(romcfg)
	rst	0x10			; hl = base of selected config
	ld 	de, #rs_flags
	add	hl, de
	ld	a, (hl)			; a contains this bank's flags
	ld	e, a			; store flags aside

	ld	a, (operOL)
	cp	#0x00
	ld	a, e			; restore a to bank's flags
	jr	nz, .ivOL
	    ; home use
	and	#rsf_DISABLE
	jr	.iv0002

.ivOL:      ; on location
	and	#rsf_OLDISABLE
	
.iv0002:
	pop	hl
	pop	de			; restore de (a in e)
	ld	a, e			; a = e
	ret


	; anisprite
	;  draw the correct sprites as per the structure
anisprite:
	    ; dereference the current config...
	xor 	a
	ld	(watchdog), a

	ld	a, (gamesel)
	ld	d, #0
	ld	e, a
	ld	hl, #(romcfg)		; full game array
	rst	0x10			; load HL with base of current config
	push	hl
	push	hl
	push	hl

	ld	de, #rs_a1		; anim array 1
	rst	0x10			; hl contains array 1
	ld	de, #(sprite1)		; sprite1
	ld	bc, #0xe078		; left top
	call	doasprite

	pop	hl
	ld	de, #rs_a2		; anim array 2
	rst	0x10			; hl contains array 2
	ld	de, #(sprite2)		; sprite2
	ld	bc, #0x2d78		; right top
	call	doasprite

	pop	hl
	ld	de, #rs_a3		; anim array 3
	rst	0x10			; hl contains array 3
	ld	de, #(sprite3)		; sprite3
	ld	bc, #0xe052		; left bottom
	call	doasprite

	pop	hl
	ld	de, #rs_a4		; anim array 4
	rst	0x10			; hl contains array 4
	ld	de, #(sprite4)		; sprite4
	ld	bc, #0x2d52  		; right bottom 
	call	doasprite

	ret


;	doasprite
;	 draw a sprite to the screen
;	 b  = x position
;	  c = y position
;	 de = hardware sprite slot number
;	 hl = animation array
doasprite:
	push	af
	xor 	a
	ld	(watchdog), a
	pop	af

	    ; set up x and y
	ld	iy, #(spritecoords)
	add	iy, de

        ld      spriteX(iy), b		; X
        ld      spriteY(iy), c		; Y


	    ; set up sprite color and frame
        ld      ix, #(sprtbase)
	add	ix, de

	    ; animation frame based on timer
	ld	bc, (timer)
	ld	a, c
	rra
	rra				; add in more of these for slower
	rra
	and	#0x03			; we just want the bottom 3 bits
	ld	b, #0
	ld	c, a 			; bc now contains the array index number


	ld	a, (hl)			; color
        ld      sprtColor(ix), a	; load in the color

	inc	hl			; hl points to beginning of array
	add	hl, bc			; bc now points to the proper element
	ld	a, (hl)			; a is the sprite
    ;; the next three opcodes removed so that we can flip sprites and such
;	rla
;	rla				; bottom two bits are 00 (no flips)	
;	and	#0xfc			; make sure bottom bits are cleared

        ld      sprtIndex(ix), a	; load the sprite number in
	ret



	; creddisp
	;  displays number of credits currently, or "free play"
creddisp:
	ld	a, (credmem)
	cp	#(0xff)		; free play flag
	jr	nz, .cdcreds

.cdfree:
	    ; display "press start" on freeplay mode
	ld	a, (textmode)
	cp	#SPACE20
	ld	hl, #(.cr20PS2)
	jr	z, .cdf000
	cp	#SPACE5b
	ld	hl, #(.cr5bPS2)
	jr	z, .cdf000
	ld	hl, #(.credPS2)
.cdf000:

	ld	a, (gameflag)
	cp	#rsf_p1only
	jr	nz, .cdfreeD

	ld	a, (textmode)
	cp	#SPACE20
	ld	hl, #(.cr20PS1)
	jr	z, .cdfreeD
	cp	#SPACE5b
	ld	hl, #(.cr5bPS1)
	jr	z, .cdfreeD
	ld	hl, #(.credPS1)
.cdfreeD:
	ld	a, #(C_orange)
	ld	bc, #0x0002
	call	putstrA		; display "insert coin" or "start" text
	ret

.cdcreds:
	; #credits != 0:  press start
	ld	a, (credmem)
	cp	#0x01
	jr	c, .cdm1
	jr	nz, .cdc2
.cdc1:
	ld	a, (textmode)
	cp	#SPACE20
	ld	hl, #(.cr20PS1)
	jp	z, .ctextdisp		; too far.  jp instead of jr
	cp	#SPACE5b
	ld	hl, #(.cr5bPS1)
	jp	z, .ctextdisp		; too far.  jp instead of jr
	ld	hl, #(.credPS1)
	jp	.ctextdisp		; too far.  jp instead of jr

.cdc2:
	ld	a, (gameflag)
	cp	#rsf_p1only		; 1 player only?
	jr	z, .cdc1		; yes. jump back
	ld	a, (textmode)
	cp	#SPACE20
	ld	hl, #(.cr20PS2)
	jr	z, .ctextdisp
	cp	#SPACE5b
	ld	hl, #(.cr5bPS2)
	jr	z, .ctextdisp
	ld	hl, #(.credPS2)
	jr	.ctextdisp


.cdm1:
	;  if #extrcoin > 1	insert 1 coin for 1 credit (1per2)
	ld	a, (extrcoin)
	cp	#0x01
	jr	c, cdm2
	ld	a, (textmode)
	cp	#SPACE20
	ld	hl, #(.cr2011)
	jr	z, .ctextdisp
	cp	#SPACE5b
	ld	hl, #(.cr5b11)
	jr	z, .ctextdisp
	ld	hl, #(.cred11)
	jr	.ctextdisp

;         SPACE40        = 0x00          ; pac-games             '@'
;         SPACE20        = 0x01          ; alien armada          ' '
;         SPACE5b        = 0x02          ; beastie feastie       '['


cdm2:
	;  if #credxcc == 2	insert 1 coin for 2 credits
	ld	a, (credxcc)
	cp	#0x02
	jr	nz, cdm3
	ld	a, (textmode)
	cp	#SPACE20
	ld	hl, #(.cr2012)
	jr	z, .ctextdisp
	cp	#SPACE5b
	ld	hl, #(.cr5b12)
	jr	z, .ctextdisp
	ld	hl, #(.cred12)
	jr	.ctextdisp

cdm3:
	;  if #coinxcc == 1     insert 1 coin for 1 credit
	ld	a, (coinxcc)
	cp	#0x01
	jr	nz, cdm4
	ld	a, (textmode)
	cp	#SPACE20
	ld	hl, #(.cr2011)
	jr	z, .ctextdisp
	cp	#SPACE5b
	ld	hl, #(.cr5b11)
	jr	z, .ctextdisp
	ld	hl, #(.cred11)
	jr	.ctextdisp

cdm4:
	;  if #coinxcc == 2	insert 2 coins for 1 credit
	ld	a, (coinxcc)
	cp	#0x02
	ld	a, (textmode)
	cp	#SPACE20
	ld	hl, #(.cr2021)
	jr	z, .ctextdisp
	cp	#SPACE5b
	ld	hl, #(.cr5b21)
	jr	z, .ctextdisp
	ld	hl, #(.cred21)
	jr	.ctextdisp

.ctextdisp:
	ld	a, #(C_orange)
	ld	bc, #0x001f	; b(x)=0  c(y)=20
	call	putstrB		; display "insert coin" or "start" text

.ctext0000:
	; display number of credits
	ld	a, #(C_white)
	ld	hl, #(.creds)
	ld	bc, #0x0203
	call	putstrA

	ld	a, (credmem)
	ld	d, a
	ld	e, #(C_white)	;#0x07	; color (0x0f)
	ld	bc, #0x33
	call	prtBCDbyte	; display number of credits
	ret

credsdisp:
	ld	a, (credmem)
	cp	#(0xff)		; freeplay?
	jr	z, .cd00001	;   then skip
	call	.ctext0000	; above
.cd00001:
	ret
	

.creds:
	.byte 	6
	.ascii	"CREDIT"

.cred11:
	.byte 	0x1c
	.ascii	"@@INSERT@1@COIN@FOR@CREDIT@@"
.cred21:
	.byte 	0x1c
	.ascii	"@INSERT@2@COINS@FOR@CREDIT@@"
.cred12:
	.byte 	0x1c
	.ascii	"@INSERT@COIN@FOR@2@CREDITS@@"
.credPS1:
	.byte	0x1c
	.ascii	"@@@PRESS@P1@START@TO@PLAY@@@"
.credPS2:
	.byte	0x1c
	.ascii	"PRESS@P1@OR@P2@START@TO@PLAY"
.credBLANK:
	.byte	0x19
	.ascii	"@@@@@@@@@@@@@@@@@@@@@@@@@"


.cr2011:
	.byte 	0x1c
	.ascii	"  INSERT 1 COIN FOR CREDIT  "
.cr2021:
	.byte 	0x1c
	.ascii	" INSERT 2 COINS FOR CREDIT  "
.cr2012:
	.byte 	0x1c
	.ascii	" INSERT COIN FOR 2 CREDITS  "
.cr20PS1:
	.byte	0x1c
	.ascii	"   PRESS P1 START TO PLAY   "
.cr20PS2:
	.byte	0x1c
	.ascii	"PRESS P1 OR P2 START TO PLAY"

.cr5b11:
	.byte 	0x1c
	.ascii	"[[INSERT[1[COIN[FOR[CREDIT[["
.cr5b21:
	.byte 	0x1c
	.ascii	"[INSERT[2[COINS[FOR[CREDIT[["
.cr5b12:
	.byte 	0x1c
	.ascii	"[INSERT[COIN[FOR[2[CREDITS[["
.cr5bPS1:
	.byte	0x1c
	.ascii	"[[[PRESS[P1[START[TO[PLAY[[["
.cr5bPS2:
	.byte	0x1c
	.ascii	"PRESS[P1[OR[P2[START[TO[PLAY"
.endtxt0:


	    ; draw the screen -- dereferenced via A
headinfo:
	ld	a, (gamesel)		; draw this one...
	push 	af

.if	DEBUG
	    ; draw the current
	push	af
	ccf
	rrc	a
	ld	d, a
	ld	e, #(C_orange)
	ld	bc, #0x3c2
	call	prtBCDbyte		; print game - DEBUG

	ld	a, (gamecfg)
	ld	d, a
	ld	bc, #0x3e2
	call	prtBCDbyte		; print config - DEBUG

	ld	e, #(C_yellow)
	ld	a, (gameflag)
	ld	d, a
	ld	bc, #0x3e6
	call	prtBCDbyte		; print config - DEBUG

	pop	af
.endif


	pop	af
	ld	hl, #(romcfg)		; full game array
	ld	d, #0
	ld	e, a		; ### DE = index A
	rst	0x10		; dereference the pointer (HL)

	push	hl
	push 	hl		; store it on the stack a few times
	push	hl
	push	hl
	push	hl

	    ; draw the "screenshot"
	ld	de, #rs_chars	; ### 8: video RLE data
	rst	0x10
	ex	de, hl		; load it into DE
	ld	hl, #(vidram + 0x0040)
	call	rledraw

	pop	hl
	ld	de, #rs_color	; ### A: color RLE data
	rst	0x10
	ex	de, hl
	ld	hl, #(colram + 0x0040)
	call	rledraw

	    ; render the screen text	
	pop	hl
	ld	de, #rs_name	; ### game name
	rst	0x10		; dereference the pointer (HL)
	;ld	bc, #0x0013	; b(x)=-  c(y)=15
	ld	bc, #0x0012	; b(x)=-  c(y)=15
	call	textcenter	; center the text
	ld	a, #(C_yellow)
	call	putstrB		; draw in region b

	pop	hl		; restore head of table
	ld	de, #rs_cprt	; ### mfg copyright
	rst	0x10		; dereference the pointer (HL)
	ld	bc, #0x0014	; b(x)=-  c(y)=15
	call	textcenter	; center the text
	ld	a, #(C_red)
	call	putstrB		; draw in region b

	pop	hl		; restore head of table
	;ld	de, #rs_cprt2	; ### mfg copyright line 2
	;rst	0x10		; dereference the pointer (HL)
	;ld	bc, #0x0015	; b(x)=-  c(y)=15
	;call	textcenter	; center the text
	;ld	a, #(C_red)
	;call	putstrB		; draw in region b

	pop	hl		; restore head of table
	push	hl		; store it
	ld	de, #rs_flags	; # grabenn ze flagg
	add	hl, de
	ld	a, (hl)
	ld	(gameflag), a	; set it aside

	pop	hl		; restore head of table

	ld	a, (operOL)	;  select the proper variants based on operOL
	cp	#0x00
	jr	nz, .vselOL
	ld	de, #rs_vars	; home use - regular variants table
	jr	.vselEND

.vselOL:
	ld	de, #rs_olvars	; on location use - limited variants table

.vselEND:
	rst	0x10		; dereference the pointer (HL)
	ld	a, (hl)		; number of strings

	ld	(maxcfgs), a	; store it aside
	ld	b, a		; b gets number of strings

	add	a, #0x16
	ld	d, #0x00	; y position
	ld	e, a


varloop:
	push	bc
	push	hl
	ld	c, e		; move y position into C
	ld	d, #0		; d=0

	ld	a, (gamecfg)	; check for activation color
	inc	a
	cp	b		; see if it's the same
	jr	nz, .varDIM
	ld	a, #(C_ACTIVE)	; active color
	push	af
	jr	.varDRAW

.varDIM:
	ld	a, #(C_VARIANT)
	push	af

.varDRAW:
	ld	a, b		; a = array number offset
	add	a		; a = a*2
	ld	e, a		; e = index
	rst	0x10		; expects: de = offset

	call	textcenter	; expects: bc = X,Y
	pop	af		; restore the set aside color
	call	putstrB
	ld	e, c		; e gets current y position
	dec	e		; decrement y position
	pop	hl
	pop	bc		; restore b
	djnz	varloop

	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	; coin routines
coincheck:
	ld	a, (credmem)	; lockout coins and return if ncreds=99
	cp	#0x99
	rla
	ld	(coinlock), a
	rra
	ret	nc

		; otherwise, accept coins
	ld	hl, #(coin_port)
	bit	#coin1, (hl)
	jr	nz, cc2
cc1db:	; coin1 debounce
	bit	#coin1, (hl)
	jr	z, cc1db
	jr	addcoin

cc2:
	bit	#coin2, (hl)
	jr	nz, cc3
cc2db:	; coin2 debounce
	bit	#coin2, (hl)
	jr	z, cc2db
	jr	addcoin

cc3:
	bit	#coin3, (hl)
	ret	nz
cc3db:	; coin3 debounce
	bit	#coin3, (hl)
	jr	z, cc3db
	; fall through...

addcoin:
	call	randomize

	    ; adjust bookkeeping
	ld	bc, (opercoins)
	call	incBCDbc
	ld	(opercoins), bc

	    ; turn off the 'screensaver' or 'title screen'
	    ; but not if we're in game mode
	ld	a, (thismode)
	cp	#M_GAME
	jr	z, .ac0000
	call	tt_MENU
.ac0000:
	
	    ; coins -> credits routine
	ld	a, #1
	ld	(coincount), a

	ld	a, (coinxcc)	; #coins per credits
	ld	hl, #(extrcoin)	; leftovers
	inc	(hl)
	sub	(hl)
	ret	nz		; not enough coins per credit, return

	call	credsnd
	ld	(hl), a		; store leftovers
	ld	a, (credxcc)	; #credits per coin
	ld	hl,#(credmem)
	add	a, (hl)		; add #credits
	daa
	jr	nc, ac2
	ld	a, #0x99
ac2:
	ld	(hl), a		; store #credits
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; inputcheck - check user inputs
inputcheck:
	ld	a, (p1_port)
	ld	hl, #(thisin0)
	ld	(hl), a			; thisin0 = p1_port

	ld	hl, #(thisin0)

	call	.inright		; bank++
	call	.inleft			; bank--
	call	.inup			; variant--
	call	.indown			; variant++

	ld	a, (thisin0)
	ld	(lastin0), a		; lastin0 = thisin0
	ret


;;;;;;;;;;;;;;;;;;;;
.inright:
	    ; edge detect
	push	hl
	bit	#p1_right, (hl)
	jr	nz, .irlud0
	ld	hl, #(lastin0)
	bit	#p1_right, (hl)
	jr	z, .irlud0

	    ; incloop
	call	randomize
	call	tt_MENU			; disable screen saver
	call	clksnd4

	ld	a, (gamesel)
	ld	b, #romcnt
.irinc:
	call	dincloop
	call	isValid
	jr	nz, .irinc

	ld	(gamesel), a
	ld	(ogamesel), a
	    ; finish up
	jr	.ilrCfgRst	; reset config

;;;;;;;;;;;;;;;;;;;;
.inleft:
	push	hl
	bit	#p1_left, (hl)
	jr	nz, .irlud0
	ld	hl, #(lastin0)
	bit	#p1_left, (hl)
	jr	z, .irlud0
	    ; decloop
	call	randomize
	call	tt_MENU			; disable screen saver
	call	clksnd1

	ld	a, (gamesel)
	ld	b, #romcnt
.ildec:
	call	ddecloop
	call	isValid
	jr	nz, .ildec

	ld	(gamesel), a	; restore it
	ld	(ogamesel), a
	    ; finish up
	    ; change the game displayed
.ilrCfgRst:			; reset the config
	xor	a
	ld	(watchdog), a
	ld	(gamecfg), a
	call	changegame

.irlud0:
	call	hs_compute		; set up the ram pointer
	call	hs_printout		; draw the high score to the screen
	pop	hl
	ret
	

;;;;;;;;;;;;;;;;;;;;
.inup:
	    ; edge detect
	push	hl
	bit	#p1_up, (hl)
	jr	nz, .irlud0
	ld	hl, #(lastin0)
	bit	#p1_up, (hl)
	jr	z, .irlud0

	    ; incloop
	call	randomize
	call	tt_MENU			; disable screen saver
	call	clksnd3
	ld	a, (gamecfg)
	ld	hl, #(maxcfgs)
	ld	b, (hl)
	call	decloop
	ld	(gamecfg), a
	    ; finish up
	jr	.irlud0


;;;;;;;;;;;;;;;;;;;; 
.indown:
	    ; edge detect
	push	hl
	bit	#p1_down, (hl)
	jr	nz, .irlud0
	ld	hl, #(lastin0)
	bit	#p1_down, (hl)
	jr	z, .irlud0
	    ; decloop
	call	randomize
	call	tt_MENU			; disable screen saver
	call	clksnd2
	ld	a, (gamecfg)
	ld	hl, #(maxcfgs)
	ld	b, (hl)
	call	incloop
	ld	(gamecfg), a
	    ; finish up
	jr	.irlud0


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


    ; changegame
changegame:
	    ; clear screen
	call	cls

	    ; load in the correct graphics bank
	ld	a, (gamesel)
	ld	d, #0
	ld	e, a
	ld	hl, #(romcfg)		; full game array
	rst	0x10		; load HL with base of current config
	push	hl
	push	hl
	ld	de, #rs_rom
	add	hl, de
	ld	a, (hl)		; a = rom bank number
	ld	(gamebank), a	; store aside the game bank

	pop	hl
	ld	de, #rs_text	; textmode
	add	hl, de
	ld	a, (hl)
	ld	(textmode), a	; store it

	pop	hl
	ld	de, #rs_gfx	; select graphics bank number
	add	hl, de
	    ; set the proper bank number in the hardware
	ld	a, (hl)		; grab the bank number from the struct
	ld	(gfxbank), a	; store aside graphics bank number
	out	(#gfxport),a	; tell the gfx bank switcher to change bank

.if	DEBUG
	ld	d, a
	ld	e, #(C_red)
	ld	bc, #0x3c5
	call	prtBCDbyte	; display gfx bank number - DEBUG
.endif

	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    ; incloop
	; a = value to increment/loop
	; b = max
dincloop:
	inc	a
incloop:
	inc	a
	cp	b
	jr	nz, .il0
	ld	a, #0x00                ; max, loop around
.il0:
	ret


    ; decloop
	; a = value to decrement
	; b = max 
ddecloop:
	cp	#0x00
	jr	nz, .ddl0

	ld	a, b   
.ddl0:
	dec	a
	dec	a
	ret

decloop:
	cp	#0x00
	jr	nz, .dl0

	ld	a, b
.dl0:
	dec	a
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    ; startgame
startgame:
	ld	a, (credmem)
	cp	#0x00
	ret	z			; no credits, return
	cp	#0x01
	ld	hl, #(start_port)
	jr	z, .sg_cb1		; 1 credit, check start button 1 only

	    ; make sure we can do 2player games
	ld	a, (gameflag)
	cp	#rsf_p1only
	jr	z, .sg_cb1		; skip p2 check if we should

	; check start 2 button
	bit	#start2, (hl)
	jr	z, .start2

.sg_cb1:
	; check start 1 button
	bit	#start1, (hl)
	jr	z, .start1
	ret

.start1:
	bit	#start1, (hl)
	jr	z, .start1		; debounce
	jp	.1startit		; call the gamehook valve

.start2:
	bit	#start2, (hl)
	jr	z, .start2		; debounce
	jp	.2startit		; call the gamehook valve


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


	; print out the high score to the screen
hs_printout:
	ld      a, (thismode)           ; see if we're in the menu now
        cp      #M_MENU
	ret	nz			; return if we're in screensaver, etc.

	ld	a, (gameflag)
	and	#rsf_NOHISAVE		; see if we should inhibit display
	cp	#rsf_NOHISAVE
	jp	z, hs_none		; draw blanks

	ld	hl, (hsRamAddr)		; load HL with the score for this game
	ld	a, (hl)			; score[0] is usage flag byte
	;cp	#0x00			; see if it's uninitialized...
	;jr	z, hs_none		; none - draw blanks
	
	inc	hl			; start at the score[1]
	ld	ix, #0x3ed 		; screen position to start at
	ld	b, #3			; print 3 bytes (6 BCD digits)
.hspr000:
	push	bc			; store our loop variable (b)
	ld	d, (hl)			; d = score[n]
	inc	hl			; 	    n++
	ld	e, #(C_white)		; use color 'white'
	push	ix			;    = ix
	pop	bc			; bc =    (screen position to use)
	inc	ix			; 
	inc	ix
	call	prtBCDbyte

	pop	bc			; restore our loop variable (b)
	djnz	.hspr000		; repeat if we need to

    ; zot the leading zeros 
	ld	de, #0x03f2		; start offset
	ld	b, #5			; 6 spaces to check
.hspr_zot:
	ld	hl, #vidram		; check the video ram for '0'
	add	hl, de			; add offset
	ld	a, (hl)			; a = screen ram
	cp	#0x30			; 30 = '0'
	jr	nz, hspr_header		; wasn't a '0', draw the header text
	    ; was a zero, zot it
	ld	hl, #colram		; go to the color ram for this byte
	add	hl, de			; add offset
	xor	a			; use black
	ld	(hl), a			; set it

	    ; loop around
	dec	de			; go to the next byte to the right
	djnz	.hspr_zot		; repeat if we need to


	    ; header text
hspr_header:
	ld	a, (textmode)		; print the right string
	cp	#SPACE20
	ld	hl, #(text_hs_20)	; ' '
	jr	z, .hspr_h000
	cp	#SPACE5b
	ld	hl, #(text_hs_5b)	; '['
	jr	z, .hspr_h000
	ld	hl, #(text_hs_40)	; '@'

.hspr_h000:
	ld	a, #(C_white)
	ld	bc, #0x0900
	call	putstrA
	ret

    ; text for the screen, for all three graphics bank modes
text_hs_40:
	.byte	10
	.ascii	"HIGH@SCORE"

text_hs_5b:
	.byte	10
	.ascii	"HIGH[SCORE"

text_hs_20:
	.byte	10
	.ascii	"HIGH SCORE"

hs_none:	; clear the screen of the high score area
	ld	hl, #(colram + 0x03cb)
	ld	b, #0x0a
	ld	a, #0
	call	memset256
	ld	hl, #(colram + 0x03eb)
	ld	b, #0x0a
	ld	a, #0
	call	memset256
	ret


	; compute the high score address
hs_compute:
	    ; factor in gamesel
	ld	a, (gamesel)		; load in game selector
	rla				; it's already x2, only need one rla
	and	#0xfc			; make sure bottom two bits are clear
	ld	c, a			; c = lower byte

	ld	a, (gamesel)
	rra
	rra
	rra
	rra
	rra
	rra
	rra				; ugh.
	and	#0x01
	ld	b, a

	    ; factor in the gamecfg
	ld	a, (gamecfg)
	rla				; shift it up
	and	#0x0e			; mask it off
	add	a, b			; add it to b
	ld	b, a		

	    ; bc now contains the offset
	ld	hl, #hsRamTop		; hl contains start addr
	add	hl, bc			; hl contains absolute addr
	push 	hl
	pop	bc			; bc contains absolute addr

	ld	hl, #hsRamAddr		; hl contains pointer start
	ld	(hl), c			; store it
	inc	hl
	ld	(hl), b
	
	ret
