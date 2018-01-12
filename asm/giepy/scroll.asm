@include
;-------------------------------------------------------------------------------
; GIEPY Scroll Sprite code
;-------------------------------------------------------------------------------
pushpc
	%org_assert_long2($02a874, e938,05a5)
		jsl	SetScrollExtraBits
		nop

	%org_assert_long2($05bcd6, 9cab,4b8b)
		jml	ScrollSprInitHijack

	if !true == !sa1
		%org_assert_long2($05bc79, d030,9dad)
	else
		%org_assert_long2($05bc79, d000,9dad)
	endif
		jml	CallScrollerL1Hijack

	if !true == !sa1
		%org_assert_long2($05bcaa, f074,3fad)
	else
		%org_assert_long2($05bcaa, f014,3fad)
	endif
		jml	CallScrollerL2Hijack
pullpc


;-------------------------------------------------
; Set scroller extra bits for origin scroller
;   this code work original scroller only.
;   When use custom scroller, this routine will
;   be not execute.
;-------------------------------------------------
SetScrollExtraBits:
	%putdebug("SetScrollExtraBits")
	lda.b	$07
	sta.l	!extra_bits_l1scr
	sta.l	!extra_bits_l2scr
if !EXTRA_BYTES_EXT
	lda.b	#0
	sta.l	!extra_byte_scr
endif
	lda.b	$05			;\ 
	sec				; | origin codes
	sbc.b	#$e7			;/
	rtl


;-------------------------------------------------
; Scroll Sprite Init Hijack
;-------------------------------------------------
ScrollSprInitHijack:
	%putdebug("ScrollSprInitHijack")
	phb
	phk
	plb
	stz.w	!scroll_layer_bit	; layer1 process
	lda.l	!extra_bits_l1scr
	bmi	.isCustom
	lda.b	#(($050000|!bankB)>>16)
	pha
	plb
	jml	$05bcdc|!bankB		; return to ScrollSprInit(smw origin)

.isCustom
	%putdebug("Custom Scroller")
	jsl	.CallScrollSprL1Init
	lda.b	#$04
	sta.w	!scroll_layer_bit	; layer2 process
	lda.l	!extra_bits_l2scr	;\  check layer2 scroll sprite bits
	bmi	+			;/  (for changed it in layer1 process)
	lda.b	#(($050000|!bankB)>>16)
	pha
	plb
	jml	$05bce4|!bankB		; return to ScrollSprInit(smw origin)
+	jsl	.CallScrollSprL2Init
	plb
	rtl

.CallScrollSprL1Init
	and.b	#$0c				;\ Y <- Layer1 Extra bits
	tay					;/
	lda.b	#(ScrL1InitTablePtr>>16)	;\
	sta.b	$0b				; | $09-$0b <- Layer1 scroller init ptr group adddress
	rep	#$20				; |
	lda.w	#(ScrL1InitTablePtr&$ffff)	; |
	sta.b	$09				;/
	sep	#$20
	lda.w	!scroll_num_l1
	dec
	;--- Call generator main routine
	jmp	CallSpriteFunction

.CallScrollSprL2Init
	and.b	#$0c				;\ Y <- Extra bits
	tay					;/
	lda.b	#(ScrL2InitTablePtr>>16)	;\
	sta.b	$0b				; | $09-$0b <- Layer2 scroller init ptr group adddress
	rep	#$20				; |
	lda.w	#(ScrL2InitTablePtr&$ffff)	; |
	sta.b	$09				;/
	sep	#$20
	lda.w	!scroll_num_l2
	dec
	;--- Call generator main routine
	jmp	CallSpriteFunction


;-------------------------------------------------
; Call layer1 scroller Hijack
;-------------------------------------------------
CallScrollerL1Hijack:
	%putdebug("CallScrollerL1Hijack")
	lda.w	$9d|!base1
	beq	+
	jml	$05bc49|!bankB			; rts

+	lda.l	!extra_bits_l1scr		; C---EE--
	bmi	.isCustom
	jml	$05bc7e|!bankB			; origin
.isCustom
	%putdebug("CallScrollerL1Hijack - Custom")
	and.b	#$0c				;\ Y <- Layer1 Extra bits
	tay					;/
	lda.b	#(ScrL1MainTablePtr>>16)	;\
	sta.b	$0b				; | $09-$0b <- Layer1 scroller init ptr group adddress
	rep	#$20				; |
	lda.w	#(ScrL1MainTablePtr&$ffff)	; |
	sta.b	$09				;/

	;--- set return bank
	pla					; pull for swap
	ldx.b	#(($050000|!bankB)>>16)		;
	phx					; 
	pha					; restore
	;--- set scroller num
	sep	#$20
	lda.w	!scroll_num_l1
	dec
	;--- Call generator main routine
	jmp	CallSpriteFunction


;-------------------------------------------------
; Call layer2 scroller Hijack
;-------------------------------------------------
CallScrollerL2Hijack:
	%putdebug("CallScrollerL2Hijack")
	lda.l	!extra_bits_l2scr
	bmi	.isCustom
	lda.w	!scroll_num_l2
	beq	.ret05
	jml	$05bcaf|!bankB
.ret05
	jml	$05bc49|!bankB

.isCustom
	%putdebug("CallScrollerL2Hijack - Custom")
	ldy.w	$9d|!base1
	bne	.ret05

	and.b	#$0c				;\ Y <- Layer1 Extra bits
	tay					;/
	lda.b	#(ScrL2MainTablePtr>>16)	;\
	sta.b	$0b				; | $09-$0b <- Layer1 scroller init ptr group adddress
	rep	#$20				; |
	lda.w	#(ScrL2MainTablePtr&$ffff)	; |
	sta.b	$09				;/

	;--- set return bank
	pla					; pull for swap
	ldx.b	#(($050000|!bankB)>>16)		;
	phx					; 
	pha					; restore
	;--- set scroller num
	sep	#$20
	lda.w	!scroll_num_l2
	dec
	;--- Call generator main routine
	jmp	CallSpriteFunction


;-------------------------------------------------
; Scroll nop routine
;-------------------------------------------------
ScrollL1Nop:
	stz.w	!scroll_num_l1
	rtl
ScrollL2Nop:
	stz.w	!scroll_num_l2
	rtl



if !true == !DEBUG
DebugScrL1InitTable:
	dd CScrollerInit
	dd CSRET
	dd CScrollerInit

DebugScrL2InitTable:
	dd CSRET
	dd CScrollerInit
	dd CScrollerInit

DebugScrL1MainTable:
	dd CScrollerMain
	dd CSRET
	dd CScrollerMain

DebugScrL2MainTable:
	dd CSRET
	dd CScrollerMain
	dd CScrollerMain

	rep 40 : dd CSRET

CSRET:	rtl

	incsrc	"debug/cscroller.asm"
endif
