@include
;-------------------------------------------------------------------------------
; GIEPY Normal Sprite code
;-------------------------------------------------------------------------------
pushpc
	org $02a9ca					; This address is overritten by SA-1 patch.
		dl	InitSpriteTableFix		; So strict checking is impossible.

	if !true == !sa1
		%org_assert_long2($02a9d4, c87f,d69d)
	else
		%org_assert_long2($02a9d4, c81f,e29d)
	endif
		jsl	LoadNrmSprExtraInfo

	if !true == !sa1
		%org_assert_long2($018172, 429d,08a9)
	else
		%org_assert_long2($018172, c89d,08a9)
	endif
		jsl	SprInitHijack
		nop

	if !true == !sa1
		%org_assert_long2($0185c3, b274,919c)
	else
		%org_assert_long2($0185c3, b514,919c)
	endif
		jsl	SprMainHijack
		nop

	if !true == !sa1
		%org_assert_long2($07f787, 6b33,769d)
	else
		%org_assert_long2($07f787, 6b15,a09d)
		;%org_assert_long4($07f787, 9d,a0,15,6b)
	endif
		jml	SprEraseTweak

	if !true == !sa1
		%org_assert_long2($018127, f032,42bd)
	else
		%org_assert_long2($018127, f014,c8bd)
	endif
		jml	SpriteStatusHandlerHijack

	if !true == !sa1
		%org_assert_long2($02a9a3, aab4,b2da)
	else
		%org_assert_long2($02a9a3, aa9e,b5da)
	endif
		jml	SilverPowBitCheck

	%org_assert_long2($02a971, 04a5,c8c8)
		jsl	SetExtraBits

	;--- Tweaks for extra bits
	;  ... scratch ram change: $06 -> $08
	org $02a8ee
		db	$08
	org $02a8fd
		db	$08
	org $02a915
		db	$08
	org $02a91f
		db	$08
	org $02a933
		db	$08

	;--- Tweaks for extra bits
	;  ... except extra bits from $14d4
	org $02a964		; Horizontal level
		db	$01
	org $02a94c		; Vertical level
		db	$01

	;--- Tweaks for status handler
	;  ... change proc
	if !sa1
		%org_assert_long2($01d43e, 6032,429e)
	else
		%org_assert_long2($01d43e, 6014,c89e)
	endif
		jsr	$8133
		rtl

	%org_assert_word($0180b5, b8ad)
		jml CheckConversion

	;--- Hammer Bro init routine overwrite
	;  ... for old-style generate custom sprite
	%org_assert_jsl($0187a7, $02da59)
		jml	SetConversionFlag
	%org_assert_word($0182b3, 87a7)
		dw	$85c2

	;--- Unused routine call
	;  ... for generate custom sprite
	%org_assert_word($0187c5, af22)
		jml	SetSpriteTables

pullpc


;-------------------------------------------------
; InitSpriteTableFix
;-------------------------------------------------
InitSpriteTableFix:
	; backup extra bits
	lda.l	!extra_bits,x
	pha
if !sa1
	phx
	phy
	sep	#$10
endif
	jsl	$07f7d2|!bankB
if !sa1
	rep	#$10
	ply
	plx
endif
	pla
	sta.l	!extra_bits,x
	rtl


;-------------------------------------------------
; Load Sprite's Extra info
;-------------------------------------------------
LoadNrmSprExtraInfo:
	%putdebug("LoadNrmSprExtraInfo")
	sta.w	!1fe2,x

if !true == !EXTRA_BYTES_EXT
	;--- get extra bytes length
	jsr	LoadExBytesSub
	beq	.return			;\  Load extra bytes.
-	lda.b	[$ce],y			; | if extra bytes size greater than 7,
	sta.l	!extra_byte_1,x		; | extra_byte_4 will overwrite by
	iny				; | latest extra byte value.
	dec	$04			; |
	beq	.return			; |
-	lda.b	[$ce],y			; |
	sta.l	!extra_byte_2,x		; |
	iny				; |
	dec	$04			; |
	beq	.return			; |
-	lda.b	[$ce],y			; |
	sta.l	!extra_byte_3,x		; |
	iny				; |
	dec	$04			; |
	beq	.return			; |
-	lda.b	[$ce],y			; |
	sta.l	!extra_byte_4,x		; |
	iny				; |
	dec	$04			; |
	bne	-			;/
