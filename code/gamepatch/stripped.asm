;
; stripped.asm
;
;   A stripped down version of puckman

.area 	.CODE (ABS)

;  0    Initial version

; NOTE: This file should be assembled with 'asz80', which is 
;	in the 'zcc 0.96' package.  It uses the ".bound" extension
;	written by myself, which notifies you if you go past a boundary

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; some hardware defines

        watchdog = 0x50c0       ; watchdog timer
        in0      = 0x5000       ; input 1: p1 inputs
        in1      = 0x5040       ; input 2: coin inputs
	sounden  = 0x5001	; sound enable
	inten	 = 0x5000	; interrupt enable
        vidram   = 0x4000       ; video ram base
        colram   = 0x4400       ; color ram base
	sprites  = 0x4ff0	; sprite control registers
        letter   = 0x4a
        offs1    = 0x20
        offs2    = 0x40

	playerup   = 0x4e09
	credram    = 0x4e6e
	livesleft  = 0x4e14
	livesdisp  = 0x4e15
	livesstart = 0x4e6f

        p1_port         = in0
        p1_up           = 0
        p1_left         = 1
        p1_right        = 2
        p1_down         = 3

        start_port      = in1
        start1          = 5
        start2          = 6

; code hooks

	textout	= 0x2c5e

.org 0x234e
.bound 0x3fff
