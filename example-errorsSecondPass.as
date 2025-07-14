MAIN:	add	r3 , LIST
LOOP:	prn	#48
	lea	STR, r6
	inc	r6
	mov	r3, K1
	sub	r1, r4
	bne	END
LOOP:	cmp	val, #-6
	bne	%END
	de	K
	jmp	%LOOP
END:	stop
STR:	.string "abcd"
LIST:	.data	6, -9
	.data	-100
.entry	K2
K:	.data	31
.extern	val1
.entry val1