else
	iny
endif
.return
	rtl

;--- Subroutine for read extra bytes
if !true == !EXTRA_BYTES_EXT
LoadExBytesSub:
	iny
	phx
	php
	rep	#$10
	ldx.b	$05
	lda.l	EBLengthTable,x
	sep	#$10
	plp
	plx
	sec
	sbc.b	#3
	sta.b	$04
	rts
endif


;-------------------------------------------------
; Sprite Init Hijack
;-------------------------------------------------
SprInitHijack:
	%putdebug("SprInitHijack")
	lda.l	!sprite_flags,x
	bit.b	#$01
	bne	.skip
	
	lda.b	#$08
	sta.w	!14c8,x
	
	lda.l	!extra_bits,x	; C---EE-- : C=custom, EE=sprite grp
	bmi	.isCustom
.ret
	rtl

.skip
	pla
	pla
	pea	$85c1
	rtl

.isCustom
	%putdebug("SprInitHijack - Custom")
	jsl	SetSpriteTables
	lda.l	!new_code_flag
	beq	.ret

	lda.l	!extra_bits,x	; C---EE-- : C=custom, EE=sprite grp
	and.b	#$0c
	tay
	lda.b	#(SprInitTablePtr>>16)		;\
	sta.b	$0b				; | $00-$02 <- Shooter ptr group adddress
	rep	#$20				; |
	lda.w	#(SprInitTablePtr&$ffff)	; |
	sta.b	$09				;/
	sep	#$20
	pla
	pla
	pea	$85c1
	lda.l	!new_sprite_num,x
	jmp	CallSpriteFunction

;---------------------------------------
; CheckConversion hijack
;   ... Re-initialize any sprites marked for conversion.
;---------------------------------------
CheckConversion:
	lda.l	!sprite_flags
	bpl	.checkCluster

	if !true == !sa1
		ldx.b	#21
	else
		ldx.b	#11
	endif
-	lda.l	!sprite_flags,x
	bit.b	#$01
	beq	.next
	and.b	#$fe
	sta.l	!sprite_flags,x

	lda.l	!extra_bits,x
	ora.b	#$80
	bit.b	#$08
	bne	.custom
	and.b	#$7f
.custom
	sta.l	!extra_bits,x

	lda.b	#$ff
	sta.w	!161a,x

	jsl	SetSpriteTables
.next
	dex
	bpl	-

	lda.l	!sprite_flags
	and.b	#$7f
	sta.l	!sprite_flags
.checkCluster
	lda.w	$18b8|!base2
	beq	.skipCluster
	jml	$0180ba|!bankB

.skipCluster
	jml	$0180be|!bankB

;---------------------------------------
; SetConversionFlag subroutine
;   ... Set flag to re-initialize custom sprite after the sprite loop.
;   args:
;     - !new_sprite_num,x ... custom sprite num
;---------------------------------------
SetConversionFlag:
	; set master conversion flag
	lda.l	!sprite_flags
	ora.b	#$80
	sta.l	!sprite_flags

	; mark sprite as needing conversion
	lda.l	!sprite_flags,x
	ora.b	#$01
	sta.l	!sprite_flags,x

	rtl

;---------------------------------------
; SetSpriteTables subroutine
;   ... Init custom sprite's tweaks.
;   args:
;     - !extra_bits,x     ... extra bits
;     - !new_sprite_num,x ... custom sprite num
;---------------------------------------
SetSpriteTables:
	phy
	phb
	phk
	plb
	php

	; calc minimum inx
	sep	#$30
	lda.l	!extra_bits,x
	and.b	#$0c
	tay			; y = extra_bits
	lda.l	!new_sprite_num,x
	sec
	sbc.w	SprTweakTablePtr,y

	; mask
	rep	#$20
	and.w	#$00ff
	asl	#4
	sta.b	$00

	; Get array of Tweak struct
	lda.w	SprTweakTablePtr+1,y
	clc
	adc.b	$00
	sta.b	$00
	rep	#$10
	sep	#$20
	lda.w	SprTweakTablePtr+3,y
	adc.b	#0
	pha
	plb
	ldy.b	$00

	; Get tweaks
	lda.w	$0001,y
	sta.w	!9e,x
	lda.w	$0002,y
	sta.w	!1656,x
	lda.w	$0003,y
	sta.w	!1662,x
	lda.w	$0004,y
	sta.w	!166e,x
	and.b	#$0f
	sta.w	!15f6,x
	lda.w	$0005,y
	sta.w	!167a,x
	lda.w	$0006,y
	sta.w	!1686,x
	lda.w	$0007,y
	sta.w	!190f,x

	; return if it isn't new code
	lda.w	$0000,y
	sta.l	!new_code_flag
	bne	.useUserCode
	;--- use smw original sprite. so, custom bit = OFF
	lda.l	!extra_bits,x		; C---EE--
	and.b	#$7f
	sta.l	!extra_bits,x		; 0---EE--
	bra	.ret

	; read extra prop if newcode
