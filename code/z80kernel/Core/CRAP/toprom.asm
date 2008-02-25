;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Pac Menu code - toprom
;
;  2004-01  Scott "Jerry" Lawrence
;
;  $Id: toprom.asm,v 1.3 2004/11/14 06:56:45 jerry Exp $
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module toprom
;
;    This module contains the two routines needed in each and every
;    rom bank.  The two routines allow for the menu to switch into
;    the rom bank properly (TR_GAME) as well as the routine to switch
;    back to the menu code (TR_MENU)
;
;    Both routines disable interrupts to prevent the game from going
;    out to lunch during an interrupt.  I don't think this can
;    actually happen, now that I think about it, but it can't hurt.
;    ;)
;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.org 0xbfd0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; TR_CREDEC
;   decrement the credits in credit memory, based on number of players selected
;   every game needs this, seems like it should be provided here
TR_CREDEC:
	push	af			; 1
	push	bc			; 1
	ld	a, (started)		; 3  number of players
	ld	b, a			; 1  b = nplayers
	ld	a, (credmem)		; 3
	cp	#0xff			; 2  check for freeplay flag
	jr	z, .TRRET		; 2  if freeplay, skip
.TRCD:
	xor	#0x00			; 2  zero flags
	dec	a			; 1  credits--
	daa				; 1  BCD adjust
	djnz	.TRCD			; 2
	ld	(credmem), a		; 3 
.TRRET:
	pop	bc			; 1
	pop	af			; 1
	ret				; 1


; TR_GAME
;   Start up a particular game.
;   The game code itself needs to be hacked to look at the PP30
;   ram space for control variables.
;
;	in	b	rom bank number
;	in	c	gfx bank number
;	in	hl	entry point in new rom
;	out	-
;	mod	*	(the game itself should init soon after being called)
TR_GAME:
	di				; 1  Interrupts BAD
	ld	a, c			; 1
	out	(#gfxport), a		; 2	; graphics bank
	ld	a, b			; 1
	out	(#romport), a		; 2	; program bank
	    ; at this point, we're actully running from a different rom bank
	jp	(hl)			; 1


; TR_MENU
;   Skip back to the original menu code.
;   
;	in	-
;	out	-
;	mod	*	(the pp30 menu code will init after being called)
TR_MENU:
	di				; 1  Interrupts BAD
	xor	a			; 1
	out	(#gfxport), a		; 2
	out	(#romport), a		; 2
	    ; at this point, we're in the menu bank
	rst	0x08			; 1
TR_END:
.bound	0xc000
