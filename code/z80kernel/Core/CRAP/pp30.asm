;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Pac Menu code - pp30 specific hardware defines
;
;  2004-01  Scott "Jerry" Lawrence
;
;  $Id: pp30.asm,v 1.2 2004/05/23 23:10:29 jerry Exp $
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module pp30

	; PP30 specific hardware
	nvrambase	== 0xc000
	nvramsize	== 0x0800
	romport		== 0x80
	gfxport		== 0x81
