;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Pac Menu code - hs example
;
;  2005-04  Scott "Jerry" Lawrence
;
;  $Id: hs_example.asm,v 1.2 2005/04/03 03:15:53 jerry Exp $
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;    This module is not compiled in to the final binary, it is just
;    meant as an example to show how a game should load/save 
;    high scores through the rom.

;    the important addresses to know are:
;	ghs_base	whatever the base address in ram is of the 
;			game's high score ram.  (assuming 3 contigous bytes)
;	hsRamAddr	(in ram.asm) the address that contains the 
;			precomputed pointer to NVRam to load/store the score

;    Ms-Pac high score:
;       $4e88   TOP CC
;       $4e89   TOP BB
;       $4e8a   TOP AA
ghs_base	= 0x4e88


; GHS_FROM_NVRAM
;   installs the high score from NV-RAM, if there was one in there.
;   call this from the "press start" intercept patch
;           mod     af, bc, de, hl
GHS_FROM_NVRAM:
        ld      hl, (hsRamAddr) ; hl points to score NV-RAM
        ld      a, (hl)         ; load the changed flag
        cp      #0
        ret     z               ; it's zero, do nothing
        inc     hl              ; hl now points to score data

        ld      de, #ghs_base   ; destination
        ld      bc, #0x0003     ; 3 bytes to copy
        ldir                    ; copy the bytes

	    ; some games (like mspac) may need further help to 
	    ; initially draw the score to the screen
	ret

; GHS_TO_NVRAM
;   extracts the score from the game, stores it in NV-RAM
;   call this just before returning to the menu
;           mod     af, bc, de, hl
GHS_TO_NVRAM:
        ld      hl, (hsRamAddr) ; hl points to score NV-RAM
        set     #1, (hl)        ; set "contains score" flag
        inc     hl              ; move to score ram (source)
        ex      de, hl
        ld      hl, #ghs_base   ; destination
        ld      bc, #0x0003     ; 3 bytes to copy
        ldir                    ; copy the bytes
        ret


