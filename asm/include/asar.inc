@include
;**** please don't change here ****
!__asar_inc ?= 0
if 0 == !__asar_inc
!__asar_inc := 1
if 0 == !SYSTEM_INITIALIZING
;**********************************
;-------------------------------------------------
; asar.inc
; --------
;
; This file is included when assembling with asar.
; You can describe something common to all files.
;-------------------------------------------------
;--- You can define values here freely.

if !LIBRARY_ASSEMBLING
	; You can define values for library assembling.
	; incsrc librarydef.inc
else
	; You can define values for all type sprites.
	; incsrc spritesdef.inc

if 1 == !g_sprite_type		; in "Sprite" section asm
	; You can define values for sprites.
	if 1 == !g_sprite_subtype
		; You can define values for regular sprite here.
		; incsrc regulardef.inc
	elseif 2 == !g_sprite_subtype
		; You can define values for shooter here.
		; incsrc shooterdef.inc
	elseif 3 == !g_sprite_subtype
		; You can define values for generator here.
		; incsrc generatordef.inc
	elseif 4 == !g_sprite_subtype
		; You can define values for initializer here.
		; incsrc initializerdef.inc
	elseif 5 == !g_sprite_subtype
		; You can define values for scroller here.
		; incsrc scrollerdef.inc
	endif
elseif 2 == !g_sprite_type	; in "Extended" section asm
	; You can define values for extended sprites here.
	; incsrc extendeddef.inc
elseif 3 == !g_sprite_type	; in "Cluster" section asm
	; You can define values for cluster sprites here.
	; incsrc clusterdef.inc
elseif 4 == !g_sprite_type	; in "Overworld" section asm
	; You can define values for overworld sprites here.
	; (however, ow sprites isn't implemented.)
	; incsrc overworlddef.inc
endif ; sprite type switch
endif ; library
;**** please don't change here ****
endif ; system initializing
endif ; include
;**********************************
