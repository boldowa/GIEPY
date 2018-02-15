;-------------------------------------------------
; Custom sprite shooter
;-------------------------------------------------
;-------------------
; configs
;-------------------
!Group		:= 3		; number of group for sprite to generate
!Interval	:= 20
!Sound		:= 20
!SmokeExSprite	:= 1		; extended sprite 1: smoke

;---------------------------------------
; --- main code ---
;---------------------------------------
_ret2:	rts
Shoot:
	lda.w	!17ab,x
	bne	_ret2
	lda.b	#!Interval
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
	%putdebug("Call - $02a9de")
if !sa1
	xba				;\  In case of sa1, this routine will destroy
	pha				; | A register higher 8 bits.
					;/  So, backup it in stack.

	jsl	$02a9de|!bankB		;   

	pla				;\  restore A register higher 8 bits.
	xba				;/

	tya				; restore subroutine's result.
else
	jsl	$02a9de|!bankB		;
endif
	bmi	.ret

	;--- generate parameter set
	lda.b	#!Sound
	sta.w	$1dfc|!base2
	lda.b	#1
	sta.w	!14c8,y
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
	
	;--- Generate sprite
;	if !true == !EXTRA_BYTES_EXT
;	lda.l	!extra_byte_sh,x	; --- get generate sprite
;	endif
	xba				; --- get number of sprite to generate from cfg's "unique number".
	phx
	tyx
	sta.l	!new_sprite_num,x	; --- set
	jsl	$07f7d2|!bankB		;
	lda.b	#$80|(!Group<<2)	; >>> Important: Set the extra bits before
	sta.l	!extra_bits,x		;                call $0187a7(custom tweak filler) routine
	jsl	$0187a7			; --- set custom sprite's tweak.
	plx

	;--- generate smoke
	lda.b	#!SmokeExSprite
	jsl	ShowShooterSmoke
.ret
	rts


;-----------------------------
; program entry point
;-----------------------------
;	print "MAIN ",pc
main:					; <--- You can use this label for main.
	phb				;      (also init, There is no distinction between uppercase and lowercase letters.)
	phk
	plb
	jsr	Shoot
	plb
	rtl

