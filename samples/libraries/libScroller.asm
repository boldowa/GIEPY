;-------------------------------------------------
; libScroller
; -----------
;-------------------------------------------------

;---------------------------------------
; UpdateLayerPos
; --------------
; [Input]
;   X register	: position type
;		    0 - Layer1 X
;		    2 - Layer1 Y
;		    4 - Layer2 X
;		    6 - Layer2 Y
;---------------------------------------
UpdateLayerPos:
	lda.w	$144e|!base2,x
	and.w	#$00ff
	clc
	adc.w	$1446|!base2,x
	sta.w	$144e|!base2,x
	and.w	#$ff00
	bpl	+
	ora.w	#$00ff
+	xba
	clc
	adc.w	$1462|!base2,x
	sta.w	$1462|!base2,x

	;--- Invert $08
	lda.b	$08
	eor.w	#$ffff
	inc	a
	sta.b	$08
	rtl

