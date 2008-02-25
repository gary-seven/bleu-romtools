;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Pac Menu code - rom menu configuration
;
;  2004-01  Scott "Jerry" Lawrence
;
;  $Id: romcfg.asm,v 1.75 2005/04/24 01:12:12 jerry Exp $
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module romcfg

;
;	What follows os the table I used to keep track of which
;	game is where in the graphics, program, and color roms.
;	The four columns are as follows:
;
;		Program rom bank
;		Graphics rom bank
;		Variant number - when multiple games in a rom
;		 - config changes are stored elsewhere (fast/hard/etc.)
;		Color prom bank
;

    ; this table is horribly out of date.
; P G V	     SHORTNAME	GAME		COPYRIGHT
; 0 0 0      menu	-menu code-	2004 Retro-Arcade
; 0 0 1      quadblok	QuadBlok	2004 UmlautLlama
; 0 0 2      lsr	LightswitchRave	2004 UmlautLlama
;
; 1 1 0	     pacman	Pac-Man		1980 Midway/Namco
; 1 1 0	     hangly	Hangly Man	1981 Nittoh		(map only)
; 1 1 2	     puckman	Puckman		1980 Namco		(map text)
;
; 2 2 1	     pacplus	Pac-Man Plus	1982 Midway		(code, text)
;  
; 3 0 0	     mspacmab	Ms. Pac-Man	1981 Midway/GCC
; 3 0 8	     mspacatk	Ms. Pac Attack	1981 Two-Bit Score	(Sil hack)
; 3 0 -	   x mspacpls	Ms. Pacman Plus	1981 Two-Bit Score
; 3 5 -	     mspacmab	Ms. Pac-RIP	1981 Midway/GCC
;  
; 4 3 0	   x piranha	Piranha		1981 GL
; 5 4 0	     ladybug	LadyBug		2000 Widel
; 6 6 0      alienarm	Alien Armada	2003 Widel
; 7 7 0	     superglob	Super Glob	1983 Epos  (Beastie Feastie) 
; 8 8 0      lizwiz 	Lizard Wizard
; 9 9 0	     mrtnt	Mr. TNT		1983 Telko
; A A 0	   x eggor	Eggor		1982 Digitrex Techstar
; B B 0	     pengo	Pengo		1982 Sega/Widel
; C C 0	     crush	Crush Roller	1981 Kural Samno Electric
; D D 0	   x jumpshot	Jump Shot	1989 Bally Midway
; E E 0	   x vanvan	Van-Van Car	1983 Sanritsu
; F F 0	   x alibaba	Ali Baba	1982 Sega
;  
;  
;  - maybe -
; 3 X c	     mspengo	Ms. Pengo	2004 UmlautLlama
; - X -	   x brakman	Brak Man	1999 UmlautLlama
; - X -	   x pengoman	Pengo Man	1999 UmlautLlama


; in other words: (this is the definitive list, the above is annoying)
;
;   == Program Roms ==
;	P 0	Menu code, QuadBlok, LightswitchRave
; 	P 1 	Pac-man, map variants
;		Code:  Pac-Man - 25 map variant
;		Maps:  Pac, Puckman, Hangly
;	P 2 	Pac-Man Plus
;	P 3	Ms. Pac-Man, map variants
;		Maps: Ms. Pac-Man, Attack, Ms. Pengo
;	P 4	Piranha
;	P 5	Wavy Bug
;	P 6	Alien Armada
;	P 7	Beastie Feastie
;	P 8	Lizard Wizard
;	P 9	Mr. TNT
;	P a	Eggor (Eyes)
;	P b	Pengo
;
;	P c	Crush Roller
;	P d	Jump Shot
;	P e	Van-Van Car
;	P f	Ali Baba
;
;   == Graphics Roms ==
;	G 0	Ms. Pac-Man, Quadblock, Menu, LSR
;	G 1	Pac-Man
;	G 2	Pac-Man Plus
;	G 3	Piranha
;	G 4	Wavy Bug
;	G 5	Ms. Pac-Man RIP
;	G 6	Alien Armada
;	G 7	Beastie Feastie
;	G 8	Lizard Wizard
;
;	G 9	Mr. TNT
;	G a	Eggor (Eyes)
;	G b	Pengo
;
;	G c	Crush Roller
;	G d	Jump Shot
;	G e	Van-Van
;	G f	Ali Baba
;
;  After dark?  3D? Pengo?
;	
;   == Color Proms ==
;	C 0	Pac-Man, Ms. Pac-Man
;	C 1	Pac-Man Plus
;	C 2	Lizard Wizard
;	C 3	Beastie Feastie (?)
;	( none used; hacks only )

RCP_MENU	= 0x00
RCP_QUADBLOK	= 0x00
RCP_LSR		= 0x00
RCP_PACMAN	= 0x01
RCP_PPLUS	= 0x02
RCP_MSPAC	= 0x03
RCP_PIRANHA	= 0x04
RCP_WAVYBUG	= 0x05
RCP_AARMADA	= 0x06
RCP_BEASTF	= 0x07	; unused
RCP_LIZWIZ	= 0x08	; unused
RCP_MRTNT	= 0x09
RCP_EGGOR	= 0x0A
RCP_PENGO	= 0x0B
RCP_CRUSH	= 0x0C	; unused
RCP_JUMPSHOT	= 0x0D	; unused
RCP_VANVAN	= 0x0E	; unused
RCP_ALIBABA	= 0x0F	; unused

RCG_MENU	= 0x00
RCG_QUADBLOK	= 0x00
RCG_LSR		= 0x00
RCG_PACMAN	= 0x01
RCG_PPLUS	= 0x02
RCG_MSPAC	= 0x00
RCG_PIRANHA	= 0x03
RCG_WAVYBUG	= 0x04
RCG_MSPRIP	= 0x0f 	; 0x05
RCG_AARMADA	= 0x06
RCG_BEASTF	= 0x07	; pac-man 3d
RCG_LIZWIZ	= 0x08	; ms mini
RCG_MRTNT	= 0x09
RCG_EGGOR	= 0x0A
RCG_PENGO	= 0x0B
RCG_CRUSH	= 0x0C	
RCG_JUMPSHOT	= 0x0D	; ms yak
RCG_VANVAN	= 0x0E  ; ms afterdark
RCG_ALIBABA	= 0x05  ; 0x0f   ; ms renaissance




