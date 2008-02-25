;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Pac Menu code - rom config menu configuration
;
;  2004-01  Scott "Jerry" Lawrence
;
;  $Id: romcfgdefs.asm,v 1.10 2005/01/03 23:47:03 jerry Exp $
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module romcfgdefs

	    ; these are IDs for gamehook.asm
	;	(they need to be in the same order as in romcfg.asm)

    __mspac = 0
	Rmspac		= __mspac + 0		; original
	Rrmspac		= __mspac + 1		; reverse
	Rmspacatk	= __mspac + 2		; attack
	Rmspacplus	= __mspac + 3		; plus (attack reverse)
	Rmspacrmx	= __mspac + 4		; remix
	RmodeJ		= __mspac + 5		; mode j

    __mspacRIP = RmodeJ + 1
	RRmspac		= __mspacRIP + 0
	RRrmspac	= __mspacRIP + 1
	RRmspacatk	= __mspacRIP + 2
	RRmspacplus	= __mspacRIP + 3
	RRmspacrmx	= __mspacRIP + 4
	RRmodeJ		= __mspacRIP + 5

    __pac = RRmodeJ + 1
	Rpac		= __pac + 0
	Rpuck		= __pac + 1
	Rhangly		= __pac + 2
	Rvert		= __pac + 3
	Rxens		= __pac + 4
	Rshuffle	= __pac + 5
	Ryorg		= __pac + 6
	Rsil		= __pac + 7
	Rtbone		= __pac + 8
	Rcrazy		= __pac + 9
	Rhard		= __pac + 10
	Rordered	= __pac + 11
	Rrandom		= __pac + 12

    __pplus = Rrandom + 1
	RPpac		= __pplus + 0
	RPpuck		= __pplus + 1
	RPhangly	= __pplus + 2
;	RPvert		= __pplus + xxxx
	RPxens		= __pplus + 3
	RPshuffle	= __pplus + 4
	RPyorg		= __pplus + 5
	RPsil		= __pplus + 6
	RPtbone		= __pplus + 7
	RPcrazy		= __pplus + 8
	RPhard		= __pplus + 9
	RPordered	= __pplus + 10
	RPrandom	= __pplus + 11

    __xxx = RPrandom + 1
	Rpiranha	= __xxx + 0
	Rwavybug	= __xxx + 1
	Rquad		= __xxx + 2
	Raarmada	= __xxx + 3
	Rmrtnt		= __xxx + 4
	Reggor		= __xxx + 5

	Rcrush		= __xxx + 10
	Rlizwiz		= __xxx + 11
	Rbeastf		= __xxx + 12
	Rlsr 		= __xxx + 13
	Rpengman	= __xxx + 14

