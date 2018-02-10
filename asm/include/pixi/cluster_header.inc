;-------------------------------------------------
; cluster_header.inc
;-------------------------------------------------

macro LDE()
	LDA !cluster_num,y
	AND #$80
endmacro