;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; listingof romsets

	; romcfg -- the main game configuration listing;
	; 	these are the complete game listings (with cheat, etc.)
romcfg::
	    ; mspac
	.word	mspacmab	; mspac normal
	.word	mspacrev	; mspac reverse
	.word	mspacatk	; mspac attacks
	.word	mspacplus	; miss pac plus
	.word	mspacrmx	; mspac remix
	.word	modeJ		; mspac Mode J

	    ; mspac RIP
	.word	mRspacmab	; RIP mspac normal
	.word	mRspacrev	; mspac reverse
	.word	mRspacatk	; RIP mspac attacks
	.word	mRspacplus	; RIP miss pac plus
	.word	mRspacrmx	; mspac remix
	.word	mRodeJ		; RIP mspac Mode J

	    ; pac
	.word	pacman		; pac-man (Midway)
	.word	pacman1		; puck-man 
	.word	pacman2		; Hangly-man
	.word	pacman3		; vertical tunnel
	.word	pacman4		; xen's revenge
	.word	pacman5		; Shuffle
	.word	pacman6		; yorgle maps
	.word	pacman7		; silmaps
	.word	pacman8		; t-bone maps
	.word	pacman9		; crazy/ultra maps
	.word	pacmanA		; hard maps
	.word	pacmanB		; all in order
	.word	pacmanC		; all random order

	    ; pac plus
	.word	pacpls		; pac-plus (Midway)
	.word	pacpls1		; puck-man 
	.word	pacpls2		; Hangly-pls
;	.word	pacpls3		; vertical tunnel
	.word	pacpls4		; xen's revenge
	.word	pacpls5		; Shuffle
	.word	pacpls6		; yorgle maps
	.word	pacpls7		; silmaps
	.word	pacpls8		; t-bone maps
	.word	pacpls9		; crazy/ultra maps
	.word	pacplsA		; hard maps
	.word	pacplsB		; all in order
	.word	pacplsC		; all random order

	    ; pac hacks
	.word	piranha		; piranha
	.word	wavybug		; Widel's Ladybug (Kazzy Map)
	    ; misc
	.word	quadblok	; quadblock
	    ; other games
	.word	aarmada		; alien armada
	.word	mrtnt		; Mr TNT
	.word	eggor		; Eggor
	.word	pengo		; Pengo

	
		; put these after the last valid one.
	romcnt	= .-romcfg
	Dmaxcfgs = romcnt/2	; - this is how the menu knows how many to use

	.word	lizwiz		; lizard wizard
	.word	beastf		; beastie feastie (best. game. evar.)
	.word	lsr		; lightswitch Rave
	;.word	pengoman	; pengoman


	; this is the gfx bank to use for title screens
	titlebank	= Rmspac	; set to mspacmab
	
	rs_name		= 0x00		; W offset to name string
	rs_cprt		= 0x02		; W offset to copyright string
	rs_cprt2	= 0x04		; W offset to copyright string line 2
	rs_vars		= 0x06		; W offset to vars list
	rs_olvars	= 0x08		; W offset to On-Location vars list
	rs_chars	= 0x0a		; W offset to RLE SCR image char 
	rs_color	= 0x0c		; W offset to RLE SCR image color
	rs_rom		= 0x0e		; B rom bank number
	rs_gfx		= 0x0f		; B graphics bank number
	rs_a1		= 0x10		; W animation array 1
	rs_a2		= 0x12		; W animation array 2
	rs_a3		= 0x14		; W animation array 3
	rs_a4		= 0x16		; W animation array 4
	rs_flags	= 0x18		; B flags:
	 rsf_p1only	=  0x01		;    1 player only
	 rsf_DISABLE	=  0x80		;    disable for Home use
	 rsf_OLDISABLE	=  0x40		;    disable for On Location use
	 rsf_NOHISAVE	=  0x02		;    don't display high score in menu?
	rs_text		= 0x19		; B text flags
	 SPACE40	=  0x00		;   pac-games		'@'
	 SPACE20	=  0x01		;   alien armada 	' '
	 SPACE5b	=  0x02		;   beastie feastie	'['
	


; configuration for romset regions is:
;	word	pointer to name ascii string
;	word	pointer to cprt string  (year mfg)
;	word	pointer to cprt string2 (hack/bootleg/etc)
;	word	pointer to variations list
;	word	pointer to screen img chars
;	word	pointer to screen img colors
;	char	bank number for graphics
; 	char	bank number for romset
;	word	pointer to animation array 1 (structure below)
;	word	pointer to animation array 2 (structure below)
;	word	pointer to animation array 3 (structure below)
;	word	pointer to animation array 4 (structure below)
;	char	game flags
;	char	text flags



;	word	jump point for attract sequence
;	word	jump point for game play
;	word	pointer for high score list


; animation structure
;	byte	color
;	byte	frame 0
;	byte	frame 1
;	byte	frame 2
;	byte	frame 3

.an_NULL:	.byte 	0x00, 0, 0, 0, 0	; blank


;  Still to do: 
;   Map out pac-man, Pac-Man Plus, Puckman, Hanglyman, to integrate 
;	them all into one romset.
;	if not:
;	   Puckman, HanglyMan, Pac-Man in one
;	   Pac-Man Plus in another
;   Hack all games for coin entry
;   Hack all games for buy-in
;   Hack all games for attract mode wandering

; pac variants
pacvars:
	.word	0x07
	.word	.vnorm
	.word	.vfast
	.word	.vhard
	.word	.vfasthard
	.word	.vpause
	.word	.vfastpause
	.word	.vcheat

Opacvars:
	.word	0x04
	.word	.vnorm
	.word	.vfast
	.word	.vhard
	.word	.vfasthard


; mspac variants
mspacvars:
	.word	0x05
	.word	.vnorm		; 0
	.word	.vfast		; 1
	.word	.vpause		; 2
	.word	.vfastpause	; 3
	.word	.vcheat		; 4

Omspacvars:
	.word	0x02
	.word	.vnorm
	.word	.vfast

