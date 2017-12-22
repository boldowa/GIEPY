@include
;-------------------------------------------------------------------------------
; Common define
;-------------------------------------------------------------------------------
!__common_inc ?= 0
if 0 == !__common_inc
!__common_inc := 1

!NULL	:= 0
!true	:= 1
!false	:= 0

!DEBUG	?= !false	; if debug mode isn't defined,
			; disable debug.

endif
