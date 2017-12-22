@include
;-------------------------------------------------------------------------------
; Install check define
;-------------------------------------------------------------------------------
!__inschk_inc ?= 0
if 0 == !__inschk_inc
!__inschk_inc := 1

; init insflags
!INSFLAGS := 0

!OmitInsCheck	?= !false	; bit speed up
				; if enabled it on sprite insertion.
				; but, you shouldn't enable it when
				; system install.
				;
				; Extra bytes option check will ignored
				; when enable it.

;---------------------------------------
; Install flag
;---------------------------------------
!INS_SA1		:= $0001
!INS_EXTRA_BYTES	:= $0002

endif
