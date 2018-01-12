@include
;-------------------------------------------------------------------------------
; GIEPY Normal Sprite code
;-------------------------------------------------------------------------------
pushpc
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
	org $02a964
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

	;--- Hammer Bro init routine overwrite
	;  ... for generate custom sprite
	%org_assert_long2($0187a7, 02da,5922)
		jml	SetSpriteTables
	%org_assert_word($0182b3, 87a7)
		dw	$85c2

pullpc


;-------------------------------------------------
; Load Sprite's Extra info
;-------------------------------------------------
LoadNrmSprExtraInfo:
	%putdebug("LoadNrmSprExtraInfo")
	sta.w	!1fe2,x
	;--- set sprite number
	lda.b	$05
	sta.l	!new_sprite_num,x
	;--- set extra bits
	lda.b	$07
	sta.l	!extra_bits,x

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
	lda.b	#$08
	sta.w	!14c8,x
	
	lda.l	!extra_bits,x	; C---EE-- : C=custom, EE=sprite grp
	bmi	.isCustom
.ret
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
; SetSpriteTables subroutine
;   ... Init custom sprite's tweaks.
;   args:
;     - !extra_bits,x     ... extra bits
;     - !new_sprite_num,x ... custom sprite num
;---------------------------------------
	print "SetSpriteTable: $",pc
if 0
macro read_dp(dp)
	lda.b	[<dp>],y
	iny
endmacro
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
	pha

	; Get array of Tweak struct
	lda.w	SprTweakTablePtr+1,y
	sta.b	$00
	lda.w	SprTweakTablePtr+2,y
	sta.b	$01

	; Get tweaks
	rep	#$10
	ply
	sep	#$20
	%read_dp($00)
	sta.l	!new_code_flag
	%read_dp($00)
	sta.w	!9e,x
	%read_dp($00)
	sta.w	!1656,x
	%read_dp($00)
	sta.w	!1662,x
	%read_dp($00)
	sta.w	!166e,x
	and.b	#$0f
	sta.w	!15f6,x
	%read_dp($00)
	sta.w	!167a,x
	%read_dp($00)
	sta.w	!1686,x
	%read_dp($00)
	sta.w	!190f,x

	; return if it isn't new code
	lda.l	!new_code_flag
	bne	.useUserCode
	;--- use smw original sprite. so, custom bit = OFF
	lda.l	!extra_bits,x		; C---EE--
	and.b	#$7f
	sta.l	!extra_bits,x		; 0---EE--
	bra	.ret

	; read extra prop if newcode
.useUserCode
	%read_dp($00)
	sta.l	!extra_prop_1,x
	%read_dp($00)
	sta.l	!extra_prop_2,x
.ret
	plp
	plb
	ply
	rtl
endif

SetSpriteTables:
	print "SetSpriteTable: $",pc
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
	
	lda.l	!extra_bits,x
	bmi	.isCustom
	if !true == !sa1
		lda.b	($b4)		; b4 = sprite_num_pointer
	else
		lda	!9e,x
	endif
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
;	php
;	jsl	.sub
;	plp
;	rtl
;.sub
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
	pla			; | store address
	sta.b	$0a		; |
	pla			; |
	sta.b	$09		;/
	sep	#$30
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
	;      $00-$02, $04-$09
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
	phx
if !sa1
	lda.b	($b4)
else
	lda.b	$9e,x
endif
	pha
	lda.l	!extra_bits,x
	bmi	.isCustom

.originmode
	plx
if !sa1
	phy
	rep	#$10
	ply
endif
	lda.b	$07f659|!bankB,x	; smw original sprite's $190f tweak
	jml	$02a9ab|!bankB

.isCustom
	phb
	phk
	plb
	php

	and.b	#$0c
	tay			; y = extra_bits
	lda.l	!new_sprite_num,x
	sec
	sbc.w	SprTweakTablePtr,y
	; mask
	rep	#$20
	and.w	#$00ff
	asl	#4
	clc
	adc.w	#0008
	pha

	; Get array of Tweak struct
	lda.w	SprTweakTablePtr+1,y
	sta.b	$00
	lda.w	SprTweakTablePtr+2,y
	sta.b	$01

	; Get tweaks
	rep	#$10
	ply
	sep	#$20
	lda.b	[$00],y

	plp
	plb
	ply

if !sa1
	phy
	rep	#$10
	ply
endif

	jml	$02a9ab|!bankB


;-------------------------------------------------
; sprite nop routine
;-------------------------------------------------
SpriteNop:
	stz.w	!sprite_status,x
	rtl


if !true == !DEBUG
DebugSprTweakTable:
	%putdebug("DebugSprTweakTable")
	db $00, $93, $10, $b7, $05, $01, $00, $00, $00, $ff, $ff, $ff, $ff, $ff, $ff
	rep 10 : db $01, $36, $10, $b7, $05, $01, $00, $00, $00, $ff, $ff, $ff, $ff, $ff, $ff
	;rep 10 : db $01, $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff

DebugSprInitTable:
	dd CoinInit
	dd CoinInit
	rep 10 : dd HBrosInit

DebugSprMainTable:
	dd CoinMain
	dd CoinMain
	rep 10 : dd HBrosMain

	incsrc	"debug/hbros.asm"
endif

