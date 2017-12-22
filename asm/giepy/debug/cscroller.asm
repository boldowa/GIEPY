@include
;-------------------------------------------------
; custom scroller code for debug
;-------------------------------------------------
CScrollerInit:
	%putdebug("CScrollerInit")
	phb
	phk
	plb
	jsr	CSI
	plb
	rtl

CScrollerMain:
	%putdebug("CScrollerMain")
	phb
	phk
	plb
	jsr	CSM
	plb
	rtl

CSI:
	lda.b	#2
	sta.b	$19
;	stz.w	$0100
	rts


CSM:
	lda.b	$14
	and.b	#3
	bne	+
if !true == !EXTRA_BYTES_EXT
	lda.l	!extra_byte_scr
else
	lda.b	#1
endif
	jsl	$05b329|!bankB
+	rts

