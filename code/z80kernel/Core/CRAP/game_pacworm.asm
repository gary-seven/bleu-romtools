;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Pac Menu code - pac-worm game
;
;  2004-02  Scott "Jerry" Lawrence
;
;  $Id: game_pacworm.asm,v 1.2 2004/05/23 23:10:29 jerry Exp $
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module pacworm

; nibbler-style game, using Ms. Pac graphics. 

; use the RLE routine to store "maps":  (perhaps use full-screengrabs?)
; maps:
;   mspac
;   mspacatk
;   pac
;   hangly
;   nibbleresque
;   bonus round (open, with just pellets)

; game modes:
;   clear each board