modejvars:
Omodejvars:
	.word	0x02
	.word	.vnorm
	.word	.vhard


; piranha variants
pirvars:
	.word	0x02
	.word	.vnorm
	.word	.vfast

Opirvars:
	.word	0x02
	.word	.vnorm
	.word	.vfast


.vnorm:
	.byte	6
	.ascii	"NORMAL"

.vhard:
	.byte	4
	.ascii	"HARD"

.vfast:
	.byte	4
	.ascii	"FAST"

.vfasthard:
	.byte	9
	.ascii	"FAST;HARD"

.vcheat:
	.byte	5
	.ascii	"CHEAT"

.vpause:
	.byte	5
	.ascii	"PAUSE"
	
.vfastpause:
	.byte	10
	.ascii	"FAST;PAUSE"
	



pacman:
	.word	.r0name
	.word	.r0cprt1
	.word	.r0cprt2
	.word	pacvars
	.word	Opacvars
	.word	v_pacmod
	.word	c_pacmod
	.byte	RCP_PACMAN
	.byte	RCG_PACMAN
	.word	.an_pac1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_mab4
	.byte	0x00		; flags
	.byte 	SPACE40

.an_pac1:	.byte	0x09, 44*4, 46*4, 48*4, 46*4	; pac

.r0name::
	.byte   7
	.ascii	"PAC;MAN"
.r0cprt1:
	.byte	12
	.ascii	"\\1980@MIDWAY"
.r0cprt2:
	.byte	8
	.ascii "ORIGINAL"

pacman1:	; puckman
	.word	.p1name
	.word	.p1cprt1
	.word	.r0cprt2
	.word	pacvars
	.word	Opacvars
	.word	puck_vid
	.word	c_pacmod
	.byte	RCP_PACMAN
	.byte	RCG_PACMAN
	.word	.an_pac1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_mab4
	.byte	0x00 ;rsf_DISABLE ; flags
	.byte 	SPACE40

.p1name:
	.byte   8
	.ascii	"PUCK;MAN"
.p1cprt1:
	.byte	11
	.ascii	"\\1980@NAMCO"


pacman2:
	.word	.p2name
	.word	.p2cprt1
	.word	.r0cprt2
	.word	pacvars
	.word	Opacvars
	.word	hang_vid
	.word	c_pacmod
	.byte	RCP_PACMAN
	.byte	RCG_PACMAN
	.word	.an_pac1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_mab4
	.byte	0x00		; flags
	.byte 	SPACE40

.p2name:
	.byte   10
	.ascii	"HANGLY;MAN"
.p2cprt1:
	.byte	12
	.ascii	"\\1980@NITTOH"

pacman3:
	.word	.p3name
	.word	.p3cprt1
	.word	.r0cprt2
	.word	pacvars
	.word	Opacvars
	.word	silv_vid
	.word	c_pacmod
	.byte	RCP_PACMAN
	.byte	RCG_PACMAN
	.word	.an_pac1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_mab4
	.byte	0x00		; flags
	.byte 	SPACE40

.p3name:
	.byte   15
	.ascii	"VERTICAL@TUNNEL"
.p3cprt1:
	.byte	7
	.ascii	"THE@SIL"

pacman4:
	.word	.p4name
	.word	.p4cprt1
	.word	.r0cprt2
	.word	pacvars
	.word	Opacvars
	.word	xens_vid
	.word	c_pacmod
	.byte	RCP_PACMAN
	.byte	RCG_PACMAN
	.word	.an_pac1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_mab4
	.byte	0x00		; flags
	.byte 	SPACE40

.p4name:
	.byte   12
	.ascii	"XENS@REVENGE"
.p4cprt1:
	.byte	11
	.ascii	"JEFF@MORRIS"

pacman5:
	.word	.p5name
	.word	.p5cprt1
	.word	.r0cprt2
	.word	pacvars
	.word	Opacvars
	.word	shuf_vid
	.word	c_pacmod
	.byte	RCP_PACMAN
	.byte	RCG_PACMAN
	.word	.an_pac1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_mab4
	.byte	0x00		; flags
	.byte 	SPACE40

.p5name:
	.byte   11
	.ascii	"PAC@SHUFFLE"
.p5cprt1:
	.byte	10
	.ascii	"SNAKE@EYES"


pacman6:
	.word	.p6name
	.word	.p6cprt1
	.word	.r0cprt2
	.word	pacvars
	.word	Opacvars
	.word	yorg_vid
	.word	c_pacmod
	.byte	RCP_PACMAN
	.byte	RCG_PACMAN
	.word	.an_pac1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_mab4
	.byte	rsf_OLDISABLE | rsf_DISABLE		; flags
	.byte 	SPACE40

.p6name:
	.byte   11
	.ascii	"YORGLE@MAPS"
.p6cprt1:
	.byte	11
	.ascii	"UMLAUTLLAMA"


pacman7:
	.word	.p7name
	.word	.p7cprt1
	.word	.r0cprt2
	.word	pacvars
	.word	Opacvars
	.word	sil_vid
	.word	c_pacmod
	.byte	RCP_PACMAN
	.byte	RCG_PACMAN
	.word	.an_pac1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_mab4
	.byte	0x00		; flags
	.byte 	SPACE40

.p7name:
	.byte   8
	.ascii	"SIL@MAPS"
.p7cprt1:
	.byte	7
	.ascii	"THE@SIL"


pacman8:
	.word	.p8name
	.word	.p8cprt1
	.word	.r0cprt2
	.word	pacvars
	.word	Opacvars
	.word	tbon_vid
	.word	c_pacmod
	.byte	RCP_PACMAN
	.byte	RCG_PACMAN
	.word	.an_pac1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_mab4
	.byte	0x00		; flags
	.byte 	SPACE40

.p8name:
        .byte   11
        .ascii  "T;BONE@MAPS"
.p8cprt1:
        .byte   6
        .ascii  "T;BONE"


pacman9:
	.word	.p9name
	.word	.p9cprt1
	.word	.r0cprt2
	.word	pacvars
	.word	Opacvars
	.word	crzy_vid
	.word	c_pacmod
	.byte	RCP_PACMAN
	.byte	RCG_PACMAN
	.word	.an_pac1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_mab4
	.byte	0x00		; flags
	.byte 	SPACE40

