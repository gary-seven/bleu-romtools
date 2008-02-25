;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;	tasklist.asm
;
;	list of available tasks, management routines
;
;	2006  Scott Lawrence
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module tasklist

; task list table
;	- this lists all of the tasks that the menu system provides
tasklist:
	.word	movieTask
	.word	absurdTask
	.word	quadblokTask
	NTASKS = .-tasklist
	.word	ssaveTask
	.word	testModeTask
	.word	pactrackerTask
	FTASKS = .-tasklist

;; no need to have this in the menu...
	.word 	ssaveTask

; each entry in the above table should contain a few items:
;
;	.word	title		; pascal string - (title)
;	.word	desc1		; pascal string - (description)
;	.word	desc2		; pascal string - (description)
;	.word	att0		; function pointer - attract intialization
;	.word	att		; function pointer - attract poll
;	.word	game0		; function pointer - game initialization
;	.word	game		; function pointer - game poll
;	.byte	credits		; number of credits per play
;	.word	variant list	; list of variants

	TITITLE	= 0
	TIDESC1 = 2
	TIDESC2 = 4
	TIATT0  = 6
	TIATT   = 8
	TIGAME0 = 10
	TIGAME  = 12
	TICRED	= 14
	TIVARS	= 15


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    ; init the task system - reset to task 0
taskinit:
	xor	a
	ld	(taskid), a	; set the current task ID to 0
	call	tasksel
	ret

    ; selects the current task (fills in taskptr)
tasksel:
	ld	hl, #(tasklist)
	ld	d, #0
	ld	a, (taskid)
	ld	e, a
	rst	0x10

	ld	(taskptr), hl

	xor	a
	ld	(variant), a
	ret


    ; move the task pointer to the next available task
tasknext:
	ld	a, (taskid)	; a = taskid
	cp	#(NTASKS-2)	;    (a < NTASKS)
	jr	c, .tnxt0001	; if (a < NTASKS) carry is set
	xor	a		; else a=0
.tnxt000:
	ld	(taskid), a	; taskid = a
	call	tasksel		; tasksel()
	ret
.tnxt0001:
	add	a, #2		; a=a+2
	jr	.tnxt000


    ; move the task pointer to the prevoius available task
taskprev:
	ld	a, (taskid)	; a = taskid
	cp	#2
	jr	c, .tprv001	; if ( a<2 ) carry is set
	sub	a, #2		; a=a-2
.tprv000:
	ld	(taskid), a	; taskid = a
	call	tasksel		; tasksel()
	ret
.tprv001:
	ld	a, #(NTASKS-2)	; a = NTASKS-2
	jr	.tprv000


    ; adjust "variant" to be the next available variant
variantnext:
	call	tNVarsToA
	cp	#0x00
	ret	z		; no variants.  return

	dec	a		; use 0..x instead of 0..x+1
	ld	b, a		; store it in b
	ld	a, (variant)	
	cp	b
	jr	z, .vn000

	inc	a
	ld	(variant), a
	ret

.vn000:
	xor	a
	ld	(variant), a
	ret
	


    ; adjust "variant" to be the previous available variant
variantprev:
	call	tNVarsToA
	cp	#0x00
	ret	z		; no variants.  return

	ld	a, (variant)
	cp	#0x00
	jr	z, .vp000
	dec	a
	ld	(variant), a
	ret
.vp000:
	call	tNVarsToA
	dec	a
	ld	(variant), a
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Accessors/Dereferencors

	; dereference the current task to set the title up for putstr()
tTitleToHL:
	ld	de, #TITITLE
.ttstr:
	ld	hl, (taskptr)
	rst	0x10
	ret

tDesc1ToHL:
	ld	de, #TIDESC1
	jr	.ttstr

tDesc2ToHL:
	ld	de, #TIDESC2
	jr	.ttstr

	; move the number of credits into A
tCredsToA:
	ld	de, #TICRED
	ld	hl, (taskptr)
	add	hl, de
	ld	a, (hl)
	ret

	; fill HL with the pointer to the current varlist
tVarsToHL:
	ld	de, #TIVARS
	jr	.ttstr
	
	; fill A with the current nvars
tNVarsToA:
	call	tVarsToHL
	ld	a, (hl)		; a is the number of vars  byte[0] = NVars
	ret			; return it

tVarToHL:
	ld	hl, #.TVXXX	; backfill
	call	tNVarsToA	; nvars
	cp	#0x00
	ret	z		; no vars, just return

	; HL is already at the right place
	inc	hl		; move to string list top
	
	; HL now points to the first item, it needs to be dereferenced
	ld	a, (variant)	; current variant number
	add	a		; change it to an array indx
	ld	d, #0x00	; D  = 0x00
	ld	e, a		;  E =     A
	rst	0x10
	ret
.TVXXX:
	.byte	4
	.ascii	"NONE"
	

	; call the current att0 routine
t_att0:
	ld	de, #TIATT0
.ttfcn:
	ld	hl, (taskptr)
	rst	0x10
	jp	(hl)


	; call the current att routine
t_att:
	ld	de, #TIATT
	jr	.ttfcn


	; call the current game0 routine
t_game0:
	ld	de, #TIGAME0
	jr	.ttfcn


	; call the current game routine
t_game:
	ld	de, #TIGAME
	jr	.ttfcn

