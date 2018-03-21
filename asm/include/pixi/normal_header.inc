;-------------------------------------------------
; normal_header.inc
;-------------------------------------------------

;----------------------------------------
; LoaD Extra bit
;----------------------------------------
macro LDE()
	LDA !extra_bits,x
	AND #$04
endmacro


;----------------------------------------
; Branch if Exita bit is Set
;----------------------------------------
macro BES(label)
	%LDE()
	bne	<label>
endmacro


;----------------------------------------
; Branch if Exita bit is Clear
;----------------------------------------
macro BEC(label)
	%LDE()
	beq	<label>
endmacro