.p9name:
        .byte   11
        .ascii  "CRAZY;ULTRA"
.p9cprt1:
        .byte   20
        .ascii  "BLUE@JUSTICE;PAC@FAN"


pacmanA:
	.word	.pAname
	.word	.pAcprt1
	.word	.r0cprt2
	.word	pacvars
	.word	Opacvars
	.word	xens_vid
	.word	c_pacmod
	.byte	RCP_PACMAN
	.byte	RCG_PACMAN
	.word	.an_pac1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_mab4
	.byte	0x00		; flags
	.byte 	SPACE40

.pAname:
        .byte   9
        .ascii  "HARD@MAPS"
.pAcprt1:
        .byte   7
        .ascii  "VARIOUS"



pacmanB:
	.word	.pBname
	.word	.pBcprt1
	.word	.r0cprt2
	.word	pacvars
	.word	Opacvars
	.word	v_pacmod
	.word	c_pacmod
	.byte	RCP_PACMAN
	.byte	RCG_PACMAN
	.word	.an_pac1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_mab4
	.byte	0x00		; flags
	.byte 	SPACE40

.pBname:
        .byte	8 
        .ascii  "ALL@MAPS"
.pBcprt1:
        .byte   8
        .ascii  "IN@ORDER"


pacmanC:
	.word	.pCname
	.word	.pCcprt1
	.word	.r0cprt2
	.word	pacvars
	.word	Opacvars
	.word	tbon_vid
	.word	c_pacmod
	.byte	RCP_PACMAN
	.byte	RCG_PACMAN
	.word	.an_pac1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_mab4
	.byte	0x00		; flags
	.byte 	SPACE40


.pCname:
        .byte   8
        .ascii  "ALL@MAPS"
.pCcprt1:
        .byte   12
        .ascii  "RANDOM@ORDER"



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	    ; pac plus

.an_map4:	.byte	0x14, 1*4, 1*4, 7*4, 7*4	; powerups

pacpls:
	.word	.r0Pname
	.word	.r0Pcprt1
	.word	.r0cprt2
	.word	pacvars
	.word	Opacvars
	.word	v_pacmod
	.word	c_pacplus
	.byte	RCP_PPLUS		; Pac Plus Program
	.byte	RCG_PPLUS		; Pac Plus graphics
	.word	.an_pac1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_map4
	.byte	0x00 ;rsf_DISABLE		; flags
	.byte 	SPACE40


.r0Pname::
	.byte   12
	.ascii	"PAC;MAN@PLUS"
.r0Pcprt1:
	.byte	12
	.ascii	"\\1982@MIDWAY"

pacpls1:	; puckman
	.word	.p1Pname
	.word	.p1cprt1
	.word	.r0cprt2
	.word	pacvars
	.word	Opacvars
	.word	puck_vid
	.word	c_pacplus
	.byte	RCP_PPLUS		; Pac Plus Program
	.byte	RCG_PPLUS		; Pac Plus graphics
	.word	.an_pac1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_map4
	.byte	0x00 ;rsf_DISABLE		; flags
	.byte 	SPACE40

.p1Pname:
	.byte   13
	.ascii	"PUCK;MAN@PLUS"


pacpls2:
	.word	.p2Pname
	.word	.p2cprt1
	.word	.r0cprt2
	.word	pacvars
	.word	Opacvars
	.word	hang_vid
	.word	c_pacplus
	.byte	RCP_PPLUS		; Pac Plus Program
	.byte	RCG_PPLUS		; Pac Plus graphics
	.word	.an_pac1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_map4
	.byte	0;rsf_DISABLE		; flags
	.byte 	SPACE40

.p2Pname:
	.byte   15
	.ascii	"HANGLY;MAN@PLUS"

pacpls3:
	.word	.p3Pname
	.word	.p3cprt1
	.word	.r0cprt2
	.word	pacvars
	.word	Opacvars
	.word	silv_vid
	.word	c_pacplus
	.byte	RCP_PPLUS		; Pac Plus Program
	.byte	RCG_PPLUS		; Pac Plus graphics
	.word	.an_pac1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_map4
	.byte	0;rsf_DISABLE		; flags
	.byte 	SPACE40

.p3Pname:
	.byte   20
	.ascii	"VERTICAL@TUNNEL@PLUS"

pacpls4:
	.word	.p4Pname
	.word	.p4cprt1
	.word	.r0cprt2
	.word	pacvars
	.word	Opacvars
	.word	xens_vid
	.word	c_pacplus
	.byte	RCP_PPLUS		; Pac Plus Program
	.byte	RCG_PPLUS		; Pac Plus graphics
	.word	.an_pac1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_map4
	.byte	0;rsf_DISABLE		; flags
	.byte 	SPACE40

.p4Pname:
	.byte   17
	.ascii	"XENS@REVENGE@PLUS"

pacpls5:
	.word	.p5Pname
	.word	.p5cprt1
	.word	.r0cprt2
	.word	pacvars
	.word	Opacvars
	.word	shuf_vid
	.word	c_pacplus
	.byte	RCP_PPLUS		; Pac Plus Program
	.byte	RCG_PPLUS		; Pac Plus graphics
	.word	.an_pac1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_map4
	.byte	0;rsf_DISABLE		; flags
	.byte 	SPACE40

.p5Pname:
	.byte   16
	.ascii	"PAC@SHUFFLE@PLUS"


pacpls6:
	.word	.p6Pname
	.word	.p6cprt1
	.word	.r0cprt2
	.word	pacvars
	.word	Opacvars
	.word	yorg_vid
	.word	c_pacplus
	.byte	RCP_PPLUS		; Pac Plus Program
	.byte	RCG_PPLUS		; Pac Plus graphics
	.word	.an_pac1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_map4
	.byte	rsf_OLDISABLE | rsf_DISABLE		; flags
	.byte 	SPACE40

.p6Pname:
	.byte   16
	.ascii	"YORGLE@MAPS@PLUS"


