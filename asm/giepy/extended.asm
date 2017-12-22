@include
;-------------------------------------------------------------------------------
; GIEPY Extended Sprite code
;-------------------------------------------------------------------------------
pushpc
	%org_assert_long2($029b1b, 08d0,9da4)
		jml	ExSpriteHijack
pullpc


;-------------------------------------------------
; Extra sprite handler
;-------------------------------------------------
ExSpriteHijack:
	ldy.b	$9d
	bne	.exec
	ldy.w	!176f,x
	beq	.exec
	dec.w	!176f,x
.exec
	cmp.b	#19
	bcs	.isCustom
	jml	$029b27|!bankB		; smw orig extra sprites

.isCustom
	sec
	sbc.b	#19
ExtraReloc:
	jsl	$008000			; * This address will be overwrote by sprite tool.
	jml	$029b12|!bankB		; return to smw routine

