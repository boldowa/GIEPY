@include
;-------------------------------------------------------------------------------
; GIEPY Initializer code
;-------------------------------------------------------------------------------
pushpc
	; nothing
pullpc

;-------------------------------------------------
; Call Initializer
;   * this routine called from load.asm
;-------------------------------------------------
CallInitializer:
	lda.b	#1		;\  run cluster
	sta.w	$18b8|!base2	;/
	lda.b	$07
	and.b	#$0c
	tay
	lda.b	#(InitializerTablePtr>>16)	;\
	sta.b	$0b				; | $00-$02 <- Initializer ptr group adddress
	rep	#$20				; |
	lda.w	#(InitializerTablePtr&$ffff)	; |
	sta.b	$09				;/
	sep	#$20
	lda.b	$05				; Initializer number
	;--- Call initializer routine
	jmp	CallSpriteFunction

if !true == !DEBUG
DebugInitializerTable:
	rep 10 : dd	ClusterGen

	incsrc	"debug/clsp_gen.asm"
endif