pacpls7:
	.word	.p7Pname
	.word	.p7cprt1
	.word	.r0cprt2
	.word	pacvars
	.word	Opacvars
	.word	sil_vid
	.word	c_pacplus
	.byte	RCP_PPLUS		; Pac Plus Program
	.byte	RCG_PPLUS		; Pac Plus graphics
	.word	.an_pac1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_map4
	.byte	0;rsf_DISABLE		; flags
	.byte 	SPACE40

.p7Pname:
	.byte   13
	.ascii	"SIL@MAPS@PLUS"

pacpls8:
	.word	.p8Pname
	.word	.p8cprt1
	.word	.r0cprt2
	.word	pacvars
	.word	Opacvars
	.word	tbon_vid
	.word	c_pacplus
	.byte	RCP_PPLUS		; Pac Plus Program
	.byte	RCG_PPLUS		; Pac Plus graphics
	.word	.an_pac1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_map4
	.byte	0;rsf_DISABLE		; flags
	.byte 	SPACE40

.p8Pname:
        .byte   16
        .ascii  "T;BONE@MAPS@PLUS"



pacpls9:
	.word	.p9Pname
	.word	.p9cprt1
	.word	.r0cprt2
	.word	pacvars
	.word	Opacvars
	.word	crzy_vid
	.word	c_pacplus
	.byte	RCP_PPLUS		; Pac Plus Program
	.byte	RCG_PPLUS		; Pac Plus graphics
	.word	.an_pac1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_map4
	.byte	0;rsf_DISABLE		; flags
	.byte 	SPACE40

.p9Pname:
        .byte   16
        .ascii  "CRAZY;ULTRA@PLUS"


pacplsA:
	.word	.pAPname
	.word	.pAcprt1
	.word	.r0cprt2
	.word	pacvars
	.word	Opacvars
	.word	xens_vid
	.word	c_pacplus
	.byte	RCP_PPLUS		; Pac Plus Program
	.byte	RCG_PPLUS		; Pac Plus graphics
	.word	.an_pac1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_map4
	.byte	0;rsf_DISABLE		; flags
	.byte 	SPACE40

.pAPname:
        .byte   14
        .ascii  "HARD@MAPS@PLUS"



pacplsB:
	.word	.pBPname
	.word	.pBcprt1
	.word	.r0cprt2
	.word	pacvars
	.word	Opacvars
	.word	v_pacmod
	.word	c_pacplus
	.byte	RCP_PPLUS		; Pac Plus Program
	.byte	RCG_PPLUS		; Pac Plus graphics
	.word	.an_pac1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_map4
	.byte	0;rsf_DISABLE		; flags
	.byte 	SPACE40

.pBPname:
        .byte	13
        .ascii  "ALL@MAPS@PLUS"



pacplsC:
	.word	.pCPname
	.word	.pCcprt1
	.word	.r0cprt2
	.word	pacvars
	.word	Opacvars
	.word	tbon_vid
	.word	c_pacplus
	.byte	RCP_PPLUS		; Pac Plus Program
	.byte	RCG_PPLUS		; Pac Plus graphics
	.word	.an_pac1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_map4
	.byte	0;rsf_DISABLE		; flags
	.byte 	SPACE40

.pCPname:
        .byte   13
        .ascii  "ALL@MAPS@PLUS"



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.an_mab1:	.byte	0x09, 45*4, 55*4, 47*4, 55*4	; mspac
.an_mab2:	.byte	0x01, 36*4, 37*4, 36*4, 37*4	; red ghost looking left
.an_mab3:	.byte	0x11, 28*4, 29*4, 28*4, 29*4	; blue afraid ghost
.an_mab4:	.byte	0x0f, 0*4, 0*4, 1*4, 1*4	; fruit


.r1name:
	.byte   10
	.ascii	"MS%PAC;MAN"
.r1cprt1:
	.byte	20
	.ascii	"\\1981@GENCOMP:MIDWAY"
.r1cprt2:
	.byte	8
	.ascii "ORIGINAL"

.rR1name:
	.byte   14
	.ascii	"MS%PAC;MAN@3;D"

.rr1name:
	.byte	14
	.ascii	"MS%PAC;MAN:REV"

.rRr1name:
	.byte	18
	.ascii	"MS%PAC;MAN@3;D:REV"

mspacmab:
	.word	.r1name
	.word	.r1cprt1
	.word	.r1cprt2
	.word	mspacvars
	.word	Omspacvars
	.word	v_mspacmab
	.word	c_mspacmab
	.byte	RCP_MSPAC
	.byte	RCG_MSPAC
	.word	.an_mab1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_mab4
	.byte	0x00		; flags
	.byte 	SPACE40

mRspacmab:
	.word	.rR1name
	.word	.r1cprt1
	.word	.r1cprt2
	.word	mspacvars
	.word	Omspacvars
	.word	v_mspacmab
	.word	c_mspacmab
	.byte	RCP_MSPAC
	.byte	RCG_MSPRIP
	.word	.an_mab1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_mab4
	.byte	0x00		; flags
	.byte 	SPACE40


mspacrev:
	.word	.rr1name
	.word	.r1cprt1
	.word	.r1cprt2
	.word	mspacvars
	.word	Omspacvars
	.word	v_mspacmab
	.word	c_mspacmab
	.byte	RCP_MSPAC
	.byte	RCG_MSPAC
	.word	.an_mab1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_mab4
	.byte	rsf_OLDISABLE | rsf_DISABLE		; flags
	.byte 	SPACE40

mRspacrev:
	.word	.rRr1name
	.word	.r1cprt1
	.word	.r1cprt2
	.word	mspacvars
	.word	Omspacvars
	.word	v_mspacmab
	.word	c_mspacmab
	.byte	RCP_MSPAC
	.byte	RCG_MSPRIP
	.word	.an_mab1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_mab4
	.byte	rsf_OLDISABLE | rsf_DISABLE		; flags
	.byte 	SPACE40


mspacatk:
	.word	.r2name
	.word	.r2cprt1
	.word	.r2cprt2
	.word	mspacvars
	.word	Omspacvars
	.word	v_mspacatk
	.word	c_mspacatk
	.byte	RCP_MSPAC
	.byte	RCG_MSPAC
	.word	.an_mab1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_mab4
	.byte	0x00		; flags
	.byte 	SPACE40

