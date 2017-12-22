@include
;-------------------------------------------------------------------------------
; GIEPY Generator code
;-------------------------------------------------------------------------------
pushpc
	%org_assert_long2($02a8b4, 1acb,e938)
		jsl	SetGeneratorExBits

	%org_assert_long2($02b003, 23d0,9da4)
		jml	GeneratorHijack
pullpc


;-------------------------------------------------
; Set generator extra bits for origin generator
;   this code work original generator only.
;   When use custom generator, this routine will
;   be not execute.
;-------------------------------------------------
SetGeneratorExBits:
	pha
	lda.b	$07
	sta.l	!extra_bits_gen
	pla

	sec				;\ origin codes
	sbc.b	#$cb			; |
	inc	a			;/
	rtl


;-------------------------------------------------
; Generator Hijack
;-------------------------------------------------
GeneratorHijack:
	%putdebug("GeneratorHijack")
	ldy.b	$9d				; sprite locked
	beq	+
	jml	$02b02a|!bankB			; rts

+	lda.l	!extra_bits_gen
	bmi	.isCustom
	jml	$02b007|!bankB			; origin generator

.isCustom
	%putdebug("Custom Generator")
	jsl	.CallGenerator
	jml	$02b02a|!bankB			; rts

.CallGenerator
	;--- Set args for CallSpriteFunction
	and.b	#$0c				;\ Y <- Extra bits
	tay					;/
	lda.b	#(GeneratorTablePtr>>16)		;\
	sta.b	$0b				; | $00-$02 <- Shooter ptr group adddress
	rep	#$20				; |
	lda.w	#(GeneratorTablePtr&$ffff)	; |
	sta.b	$09				;/
	sep	#$20
	lda.w	!18b9				; generator number.
	dec
	;--- Call generator main routine
	jmp	CallSpriteFunction
	
if !true == !DEBUG
DebugGeneratorTable:
	rep 10 : dd	ExGenerator

	incsrc	"debug/exgenerator.asm"
endif