.useUserCode
	lda.w	$0008,y
	sta.l	!extra_prop_1,x
	lda.w	$0009,y
	sta.l	!extra_prop_2,x
.ret
	plp
	plb
	ply
	rtl

;-------------------------------------------------
; Sprite Main Hijack
;-------------------------------------------------
SprMainHijack:
	%putdebug("SprMainHijack")
	stz.w	$1491|!base2
	
	lda.l	!sprite_flags,x
	bit.b	#$01
	bne	.skip
	
	lda.l	!extra_bits,x
	bmi	.isCustom
	if !true == !sa1
		lda.b	($b4)		; b4 = sprite_num_pointer
	else
		lda	!9e,x
	endif
.ret
	rtl

.skip
	pla
	pla
	pea	$85c1
	rtl

.isCustom
	%putdebug("SprMainHijack - Custom")
	and.b	#$0c
	tay
	lda.b	#(SprMainTablePtr>>16)		;\
	sta.b	$0b				; | $09-$0b <- Sprite ptr group adddress
	rep	#$20				; |
	lda.w	#(SprMainTablePtr&$ffff)	; |
	sta.b	$09				;/
	sep	#$20
	pla
	pla
	pea	$85c1
	lda.l	!new_sprite_num,x
	;jmp	CallSpriteFunction


;-------------------------------------------------
; Call Sprite Function
;   Input:
;     $09 - $0b : sprite ptr group ptr
;             Y : Extra bits (group no. << 2)
;             A : Sprite number
;-------------------------------------------------
CallSpriteFunction:

if !PIXI_COMPATIBLE
	phx			;\
	php			; | PIXI saves registers
	phb			; | before call sprites routine.
	jsl	.subCall	; |
	plb			; |
	plp			; |
	plx			; |
	rtl			;/
.subCall
endif
	; calc minimun inx
	sep	#$30
	sec
	sbc.b	[$09],y

	; mask
	rep	#$20
	and.w	#$00ff
	asl
	asl
	pha

	; Get array of routine's address
	iny
	lda.b	[$09],y
	pha
	iny
	lda.b	[$09],y
	sta.b	$0a
	pla
	sta.b	$09

	; Get code address
	rep	#$10
	ply
	sep	#$20
	lda.b	[$09],y		;\
	pha			; | Get code address
	iny			; | (24bits)
	lda.b	[$09],y		; |
	pha			; |
	iny			; |
	lda.b	[$09],y		;/

	xba			;\  This is extention for the future.
	iny			; | Currently, this byte will be ignored.
	lda.b	[$09],y		; |
	xba			;/

	sta.b	$0b		;\
if !PIXI_COMPATIBLE		; |
	pha			; |
	plb			; | store address
endif				; |
	pla			; |
	sta.b	$0a		; |
	pla			; |
	sta.b	$09		;/
	sep	#$30
if !PIXI_COMPATIBLE
	txy
	lda.w	!14c8,x
endif	
	jml	[$0009|!base1]	; jump to code


;-------------------------------------------------
; Sprite Erase Hijack
;-------------------------------------------------
SprEraseTweak:
	sta.w	!15a0,x
	stz.w	!15ac,x
	dec
	sta.l	!extra_bits,x
	if !true == !EXTRA_BYTES_EXT
		sta.l	!extra_byte_1,x
		sta.l	!extra_byte_2,x
		sta.l	!extra_byte_3,x
		sta.l	!extra_byte_4,x
	endif
	rtl


;-------------------------------------------------
; Sprite Status Handler Hijack
;-------------------------------------------------
SpriteStatusHandlerHijack:
	lda.w	!sprite_status,x	; sprite status
	cmp.b	#2			;\  if status is Init or Erace, 
	bcc	.CallDefaultHandler	;/  execute normal handler.
	cmp.b	#8
	bne	.StsIsNotNormal
	jml	$0185c3|!bankB		; jmp to CallSpriteMain