mRspacatk:
	.word	.rR2name
	.word	.r2cprt1
	.word	.r2cprt2
	.word	mspacvars
	.word	Omspacvars
	.word	v_mspacatk
	.word	c_mspacatk
	.byte	RCP_MSPAC
	.byte	RCG_MSPRIP
	.word	.an_mab1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_mab4
	.byte	0x00		; flags
	.byte 	SPACE40

.r2name:
	.byte   13
	.ascii	"MS%PAC@ATTACK"
.rR2name:
	.byte   17
	.ascii	"MS%PAC@ATTACK@3;D"
.r2cprt1:
	.byte	19
	.ascii	"\\1981@TWO@BIT@SCORE"
.r2cprt2:
	.byte	12
	.ascii "MODIFICATION"

mspacplus:
	.word	.mPname
	.word	.mPcprt1
	.word	.r2cprt2
	.word	mspacvars
	.word	Omspacvars
	.word	v_mspacatk
	.word	c_mspacatk
	.byte	RCP_MSPAC
	.byte	RCG_MSPAC
	.word	.an_mab1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_mab4
	.byte	rsf_OLDISABLE | rsf_DISABLE		; flags
	.byte 	SPACE40

mRspacplus:
	.word	.mPRname
	.word	.mPcprt1
	.word	.r2cprt2
	.word	mspacvars
	.word	Omspacvars
	.word	v_mspacatk
	.word	c_mspacatk
	.byte	RCP_MSPAC
	.byte	RCG_MSPRIP
	.word	.an_mab1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_mab4
	.byte	rsf_OLDISABLE | rsf_DISABLE		; flags
	.byte 	SPACE40


.mPname:
        .byte   13      
        .ascii  "MISS@PAC@PLUS"
.mPRname:
        .byte   17
        .ascii  "MISS@PAC@PLUS@3;D"
.mPcprt1:
        .byte   19
        .ascii  "\\1981@TWO@BIT@SCORE"

mspacrmx:
	.word	.mRname
	.word	.mRcprt1
	.word	.r2cprt2
	.word	mspacvars
	.word	Omspacvars
	.word	v_mspacatk
	.word	c_mspacatk
	.byte	RCP_MSPAC
	.byte	RCG_MSPAC
	.word	.an_mab1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_mab4
	.byte	0x00		; flags
	.byte 	SPACE40

mRspacrmx:
	.word	.mRRname
	.word	.mRcprt1
	.word	.r2cprt2
	.word	mspacvars
	.word	Omspacvars
	.word	v_mspacatk
	.word	c_mspacatk
	.byte	RCP_MSPAC
	.byte	RCG_MSPRIP
	.word	.an_mab1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_mab4
	.byte	0x00		; flags
	.byte 	SPACE40

.mRname:
        .byte   12
        .ascii  "MS%PAC@REMIX"
.mRRname:
        .byte   16
        .ascii  "MS%PAC@REMIX@3;D"
.mRcprt1:
        .byte   19
        .ascii  "NAMCO:TWO@BIT@SCORE"


modeJ:
	.word	.r2Jname
	.word	.r2Jc1
	.word	.r2Jc2
	.word	modejvars
	.word	Omodejvars
	.word	v_mspacmab
	.word	c_modej
	.byte	RCP_MSPAC
	.byte	RCG_MSPAC
	.word	.an_mab1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_mab4
	.byte	0x00		; flags
	.byte 	SPACE40

mRodeJ:
	.word	.r2RJname
	.word	.r2Jc1
	.word	.r2Jc2
	.word	modejvars
	.word	Omodejvars
	.word	v_mspacmab
	.word	c_modej
	.byte	RCP_MSPAC
	.byte	RCG_MSPRIP
	.word	.an_mab1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_mab4
	.byte	0x00		; flags
	.byte 	SPACE40

.r2Jname:
	.byte   12
	.ascii	"MS%PAC@HYPER"
.r2RJname:
	.byte   16
	.ascii	"MS%PAC@HYPER@3;D"
.r2Jc1:
	.byte	18
	.ascii	"\\2005@UMLAUTLLAMA@"
.r2Jc2:
	.byte	12
	.ascii "MODIFICATION"

.an_wav1:	.byte	0x09, 44, 46, 44, 46	; ladybug
.an_wav2:	.byte	0x15, 39, 38, 38, 39	; inchworm
.an_wav3:	.byte	0x0f, 15, 14, 15, 14 	; bug thing
.an_wav4:	.byte	0x17, 17, 17, 16, 16	; grasshopper

wavyvars:
	.word	0x04
	.word	.vnorm
	.word	.vfast
	.word	.vpause
	.word	.vfastpause

Owavyvars:
	.word	0x02
	.word	.vnorm
	.word	.vfast

wavybug:
	.word	.r6name
	.word	.r6cprt1
	.word	.r6cprt2
	.word	wavyvars
	.word	Owavyvars
	.word	wavy_vid
	.word	wavy_col
	.byte	RCP_WAVYBUG
	.byte	RCG_WAVYBUG
	.word	.an_wav1
	.word	.an_wav2
	.word	.an_wav3
	.word	.an_wav4
	.byte	0x00		; flags
	.byte 	SPACE40

.r6name:
	.byte   8
	.ascii	"WAVY@BUG"
.r6cprt1:
	.byte	17
	.ascii	"\\2000@WIDEL:KAZZY"
.r6cprt2:
	.byte	12
	.ascii "HACK"

.an_ppp3:	.byte	0x11, 28*4, 28*4, 28*4, 29*4	; slower blue 
.an_ppp4:	.byte	0x17, 0*4, 0*4, 5*4, 5*4	; keys

piranha:
	.word	.r7name
	.word	.r7cprt1
	.word	.r7cprt2
	.word	pirvars
	.word	Opirvars
	.word	pir_vid
	.word	pir_col
	.byte	RCP_PIRANHA
	.byte	RCG_PIRANHA
	.word	.an_pac1
	.word	.an_mab2
	.word	.an_ppp3
	.word	.an_ppp4
	.byte	0x00		; flags
	.byte 	SPACE40

.r7name:
	.byte   7
	.ascii	"PIRANHA"
