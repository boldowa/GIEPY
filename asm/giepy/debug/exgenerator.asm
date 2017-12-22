@include
;-------------------------------------------------
; ExGenerator main code for debug
;-------------------------------------------------
ExGenerator:
	%putdebug("ExGenerator")
	phb
	phk
	plb
	jsr	GenSomething
	plb
	rtl

GenSomething:
	inc	$0db6|!base2
	inc	$0dbf|!base2
	lda.b	$14
	and.b	#1
	bne	+
if !true == !EXTRA_BYTES_EXT
	lda.l	!extra_byte_gen
else
	lda.b	#1
endif	
	sta.w	$1dfc|!base2
+	rts

