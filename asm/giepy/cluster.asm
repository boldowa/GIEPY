@include
;-------------------------------------------------------------------------------
; GIEPY Cluster Sprite code
;-------------------------------------------------------------------------------
pushpc
	if !true == !sa1
		%org_assert_long2($02f815, f078,92bd)
	else
		%org_assert_long2($02f815, f018,92bd)
	endif
	jml	HandleClusterHijack
pullpc

;-------------------------------------------------
; Cluster sprite handler
;-------------------------------------------------
HandleClusterHijack:
	;lda.w	$0100|!base2		;\  If in mosaic routine, don't run sprite.
	;cmp.b	#$13			; |   ... Why???
	;beq	.return			;/
	lda.w	$1892|!base2,x
	bne	+
.return
	jml	$02f81d|!bankB

+	cmp.b	#9			; smw is using 9 slots.
	bcs	.isCustom
	pea	$f81c			;\  jsr $02f81a
	jml	$02f821|!bankB		;/    ... return -> 02f81d

.isCustom
	sec
	sbc.b	#9
	ldy.b	#(($020000|!bankB)>>16)
	phy
	pea	$f81c			;  jsl [<ClusterReloc>]
if !PIXI_COMPATIBLE
	phx
	php
	phb
	txy
endif

ClusterReloc:
if !PIXI_COMPATIBLE
	jsl	SpriteNop		; * This address will be overwrote by sprite tool.
	plb
	plp
	plx
	rtl
else
	jml	SpriteNop		; * This address will be overwrote by sprite tool.
endif

