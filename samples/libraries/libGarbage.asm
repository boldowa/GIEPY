;-----------------------------------------------------------
; Library that gave up thinking...
;-----------------------------------------------------------

;-----------------------------------------------------------
; --- ShowShooterSmoke ---
;   [Input]
;     A  : The number for extended sprite for generate
;     X  : Index of currently executing shooter
;   [Return]
;     none.
;-----------------------------------------------------------
!TimeToShow	= 27
ShowShooterSmoke:
	xba
	;--- Find free spot
	ldy.b	#3
-	lda.w	$17c0|!base2,y
	beq	.SetSmoke
	dey
	bpl	-
	rtl

;----- smoke generate main
.ShooterSmokeDispX
	db	$f4,$0c
.SetSmoke
	xba
	sta.w	$17c0|!base2,y
	lda.w	!178b,x
	sta.w	$17c4|!base2,y
	lda.b	#!TimeToShow
	sta.w	$17cc|!base2,y
	lda.b	$94
	cmp.w	!179b,x
	lda.b	$95
	sbc.w	!17a3,x
	lda.w	!179b,x
	phx
	ldx.b	#0
	bcc	+
	inx
	clc
+	adc.l	.ShooterSmokeDispX,x
	sta.w	$17c8|!base2,y
	plx
	rtl
