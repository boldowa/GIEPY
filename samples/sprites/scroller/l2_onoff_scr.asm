;-------------------------------------------------
; Layer2 On/Off switch controlled scroller
; ----------------------------------------
;   This is a scroller that disassembled
;   SMW's sprite($DC) and fixes a earth quake issue.
;-------------------------------------------------

;-----------------------------
; configs
;-----------------------------
!Sound				= 9
!VibrationFramesOnSwitch	= 16
!EarthQuakeFrames		= 32

ScrollData:
;		         Down   Up
.DestY		dw	$0020, $00c1
.MaxSpeed	dw	  -64,    64
.Acceleration	dw	   -1,     1	; This value must be a value that can divide MaxSpeed.



;-------------------------------------------------

ScrollMain:
	%putdebug("ScrollMain")
	ldx.w	$14af|!base2		; on/off status
	beq	+
	stz.w	$1442|!base2
	ldx.b	#2

+	lda.w	$1442|!base2		; is finished?
	bne	++			; return

	cpx.w	$1443|!base2
	beq	.Check
	dec.w	$1445|!base2
	bpl	+
	stx.w	$1443|!base2

+	lda.w	$1468|!base2
	eor.b	#$01
	sta.w	$1468|!base2

	stz.w	$144c|!base2
	stz.w	$144d|!base2
++	rts

.Check
	lda.b	#!VibrationFramesOnSwitch
	sta.w	$1445|!base2
	rep	#$20
	lda.w	$1468|!base2
	cmp.w	ScrollData_DestY,x
	bne	.DoScroll
	cpx.b	#0
	bne	+
	lda.w	#!Sound
	sta.w	$1dfc|!base2
	lda.w	#!EarthQuakeFrames
	sta.w	$1887|!base2
	inc.w	$1442|!base2
+	ldx.b	#0
	stx.w	$14af|!base2
	bra	.return

.DoScroll
	lda.w	$144c|!base2		; load layer2 Y speed
	cmp.w	ScrollData_MaxSpeed,x	;
	beq	+
	clc
	adc.w	ScrollData_Acceleration,x
	sta.w	$144c|!base2		; update layer2 Y speed
+	ldx.b	#6			; Layer2 Y pos index
	jsl	UpdateLayerPos
.return
	sep	#$20
	rts


;-----------------------------
; Init routine entry point
;-----------------------------
init:
	rep	#$20

	;lda.w	#$0b00
	;sta.w	!scroll_num_l1

	; reset flags
	stz.w	$1442|!base2
	stz.w	$1444|!base2

	; reset scroll pos
	stz.w	$1446|!base2
	stz.w	$1448|!base2
	stz.w	$144a|!base2
	stz.w	$144c|!base2

	; reset value for calculate layer pos
	stz.w	$144e|!base2
	stz.w	$1450|!base2
	stz.w	$1452|!base2
	stz.w	$1454|!base2

	sep	#$20
	rtl

;-----------------------------
; Main routine entry point
;-----------------------------
main:
	phb
	phk
	plb
	jsr	ScrollMain
	plb
	rtl

