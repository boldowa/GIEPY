@include
;-------------------------------------------------------------------------------
; GIEPY Sprite load code hijacks
;-------------------------------------------------------------------------------

; Sprite type bit mask
!SprTypeMask := $07

pushpc
	%org_assert_long2($02a837, 0285,1029)
		autoclean jsl	SeparateExBits

	%org_assert_long2($02a861, 0585,ceb7)
		jml	SprLoadHijack

if !true == !EXTRA_BYTES_EXT
	%org_assert_long2($02a846, 80e8,c8c8)
		jml	LoadNextSprHijack
endif

pullpc

;-------------------------------------------------
; Separate Extra Bits
;-------------------------------------------------
SeparateExBits:
	%putdebug("SeparateExBits")
	and.b	#$10		; original proc
	sta.b	$02		; original proc

	;--- get sprite type
	lda.b	[$ce],y
	and.b	#$0c
	sta.b	$07		; extra bits
	lsr
	lsr
	sta.b	$06		; sprite type
	rtl

;-------------------------------------------------
; Sprite Load Hijack
;-------------------------------------------------
SprLoadHijack:
	%putdebug("SprLoadHijack")
	lda.b	[$ce],y		; a = sprite num.
	sta.b	$05
	dey

	phx
	php
	xba
	lda.b	$06		; sprite type
	xba			; a = sprite type/num
	rep	#$30
	tax
	sep	#$20

	;--- check type
	lda.l	SprTypeTable,x
	sep	#$30
	and.b	#!SprTypeMask
	bne	.isCustom

.nonCustom
	plp
	plx
	lda.b	$05
	%jml_assert_long(jml, $02a866, 90e7c9)	; return to orig smw routine
	;jml	$02a866|!bankB

.isCustom
	%putdebug("isCustom")
	dec
	bne	+

	;--- normal custom sprite
	lda.b	$07		; exita bits
	ora.b	#$80		; set bit7(custom sprite) :  C---EE--
	sta.b	$07
	plp
	plx
	jml	$02a8dd|!bankB		; jump to normal sprite routine

+	dec
	bne	+
	jmp	LoadShooter

+	dec
	beq	LoadGenerator

	dec
	beq	LoadInitializer

	dec
	bne	.nonCustom

LoadScroller:
	%putdebug("LoadScroller")
	;--- check layer1 and layer2 scroll
	lda.w	!scroll_num_l1
	ora.w	!scroll_num_l2
	beq	+			; scroller is already working. return.
	plp
	plx
	jml	$02a846|!bankB		; jmp to LoadNextSprite

	;--- Execute scroller if any scroller isn't working
+	plp
	plx				;\  swap stack
	phy				; | for smw routine
	phx				;/ 

	;--- read extra_bytes
	if !true == !EXTRA_BYTES_EXT
		iny
		jsr	LoadExBytesSub
		beq	+			;\  Load extra bytes.
	-	lda.b	[$ce],y			; | if extra bytes size greater than 4,
		sta.l	!extra_byte_scr		; | extra byte will overwrite by
		iny				; | latest extra byte value.
		dec	$04			; |
		bne	-			;/
	+
	endif

	lda.b	$07			; extra bits
	ora.b	#$80
	sta.l	!extra_bits_l1scr	; C---EE--
	sta.l	!extra_bits_l2scr	; C---EE--
	lda.b	$05
	inc
	sta.w	!scroll_num_l1
	sta.w	!scroll_num_l2
	jml	$02a87c|!bankB		; jmp to LoadScrollSprite



LoadGenerator:
	%putdebug("LoadGenerator")
	plp
	plx
	lda.b	$07			; extra bits
	ora.b	#$80
	sta.l	!extra_bits_gen
	lda.b	$05
	inc	a
	sta.w	!18b9
	stz.w	!1938
	if !true == !EXTRA_BYTES_EXT
		iny
		jsr	LoadExBytesSub
		beq	.return
	-	lda.b	[$ce],y			;\  Load extra bytes.
		sta.l	!extra_byte_gen		; | if extra bytes size greater than 4,
		iny				; | extra byte will overwrite by
		dec	$04			; | latest extra byte value.
		bne	-			;/
	.return
		inx
		jml	$02a82e|!bankB		; jmp to LoadSpriteLoopStart
	else
		jml	$02a846|!bankB		; jmp to LoadNextSprite
	endif
	

LoadInitializer:
	%putdebug("LoadInitializer")
	phy

	if !true == !EXTRA_BYTES_EXT
		phy
		iny
		iny
		php
		rep	#$20
		lda.b	$ce
		sta.b	$98
		lda.b	$cf
		sta.b	$99
		sep	#$20
		tya
		clc
		adc.b	$98
		sta.b	$98
		bcc	.notCarry
		inc	$99
	.notCarry
		plp
		ply
	endif

	dey
	sty.b	$03

	;--- backup $05-$06 for LoadNextSprite routine.
	rep	#$10
	lda.b	$05
	pha
	php

	;--- call initializer
	sep	#$30
	jsl	CallInitializer

	;--- restore $05-06
	plp
	pla
	sta.b	$05
	sep	#$10

	ply
	plp
	plx
	jml	$02a846|!bankB			; jmp to LoadNextSprite


LoadShooter:
	%putdebug("LoadShooter")
	plp
	plx
	stx.b	$02
	dey
	sty.b	$03

	;--- search empty slot
	ldx.b	#$07
-	lda.w	!1783,x
	beq	+
	dex
	bne	-

+	dec.w	$18ff|!base2
	bpl	+
	lda.b	#$07
	sta.w	$18ff|!base2
+	ldx.w	$18ff|!base2
	ldy.w	$17b3|!base2,x
	lda.b	$07			; extra bits
	ora.b	#$80
	sta.l	!extra_bits_sh,x
	lda.b	#0
	sta.w	!1938
	ldy.b	$03
	lda.b	$05
	inc
	sta.w	!1783,x
	jml	$02aba8|!bankB



;-------------------------------------------------
; Load Next Sprite Hijack
;-------------------------------------------------
if !true == !EXTRA_BYTES_EXT
	LoadNextSprHijack:
		%putdebug("LoadNextSprHijack")
		; Skip extra bytes
		dey
		tya
		phx
		php
		rep	#$10
		ldx.b	$05			; load table inx
		clc
		adc.l	EBLengthTable,x
		;sep	#$10
		plp
		plx
		tay
		inx
		jml	$02a82e|!bankB
endif

