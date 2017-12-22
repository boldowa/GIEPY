@include
;-------------------------------------------------
; ExShooter main code for debug
;-------------------------------------------------
ExShooter:
	%putdebug("ExShooter")
	phb
	phk
	plb
	jsr	Bullet
	plb
	rtl

Bullet:
	lda.w	!17ab,x
	bne	.ret
	lda.b	#4
	sta.w	!17ab,x
	lda.w	!178b,x
	cmp.b	$1c
	lda.w	!1793,x
	sbc.b	$1d
	bne	.ret
	lda.w	!179b,x
	cmp.b	$1a
	lda.w	!17a3,x
	sbc.b	$1b
	bne	.ret
	lda.w	!179b

	sec
	sbc.b	$1a
	clc
	adc.b	#$10
	cmp.b	#$10
	bcc	.ret
	lda.b	$94
	sbc.w	!179b,x
	clc
	adc.b	#$11
	cmp.b	#$22
	bcc	.ret
	jsl	$02a9de|!bankB
	bmi	.ret

	%putdebug("GenBullet")
	lda.b	#19
	sta.w	$1dfc|!base2
	lda.b	#1
	sta.w	!14c8,y
	if !true == !EXTRA_BYTES_EXT
	lda.l	!extra_byte_sh,x	; get generate sprite
	endif
	sta.w	!9e,y
	lda.w	!179b,x
	sta.w	!e4,y
	lda.w	!17a3,x
	sta.w	!14e0,y
	lda.w	!178b,x
	sec
	sbc.b	#1
	sta.w	!d8,y
	lda.w	!1793,x
	sbc.b	#0
	sta.w	!14d4,y
	phx
	tyx
	jsl	$07f7d2|!bankB		; gen
	plx
.ret
	rts

