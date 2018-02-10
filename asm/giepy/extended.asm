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
if !PIXI_COMPATIBLE
	phx
	php
	phb
	txy
endif

ExtraReloc:
	jsl	SpriteNop		; * This address will be overwrote by sprite tool.
if !PIXI_COMPATIBLE
	plb
	plp
	plx
endif
	jml	$029b12|!bankB		; return to smw routine

if !PIXI_COMPATIBLE
ExecutePtrLongPb:
	ply
	sty.b	$02
	rep	#$30
	and.w	#$00ff
	sta.b	$03
	asl
	adc.b	$03
	tay

	pla
	sta.b	$03
	iny
	lda.b	[$02],y
	sta.b	$00
	iny
	lda.b	[$02],y
	sta.b	$01
	sep	#$30
	xba
	pha
	plb
	txy
	jmp	[$0000]		
endif