.r7cprt1:
	.byte	8
	.ascii	"\\1981@GL"
.r7cprt2:
	.byte	12
	.ascii "LICENSED@MOD"

.if 0

pengoman:
	.word	.r3name
	.word	.r3cprt1
	.word	.r3cprt2
	.word	pacvars
	.word	Opacvars
	.word	v_pengoman
	.word	c_pengoman
	.byte	RCP_MSPAC
	.byte	RCG_PENGOMAN
	.word	.an_pac1
	.word	.an_mab2
	.word	.an_mab3
	.word	.an_mab4
	.byte	0x00		; flags
	.byte 	SPACE40

.r3name:
	.byte   9
	.ascii	"PENGO;MAN"
.r3cprt1:
	.byte	18
	.ascii	"\\1997@UMLAUTLLAMA@"
.r3cprt2:
	.byte	12
	.ascii "MODIFICATION"
.endif

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; quadblok

quadblok:
	.word	.r4name
	.word	.r4cprt1
	.word	0x0000
	.word	quadvars
	.word	Oquadvars
	.word	quad_vid
	.word	quad_col
	.byte	RCP_QUADBLOK
	.byte	RCG_QUADBLOK
	.word	.an_qb0		; no anim yet
	.word	.an_qb0		; no anim yet
	.word	.an_qb0		; no anim yet
	.word	.an_qb0		; no anim yet
	.byte	0x01		; 1p only
	.byte 	SPACE40

.an_qb0:	.byte	0x12, 40*4, 42*4, 41*4, 43*4	; scores?

.r4name:
	.byte   9
	.ascii	"QUAD;BLOK"
.r4cprt1:
	.byte	17
	.ascii	"\\2005@UMLAUTLLAMA;PAJITNOV"

; VARIANT NAMES are in game_quadblok.asm


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; lightswitch rave

lsr:
	.word	.r5name
	.word	.r5cprt1
	.word	0x0000
	.word	lsrvars
	.word	Olsrvars
	.word	v_pengoman
	.word	c_pengoman
	.byte	RCP_LSR
	.byte	RCG_LSR
	.word	.an_qb0		; no anim yet
	.word	.an_qb0		; no anim yet
	.word	.an_qb0		; no anim yet
	.word	.an_qb0		; no anim yet
	.byte	0x01		; 1p only
	.byte 	SPACE40

.r5name:
	.byte   16
	.ascii	"LIGHTSWITCH@RAVE"
.r5cprt1:
	.byte	18
	.ascii	"\\2005@UMLAUTLLAMA@"

lsrvars:
	.word	0x02
	.word	.lsr_v0
	.word	.lsr_v1

Olsrvars:
	.word	0x02
	.word	.lsr_v0
	.word	.lsr_v1

.lsr_v0: .byte	6
	.ascii	"NORMAL"
.lsr_v1: .byte	15
	.ascii	"TOURNAMENT@MODE"


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; alien armada

.an_aa0:	.byte	0x0f, 0x10*4, 0x11*4, 0x12*4, 0x13*4	; ship
.an_aa1:	.byte	0x0f, 0x18*4, 0x18*4, 0x18*4, 0x18*4	; UFO
.an_aa2:	.byte	0x12, 0x0e*4, 0x0f*4, 0x0e*4, 0x0f*4	; exploding
.an_aa3:	.byte	0x0f, 0x32*4, 0x33*4, 0x32*4, 0x31*4	; supership (unused)

aarmada:
	.word	.AAname
	.word	.AAcprt1
	.word	0x0000
	.word	aavars
	.word	Oaavars
	.word	aa_vid
	.word	aa_col
	.byte	RCP_AARMADA
	.byte	RCG_AARMADA
	.word	.an_aa0		; no anim yet
	.word	.an_aa1		; no anim yet
	.word	.an_aa2		; no anim yet
	.word	.an_aa3		; no anim yet
	.byte	0x01		; 1p only
	.byte 	SPACE20

.AAname:
	.byte   12
	.ascii	"ALIEN ARMADA"
.AAcprt1:
	.byte	19
	.ascii	"@2002 FLAMMEL&WIDEL"

aavars:
	.word	0x01
	.word	.aav_v0
	.word	.aav_v1
Oaavars:
	.word	0x01
	.word	.aav_v0
	.word	.aav_v1

.aav_v0: .byte	4
	.ascii	"PLAY"
.aav_v1: .byte	15
	.ascii	"TOURNAMENT@MODE"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; beastie feastie

.an_bf0:	.byte	0x10, 0x01*4, 0x02*4, 0x01*4, 0x02*4	; beastie
.an_bf1:	.byte	0x15, 0x0d*4, 0x0e*4, 0x0f*4, 0x0e*4	; gator
.an_bf2:	.byte	0x15, 0x22*4, 0x23*4, 0x23*4, 0x22*4	; monkey
.an_bf3:	.byte	0x0e, 0x19*4, 0x19*4, 0x1a*4, 0x1a*4	; bunny 

beastf:
	.word	.BFname
	.word	.BFcprt1
	.word	0x0000
	.word	bfvars
	.word	Obfvars
	.word	v_pengoman
	.word	c_pengoman
	.byte	RCP_BEASTF
	.byte	RCG_BEASTF
	.word	.an_bf0		; 
	.word	.an_bf1		; 
	.word	.an_bf2		; 
	.word	.an_bf3		; 
	.byte	0x00		; 1p only
	.byte 	SPACE5b

.BFname:
	.byte   15
	.ascii	"BEASTIE[FEASTIE"
.BFcprt1:
	.byte	15
	.ascii	"@1984[EPOS[CORP"

bfvars:
	.word	0x03
	.word	.bfv_v0
	.word	.bfv_v1
	.word	.bfv_v2
Obfvars:
	.word	0x03
	.word	.bfv_v0
	.word	.bfv_v1
	.word	.bfv_v2

.bfv_v0: .byte	4
	.ascii	"EASY"
.bfv_v1: .byte	6
	.ascii	"MEDIUM"
.bfv_v2: .byte	4
	.ascii	"HARD"


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; lizard wizard

