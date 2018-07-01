@include
;-------------------------------------------------------------------------------
; GIEPY Shooter code
;-------------------------------------------------------------------------------
pushpc
	%org_assert_long2($02aba3, 839d,c8e9)
		jsl	SetShooterExBits
		nop

	%org_assert_word($02a8d8, 7820)
		jmp	$ab78

	if !true == !sa1
		%org_assert_long2($02abeb, 5c02,a6c8)
	else
		%org_assert_long2($02abeb, e802,a6c8)
	endif
	jml	LoadShooterExByte

	if !true == !sa1
		%org_assert_long2($02b395, f077,abbc)
	else
		%org_assert_long2($02b395, f017,abbc)
	endif
		jml	ShooterHijack
pullpc

;-------------------------------------------------
; Set shooter extra bits for origin shooter
;   this code work original shooter only.
;   When use custom shooter, this routine will
;   be not execute.
;-------------------------------------------------
SetShooterExBits:
	sbc.b	#$c8
	sta.w	!1783,x
if !EXTRA_BYTES_EXT
	lda.b	#0
	sta.l	!extra_byte_sh,x
endif
	lda.b	$07
	sta.l	!extra_bits_sh,x
	rtl

;-------------------------------------------------
; Load Shooter's Extra byte
;-------------------------------------------------

LoadShooterExByte:
	%putdebug("LoadShooterExByte")
if !true == !EXTRA_BYTES_EXT
	phy
	jsr	LoadExBytesSub
	beq	+			;\  Load extra bytes.
-	lda.b	[$ce],y			; | if extra bytes size greater than 4,
	sta.l	!extra_byte_sh,x	; | extra byte will overwrite by
	iny				; | latest extra byte value.
	dec	$04			; |
	bne	-			;/
+	ply
endif
	dey
	ldx.b	$02
	jml	$02a846|!bankB

;-------------------------------------------------
; Shooter Hijack
;-------------------------------------------------
ShooterHijack:
	%putdebug("ShooterHijack")
	pha
	ldy	!17ab,x
	beq	+
	lda	$13
	lsr
	bcc	+
	dec	!17ab,x
+	lda.l	!extra_bits_sh,x	; C---EE-- : C=custom, EE=sprite grp
	bmi	.isCustom
	pla
	jml	$02b3a4|!bankB		; SMW original shooter

.isCustom
	%putdebug("Custom Shooter")
	;--- Set args for CallSpriteFunction
	and.b	#$0c				;\ Y <- Extra bits
	tay					;/
	lda.b	#(ShooterTablePtr>>16)		;\
	sta.b	$0b				; | $00-$02 <- Shooter ptr group adddress
	rep	#$20				; |
	lda.w	#(ShooterTablePtr&$ffff)	; |
	sta.b	$09				;/
	sep	#$20
	pla					; Sprite number.
	dec

	;--- Call shooter main routine
	xba
	lda.b	#(($020000|!bankB)>>16)		;\  rtl bank
	pha					;/
	xba
	pea	$b3a6				; rtl return address
	jmp	CallSpriteFunction

