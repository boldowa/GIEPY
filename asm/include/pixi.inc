@include
;**** please don't change here ****
!__pixi_inc ?= 0
if 0 == !__pixi_inc
!__pixi_inc := 1
if 0 == !SYSTEM_INITIALIZING
;**********************************
;-------------------------------------------------
; pixi.inc
; --------
;
; This file is included when assembling with asar
; if pixi cpmpatible mode.
;-------------------------------------------------
if !false == !LIBRARY_ASSEMBLING
	!ExtendedOffset = 0
	!ClusterOffset = 0
	if 1 == !g_sprite_type		; in "Sprite" section asm
		if 1 == !g_sprite_subtype
			; normal sprite
			incsrc pixi/normal_header.inc
		elseif 2 == !g_sprite_subtype
			; shooter
			incsrc pixi/shooter_header.inc
		elseif 3 == !g_sprite_subtype
			; generator
			incsrc pixi/generator_header.inc
	;	elseif 4 == !g_sprite_subtype
	;		; initializer
	;		incsrc pixi/initializer_header.inc
	;	elseif 5 == !g_sprite_subtype
	;		; scroller
	;		incsrc pixi/scroller_header.inc
		endif
	elseif 2 == !g_sprite_type	; in "Extended" section asm
		; extended
		incsrc pixi/extended_header.inc
	elseif 3 == !g_sprite_type	; in "Cluster" section asm
		; cluster
		incsrc pixi/cluster_header.inc
	;elseif 4 == !g_sprite_type	; in "Overworld" section asm
	;	; overworld
	;	incsrc pixi/overworld_header.inc
	endif ; sprite type switch
endif ; library
;**** please don't change here ****
endif ; system initializing
endif ; include
;**********************************
