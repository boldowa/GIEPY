;-------------------------------------------------
; shooter_header.inc
;-------------------------------------------------

macro LDE()
	LDA !shoot_num,x
	AND #$40
endmacro

