@include
;-------------------------------------------------------------------------------
; GIEPY some tweaks ...
;-------------------------------------------------------------------------------
pushpc
	;---------------------------------------
	; Patch goal tape init
	; to get Extra Bits in $187b+x
	;---------------------------------------
	if !true == !sa1
		%org_assert_long2($01c089, 9d32,58bd)	; sa-1 remaped
	else
		%org_assert_long2($01c089, 9d14,d4bd)
	endif
		jml	GoalInitTweak

	;---------------------------------------
	; Clear extra ram on Load OW
	;---------------------------------------
	%org_assert_long2($00a1ad, 10ca,1a74)
		jsl	ClearExtraRamOnOWLoad
		nop

	;---------------------------------------
	; Clear extra ram on Load Level
	;---------------------------------------
	%org_assert_long2($02ac53, 3e9c,10e2)
		jsl	ClearExtraRamOnLevelLoad
		nop
pullpc


;-------------------------------------------------
; Goal Init tweak
;-------------------------------------------------
GoalInitTweak:
	lda.l	!extra_bits,x
	sta.w	!187b,x
	lda.w	!14d4,x
	jml	$01c091|!bankB


;-------------------------------------------------
; Clear extra ram on Level Load
;-------------------------------------------------
ClearExtraRamOnLevelLoad:
	sep	#$10
	stz.w	!scroll_num_l1
	jsr	ClearExtraRam
	rtl


;-------------------------------------------------
; Clear extra ram on OW Load
;-------------------------------------------------
ClearExtraRamOnOWLoad:
-	stz.b	$1a,x
	dex
	bpl	-
	php
	jsr	ClearExtraRam
	plp
	rtl


;---------------------------------------
; Extra ram clear subroutine
;---------------------------------------
ClearExtraRam:
	sep	#$30
	lda.b	#0
	;--- clear generator / scroller ram
	sta.l	!extra_bits_gen
	sta.l	!extra_bits_l1scr
	sta.l	!extra_bits_l2scr
	if !true == !EXTRA_BYTES_EXT
		sta.l	!extra_byte_gen
		sta.l	!extra_byte_scr
	endif

	;--- clear shooter ram
	ldx.b	#7
-	sta.l	!extra_bits_sh,x
	if !true == !EXTRA_BYTES_EXT
		sta.l	!extra_byte_sh,x
	endif
	dex
	bpl	-

	;--- clear sprite ram
	if !true == !sa1
		ldx.b	#21
	else
		ldx.b	#11
	endif
-	sta.l	!extra_bits,x
	sta.l	!new_sprite_num,x
	sta.l	!extra_prop_1,x
	sta.l	!extra_prop_2,x
	if !true == !EXTRA_BYTES_EXT
		sta.l	!extra_byte_1,x
		sta.l	!extra_byte_2,x
		sta.l	!extra_byte_3,x
		sta.l	!extra_byte_4,x
	endif
	dex
	bpl	-

	rts

