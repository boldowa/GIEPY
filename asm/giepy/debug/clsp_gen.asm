@include
;-------------------------------------------------
; Cluster sprite initialize code for debug
; (boo ring generator)
;
; [Scratch Memory Usage]
;   $00       ... Screen boundary X(Y) + offset
;   $01       ... Screen boundary X(Y) + offset
;   $02       ... 's' bit (#$10 or #$00)
;   $03       ... Sprite data index
;   $04       ... free to use
;   $05       ... Sprite number
;   $06       ... Sprite group (Extra bits >> 2)
;   $07       ... Extra bits
;   $08 - $0f ... free to use
;
; [etc]
;   $98       ... extra bytes pointer (if you enabled it)
;-------------------------------------------------

!NumOfGenerate = 10

ClusterGen:
	%putdebug("ClusterGen")
	phb
	phk
	plb
	jsr	GenCluster
	plb
	rtl

GenCluster:
	;--- Get extra byte
if !true == !EXTRA_BYTES_EXT
;	ldy.b	$03		; get sprite data index
;	iny			;\
;	iny			; | skip until extra byte
;	iny			;/
;	lda.b	[$ce],y		;   get extra byte
	lda.b	[$98]
	dec
else
	lda.b	#!NumOfGenerate-1
endif
	sta.b	$0e		; set cluster sprite number

	;--- check generation
	ldy.w	$18ba|!base2	; boo ring inx
	cpy.b	#$02
	bcs	.return		; already exists 2 rings, can't generate.

	ldy.b	#$01		;\
	lda.b	$05		; | ... $05 = sprite number
	and.b	#$01		; |
	beq	+		; | $0f = Rotational direction of boo ring
	ldy.b	#$ff		; |
+	sty.b	$0f		;/

	;--- generate cluster boo looop
	ldx.b	#$13		; Cluster sprite max
.generateLoop
	;--- search empty slot of cluster sprite
	lda.w	!cluster_num,x             
	bne	.SearchNextSlot

	;--- set cluster sprite parameter
	lda.b	#$04			; cluster sprite 04 = ring boo
	sta.w	!cluster_num,x             
	lda.w	$18ba|!base2
	sta.w	$0f86|!base2,x
	lda	$0e
	sta.w	$0f72|!base2,x
	lda.b	$0f
	sta.w	$0f4a|!base2,x
	stz	$0f
	beq	.skip
	ldy.b	$03
	lda	[$ce],y			; Byte format: YYYYEEsy
	ldy.w	$18ba|!base2
	pha
	and.b	#$f0
	sta.w	$0fb6|!base2,y
	pla
	and.b	#$01
	sta.w	$0fb8|!base2,y
	lda	$00
	sta.w	$0fb2|!base2,y
	lda	$01
	sta.w	$0fb4|!base2,y
	lda.b	#$00
	sta.w	$0fba|!base2,y
	lda	$02
	sta.w	$0fbc|!base2,y
.skip
	dec.b	$0e
	bmi	.done
.SearchNextSlot
	dex
	bpl	.generateLoop
.done
	inc.w	$18ba|!base2
.return
	rts                       ; return 