.StsIsNotNormal
	pha
	lda.l	!extra_bits,x
	bmi	.isCustom
	pla
.CallDefaultHandler
	jml	$018133|!bankB

	;--- Main routine can handle non-normal status
	;    if you use custom sprites.
.isCustom
	lda.l	!extra_prop_2,x		;\  If extra prop 2 b7 = true, 
	bmi	.CallMain		; | the default handling is not used at all.
					;/  MAIN is responsible for handling all statuses.

	pha				;\
	lda.b	$02,s			; | Handle status
	jsl	$01d43e|!bankB		; |
	pla				;/

	asl	a			;\  If extra prop 2 b6 = true, 
	bmi	.CallMain		;/  MAIN is called after the default handling of all statuses. 
	
	;--- MAIN routine handles $0a-$0c, and $03
	lda.b	$01,s
	cmp.b	#9
	bcs	.CallMain
	cmp.b	#3
	beq	.CallMain

	;--- MAIN routine doesn't handle these status:
	;    $00-$02, $04-$09
	pla
	jml	$0185c2|!bankB

.CallMain
	lda.l	!extra_bits,x
	and.b	#$0c
	tay
	lda.b	#(SprMainTablePtr>>16)		;\
	sta.b	$0b				; | $00-$02 <- Shooter ptr group adddress
	rep	#$20				; |
	lda.w	#(SprMainTablePtr&$ffff)	; |
	sta.b	$09				;/
	sep	#$20
	pla

	lda.b	#(($010000|!bankB)>>16)
	pha
	pea	$85c1
	lda.l	!new_sprite_num,x
	jmp	CallSpriteFunction
	


;-------------------------------------------------
; Silver Pow bit check
;-------------------------------------------------
SilverPowBitCheck:
	%putdebug("SilverPowBitCheck")
	phx			; push for origin codes.

	lda.l	!extra_bits,x	;\  check custom spr bit
	bmi	.isCustom	;/
if !sa1				;\
	lda.b	($b4)		; | Get sprite number
else				; |
	lda.b	!9e,x		; |
endif				;/

if !sa1
	phy			;\  Vitor's SA-1 patch codes
	rep	#$10		; |
	ply			;/

	php			;\
	rep	#$30		; | mask for 16bits X register
	and.w	#$00ff		; |
	plp			;/
endif
	tax
	lda.l	$07f659|!bankB,x	; smw original sprite's $190f tweak
	jml	$02a9ab|!bankB
;-------------------
.isCustom
	jsr	.GetSilverPByte	; read silver p-switch byte into A register.
if !sa1
	phy			;\  Vitor's SA-1 patch codes
	rep	#$10		; |
	ply			;/
endif
	jml	$02a9ab|!bankB


;-----------------------------
; A = extra bits
;-----------------------------
.GetSilverPByte
	%putdebug("GetSilverPByte")
	phb
	phk
	plb
	php
;-------------------	
	sep	#$30
	and.b	#$0c
	tay			; Y = extra bits

	;--- get table index
	lda.l	!new_sprite_num,x
	sec
	sbc.w	SprTweakTablePtr,y

	; mask
	rep	#$20
	and.w	#$00ff
	asl	#4
	sta.b	$00

	; Get array of Tweak struct
	lda.w	SprTweakTablePtr+1,y
	clc
	adc.b	$00
	sta.b	$00
	rep	#$10
	sep	#$20
	lda.w	SprTweakTablePtr+3,y
	adc.b	#0
	pha				;\  switch bank
	plb				;/
	ldy.b	$00

	; get tweak byte
	lda.w	$0007,y			; cfg line2 tweaks 6 ($190F value)
;-------------------	
	plp
	plb
	rts

;-------------------------------------------------
; Set extra bits
;-------------------------------------------------
SetExtraBits:
	;--- set sprite number
	lda.b	$05
	sta.l	!new_sprite_num,x
	;--- set extra bits
	lda.b	$07
	sta.l	!extra_bits,x
	iny
	iny
	lda.b	$04
	rtl

;-------------------------------------------------
; sprite nop routine
;-------------------------------------------------
SpriteNop:
	stz.w	!sprite_status,x
	rtl

