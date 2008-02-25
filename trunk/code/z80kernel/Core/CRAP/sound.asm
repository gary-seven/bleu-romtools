;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Pac Menu code - sound - all of the sound routines
;
;  #Id: sound.asm,v 1.10 2004/02/06 20:10:00 jerry Exp $
;
;  much of the sound code started from Fred Konopaska's code
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module crapsound



;	PLAY MUSIC AND SOUND ROUTINES
credsnd:
	push	af
	push	bc
	ld	a, #1
	ld	(sounden),a	;SOUND ON
	ld	bc, #0x0001
	call	soundc	
	call	sleep
	ld	bc, #0x0001
	call	soundd	
	;call	sleep
	ld	bc, #0x0001
	call	sounde	
	call	sleep
	ld	bc, #0x0001
	call	soundf	
	;call	sleep
	ld	bc, #0x0001
	call	soundg	
	call	sleep
	ld	bc, #0x0001
	call	sounda	
	;call	sleep
	ld	bc, #0x0001
	call	soundb	
	call	sleep
	xor	a
	ld	(sounden),a	;SOUND OFF
	pop	bc
	pop	af
	ret

fansnd:
	ret
	push	af
	push	bc	
	ld	a, #0x01
	ld	(sounden), a	;SOUND ON
	ld	bc, #0x0004
	call	soundc	
	call	sleep
	ld	bc, #0x0004
	call	soundd	
	call	sleep
	ld	bc, #0x0004
	call	sounde	
	call	sleep
	ld	bc, #0x000a
	call	soundg
	call	sleep
	ld	bc, #0x0004
	call	sounde	
	call	sleep
	ld	bc, #0x0010
	call	soundg
	call	sleep	
	xor	a
	ld	(sounden),a	;SOUND OFF
	pop	bc
	pop	af
	ret

clksnd1:
	push	af
	push	bc	
	ld	a, #0x01
	ld	(sounden),a	;SOUND ON
	call	soundd
	ld	bc, #0x0001
	call	sleep
	xor	a
	ld	(sounden),a	;SOUND OFF
	pop	bc
	pop	af		
	ret

clksnd2:
	push	af
	push	bc	
	ld	a, #0x01
	ld	(sounden),a	;SOUND ON
	call	soundc
	ld	bc, #0x0001
	call	sleep
	call	soundb
	ld	bc, #0x0002
	call	sleep
	xor	a
	ld	(sounden),a	;SOUND OFF
	pop	bc
	pop	af		
	ret

clksnd3:
	push	af
	push	bc	
	ld	a, #0x01
	ld	(sounden),a	;SOUND ON
	call	soundb
	ld	bc, #0x0001
	call	sleep
	call	soundc
	ld	bc, #0x0002
	call	sleep
	xor	a
	ld	(sounden),a	;SOUND OFF
	pop	bc
	pop	af		
	ret

clksnd4:
	push	af
	push	bc	
	ld	a, #0x01
	ld	(sounden),a	;SOUND ON
	call	sounda
	ld	bc, #0x0001
	call	sleep
	xor	a
	ld	(sounden),a	;SOUND OFF
	pop	bc
	pop	af		
	ret

	
;		THESE ROUTINES PLAY 7 NOTES ON VOICE 2

	sndWave	= 0x01		; waveform to use

soundc:
	ld	a, #0x0F	;VOICE 2
	ld	(v2_vol),a	;VOLUME

	xor	a
	ld	(0x5057),a	;FREQUENCY MIDDLE C
	ld	(0x5059),a
	ld	a, #0x01
	ld	(0x5058),a
	ld	a, #0x06
	ld	(v2_freq),a

	ld	a, #sndWave
	ld	(0x504A),a	;WAVE
	ret

soundd:
	ld	a, #0x0F	;VOICE 2
	ld	(0x505A),a	;VOLUME

	xor	a
	ld	(0x5059),a	;FREQUENCY D
	ld	a, #0x02
	ld	(0x5057),a
	ld	a, #0x01
	ld	(0x5058),a
	ld	a, #0x06
	ld	(v2_freq),a

	ld	a, #sndWave
	ld	(0x504a),a	;WAVE
	ret

sounde:	
	ld	a, #0x0F	;VOICE 2
	ld	(0x505A),a	;VOLUME

	xor	a
	ld	(0x5059),a	;FREQUENCY E
	ld	a, #0x04
	ld	(0x5057),a
	ld	a, #0x01
	ld	(0x5058),a
	ld	a, #0xa
	ld	(v2_freq),a

	ld	a, #sndWave
	ld	(0x504a),a	;WaVE
	ret

soundf:	
	ld	a, #0x0F	;VOICE 2
	ld	(v2_vol),a	;VOLUME

	xor	a
	ld	(0x5059),a	;FREQUENCY F
	ld	a, #0x05
	ld	(0x5057),a
	ld	a, #0x01
	ld	(0x5058),a
	ld	a, #0xD
	ld	(v2_freq),a

	ld	a, #sndWave
	ld	(0x504A),a	;WAVE
	ret

soundg:
	ld	a, #0x0F	;VOICE 2
	ld	(0x505A),a	;VOLUME

	xor	a
	ld	(0x5059),a	;FREQUENCY G
	ld	a, #0x08
	ld	(0x5057),a
	ld	a, #0x01
	ld	(0x5058),a
	ld	a, #0x08
	ld	(v2_freq),a

	ld	a, #sndWave
	ld	(0x504A),a	;WAVE
	ret

sounda:
	ld	a, #0x0F	;VOICE 2
	ld	(0x505A),a	;VOLUME

	xor	a
	ld	(0x5059),a	;FREQUENCY A
	ld	a, #0xB
	ld	(0x5057),a
	ld	a, #0x01
	ld	(0x5058),a
	ld	a, #0x08
	ld	(v2_freq),a

	ld	a, #sndWave
	ld	(0x504A),a	;WAVE
	ret

soundb:
	ld	a, #0x0F	;VOICE 2
	ld	(0x505A),a	;VOLUME

	xor	a
	ld	(0x5059),a	;FREQUENCY B
	ld	a, #0x0E
	ld	(0x5057),a
	ld	a, #0x01
	ld	(0x5058),a
	ld	a, #0x0E
	ld	(v2_freq),a

	ld	a, #sndWave
	ld	(0x504A),a	;WAVE
	ret