.an_lw0:	.byte	0x10, 0x01*4, 0x02*4, 0x01*4, 0x02*4	; beastie
.an_lw1:	.byte	0x15, 0x0d*4, 0x0e*4, 0x0f*4, 0x0e*4	; gator
.an_lw2:	.byte	0x15, 0x22*4, 0x23*4, 0x23*4, 0x22*4	; monkey
.an_lw3:	.byte	0x0e, 0x19*4, 0x19*4, 0x1a*4, 0x1a*4	; bunny 

lizwiz:
	.word	.LWname
	.word	.LWcprt1
	.word	0x0000
	.word	lwvars
	.word	Olwvars
	.word	liz_vid
	.word	liz_col
	.byte	RCP_LIZWIZ
	.byte	RCG_LIZWIZ
	.word	.an_lw0		; 
	.word	.an_lw1		; 
	.word	.an_lw2		; 
	.word	.an_lw3		; 
	.byte	0x00		; 1p only
	.byte 	SPACE40

.LWname:
	.byte   13
	.ascii	"LIZARD@WIZARD"
.LWcprt1:
	.byte	10
	.byte	0x18
	.ascii	"1985@SUNN"

lwvars:
	.word	0x02
	.word	.lwv_v0
	.word	.lwv_v1
Olwvars:
	.word	0x02
	.word	.lwv_v0
	.word	.lwv_v1

.lwv_v0: .byte	6
	.ascii	"NORMAL"
.lwv_v1: .byte	4
	.ascii	"HARD"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; MR TNT

.an_mr0:	.byte	0x09, XYF+0x05*4, XYF+0x05*4, XYF+0x03*4, XYF+0x03*4	; dude
.an_mr1:	.byte	0x09, XYF+0x11*4, XYF+0x12*4, XYF+0x13*4, XYF+0x14*4	; score
.an_mr2:	.byte	0x09, XYF+0x13*4, XYF+0x12*4, XYF+0x11*4, XYF+0x14*4	; score
.an_mr3:	.byte	0x09, YF+0x03*4, XYF+0x05*4, XYF+0x05*4, YF+0x03*4	; dude

mrtnt:
	.word	.MRname
	.word	.MRcprt1
	.word	0x0000
	.word	mrvars
	.word	Omrvars
	.word	tnt_vid
	.word	tnt_col
	.byte	RCP_MRTNT
	.byte	RCG_MRTNT
	.word	.an_mr0		; no anim yet
	.word	.an_mr1		; no anim yet
	.word	.an_mr2		; no anim yet
	.word	.an_mr3		; no anim yet
	.byte	0x00
	.byte 	SPACE40

.MRname:
	.byte   6
	.ascii	"MR@TNT"
.MRcprt1:
	.byte	11
	.byte	26
	.ascii	"1983@TELKO"

mrvars:
	.word	0x01
	.word	.mrv_v0
	.word	.mrv_v1
Omrvars:
	.word	0x01
	.word	.mrv_v0
	.word	.mrv_v1

.mrv_v0: .byte	4
	.ascii	"PLAY"
.mrv_v1: .byte	15
	.ascii	"TOURNAMENT@MODE"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; EGGOR (AKA "eyes")

.an_eg0:	.byte	0x07, YF+0x01*4, YF+0x02*4, YF+0x02*4, YF+0x01*4	; bird dude (player)
.an_eg1:	.byte	0x14, XYF+0x08*4, XYF+0x09*4, YF+0x08*4, YF+0x09*4	; egg dude (player)
.an_eg2:	.byte	0x14, YF+0x08*4, YF+0x09*4, XYF+0x08*4, XYF+0x09*4	; egg dude
.an_eg3:	.byte	0x07, XYF+0x02*4, XYF+0x02*4, XYF+0x01*4, XYF+0x01*4	; bird dude

eggor:
	.word	.EGname
	.word	.EGcprt1
	.word	0x0000
	.word	egvars
	.word	Oegvars
	.word	egg_vid
	.word	egg_col
	.byte	RCP_EGGOR
	.byte	RCG_EGGOR
	.word	.an_eg0		; no anim yet
	.word	.an_eg1		; no anim yet
	.word	.an_eg2		; no anim yet
	.word	.an_eg3		; no anim yet
	.byte	0x00
	.byte 	SPACE40

.EGname:
	.byte   5
	.ascii	"EGGOR"
.EGcprt1:
	.byte	11
	.ascii	"/1983@TELKO"

egvars:
	.word	0x01
	.word	.egv_v0
	.word	.egv_v1
Oegvars:
	.word	0x01
	.word	.egv_v0
	.word	.egv_v1

.egv_v0: .byte	4
	.ascii	"PLAY"
.egv_v1: .byte	15
	.ascii	"TOURNAMENT@MODE"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Pengo

.an_pn0:	.byte	0x0f, 0x04*4, 0x05*4, 0x04*4, 0x05*4	; pengo
.an_pn1:	.byte	0x05, 0x14*4, 0x14*4, 0x15*4, 0x15*4	; sno bee (blue)
.an_pn2:	.byte	0x03, 24*4, 25*4, 25*4, 24*4		; sno bee (pink)
.an_pn3:	.byte	0x0f, 31*4, 31*4, 30*4, 30*4	; pengo 2

pengo:
	.word	.PNname
	.word	.PNcprt1
	.word	0x0000
	.word	pengvars
	.word	Opengvars
	.word	peng_vid
	.word	peng_col
	.byte	RCP_PENGO
	.byte	RCG_PENGO
	.word	.an_pn0
	.word	.an_pn1
	.word	.an_pn2
	.word	.an_pn3
	.byte	rsf_NOHISAVE | rsf_p1only
	.byte 	SPACE20

.PNname:
	.byte   5
	.ascii	"PENGO"
.PNcprt1:
	.byte	10
	.ascii	"@1982 SEGA"

pengvars:
Opengvars:
	.word	0x04
	.word	.pnv_v0
	.word	.pnv_v1
	.word	.pnv_v2
	.word	.pnv_v3

.pnv_v0: .byte	4
	.ascii	"EASY"
.pnv_v1: .byte	6
	.ascii	"MEDIUM"
.pnv_v2: .byte	4
	.ascii	"HARD"
.pnv_v3: .byte	9
	.ascii	"VERY HARD"
