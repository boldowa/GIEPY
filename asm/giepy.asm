;-------------------------------------------------------------------------------
; GIEPY main patch system
;
; *** CAUTION ***
;   This source shouldn't depend on any other libraries.
;   Otherwise it could break when inserting sprites.
;
; Changelog)
;   12-23-2017 :  First version.
;    1-12-2018 :  C3 beta version.
;-------------------------------------------------------------------------------
;--- for debug
!SYSTEM_INITIALIZING ?= 0
!DEBUG ?= 1
!map ?= 0
;---

incsrc "include/sa1def.inc"

!VER_MAJ := $00
!VER_MIN := $01

;---------------------------------------
; Check install condition
;---------------------------------------
if !true == !sa1
	!INSFLAGS #= !INSFLAGS|!INS_SA1
endif
if !true == !EXTRA_BYTES_EXT
	!INSFLAGS #= !INSFLAGS|!INS_EXTRA_BYTES
endif
if !PIXI_COMPATIBLE
	!INSFLAGS #= !INSFLAGS|!INS_PIXI_COMPATIBLE
endif

;--- Load $02A9C9 (for backup)
!Val_02a9ca	:= read3($02a9ca)

;-------------------------------------------------
; version info
;-------------------------------------------------
	db	"VER "
	dw	_ver_end-_ver_start
_ver_start:
	dw	!INSFLAGS
	db	!VER_MIN, !VER_MAJ
_ver_end:

;-------------------------------------------------
; Main codes
;-------------------------------------------------
	db	"CODE"
	dw	_code_end-_code_start
_code_start:
	incsrc "giepy/load.asm"
	incsrc "giepy/normspr.asm"
	incsrc "giepy/shooter.asm"
	incsrc "giepy/generator.asm"
	incsrc "giepy/initializer.asm"
	incsrc "giepy/scroll.asm"
	incsrc "giepy/extended.asm"
	incsrc "giepy/cluster.asm"
	incsrc "giepy/owspr.asm"
	incsrc "giepy/tweaks.asm"
SomethingNop:
	rtl
_code_end:


;-------------------------------------------------
; Extra Bytes length table
;-------------------------------------------------
if !true == !EXTRA_BYTES_EXT
		db	"EBLT"
		dw	_eblt_end-_eblt_start
	_eblt_start:
	EBLengthTable:
		%putdebug("EBLengthTable")
		;--- Group 0
		rep 256 : db 3
		;--- Group 1
		rep 256 : db 3
		;--- Group 2
		rep 256 : db 3
		;--- Group 3
		rep 256 : db 3
	_eblt_end:
	pushpc
		;--- This information is used by Lunar Magic.
		org $0ef30c
			dl EBLengthTable
	pullpc
endif


;-------------------------------------------------
; Sprite type table
;
;  [ Data format ]
;     bit 76543210
;         ----rttt
;
;         ttt : sprite type
;                  0 ... smw original sprite
;                  1 ... custom normal sprite
;                  2 ... custom shooter
;                  3 ... custom generator
;                  4 ... custom initializer
;                  5 ... custom scroller
;                  6 ... (unused)
;                  7 ... (unused)
;
;           r : reserved
;-------------------------------------------------
	db	"SPTT"
	dw	_sptt_end-_sptt_start
_sptt_start:
SprTypeTable:
	%putdebug("SprTypeTable")
	;--- Group 0
	rep 256 : db 0
	;--- Group 1
	rep 256 : db 0
	;--- Group 2
	rep 256 : db 0
	;--- Group 3
	rep 256 : db 0
_sptt_end:


;-------------------------------------------------
; GIEPY normal sprite tweaks ptr
;-------------------------------------------------
	db	"STWP"
	dw	_stwp_end-_stwp_start
_stwp_start:
SprTweakTablePtr:
	; min inx  : table pointer
	;--- Group 0
	db  0      :  dl !NULL
	;--- Group 1
	db  0      :  dl !NULL
	;--- Group 2
	db  0      :  dl !NULL
	;--- Group 3
	db  0      :  dl !NULL
_stwp_end:


;-------------------------------------------------
; GIEPY sprite init function ptr ptr
;-------------------------------------------------
	db	"SPIP"
	dw	_spip_end-_spip_start
_spip_start:
SprInitTablePtr:
	; min inx  : table pointer
	;--- Group 0
	db  0      :  dl !NULL
	;--- Group 1
	db  0      :  dl !NULL
	;--- Group 2
	db  0      :  dl !NULL
	;--- Group 3
	db  0      :  dl !NULL
_spip_end:


;-------------------------------------------------
; GIEPY sprite main function ptr ptr
;-------------------------------------------------
	db	"SPMP"
	dw	_spmp_end-_spmp_start
_spmp_start:
SprMainTablePtr:
	; min inx  : table pointer
	;--- Group 0
	db  0      :  dl !NULL
	;--- Group 1
	db  0      :  dl !NULL
	;--- Group 2
	db  0      :  dl !NULL
	;--- Group 3
	db  0      :  dl !NULL
_spmp_end:


;-------------------------------------------------
; GIEPY shooter function ptr ptr
;-------------------------------------------------
	db	"SHTP"
	dw	_shtp_end-_shtp_start
_shtp_start:
ShooterTablePtr:
	; min inx  : table pointer
	;--- Group 0
	db  0      :  dl !NULL
	;--- Group 1
	db  0      :  dl !NULL
	;--- Group 2
	db  0      :  dl !NULL
	;--- Group 3
	db  0      :  dl !NULL
_shtp_end:


;-------------------------------------------------
; GIEPY generator function ptr ptr
;-------------------------------------------------
	db	"GENP"
	dw	_genp_end-_genp_start
_genp_start:
GeneratorTablePtr:
	; min inx  : table pointer
	;--- Group 0
	db  0      :  dl !NULL
	;--- Group 1
	db  0      :  dl !NULL
	;--- Group 2
	db  0      :  dl !NULL
	;--- Group 3
	db  0      :  dl !NULL
_genp_end:


;-------------------------------------------------
; GIEPY initializer function ptr ptr
;-------------------------------------------------
	db	"INIP"
	dw	_inip_end-_inip_start
_inip_start:
InitializerTablePtr:
	; min inx  : table pointer
	;--- Group 0
	db  0      :  dl !NULL
	;--- Group 1
	db  0      :  dl !NULL
	;--- Group 2
	db  0      :  dl !NULL
	;--- Group 3
	db  0      :  dl !NULL
_inip_end:


;-------------------------------------------------
; GIEPY Layer1 scroller init function ptr ptr
;-------------------------------------------------
	db	"SCI1"
	dw	_sci1_end-_sci1_start
_sci1_start:
ScrL1InitTablePtr:
	; min inx  : table pointer
	;--- Group 0
	db  0      :  dl !NULL
	;--- Group 1
	db  0      :  dl !NULL
	;--- Group 2
	db  0      :  dl !NULL
	;--- Group 3
	db  0      :  dl !NULL
_sci1_end:


;-------------------------------------------------
; GIEPY Layer1 scroller main function ptr ptr
;-------------------------------------------------
	db	"SCM1"
	dw	_scm1_end-_scm1_start
_scm1_start:
ScrL1MainTablePtr:
	; min inx  : table pointer
	;--- Group 0
	db  0      :  dl !NULL
	;--- Group 1
	db  0      :  dl !NULL
	;--- Group 2
	db  0      :  dl !NULL
	;--- Group 3
	db  0      :  dl !NULL
_scm1_end:


;-------------------------------------------------
; GIEPY Layer2 scroller init function ptr ptr
;-------------------------------------------------
	db	"SCI2"
	dw	_sci2_end-_sci2_start
_sci2_start:
ScrL2InitTablePtr:
	; min inx  : table pointer
	;--- Group 0
	db  0      :  dl !NULL
	;--- Group 1
	db  0      :  dl !NULL
	;--- Group 2
	db  0      :  dl !NULL
	;--- Group 3
	db  0      :  dl !NULL
_sci2_end:


;-------------------------------------------------
; GIEPY Layer2 scroller main function ptr ptr
;-------------------------------------------------
	db	"SCM2"
	dw	_scm2_end-_scm2_start
_scm2_start:
ScrL2MainTablePtr:
	; min inx  : table pointer
	;--- Group 0
	db  0      :  dl !NULL
	;--- Group 1
	db  0      :  dl !NULL
	;--- Group 2
	db  0      :  dl !NULL
	;--- Group 3
	db  0      :  dl !NULL
_scm2_end:


;-------------------------------------------------
; GIEPY location info
;-------------------------------------------------
	db	"LOC "
	dw	_loc_end-_loc_start
_loc_start:
	;--- Re-location info.
	;    GIEPY rewites the data indicated by this data.
	dl	ClusterReloc+1		; Cluster sprite routine address
	dl	ExtraReloc+1		; Extended sprite routine address

if !PIXI_COMPATIBLE
	dl	ExecutePtrLongPb
else
	dl	0
endif

	;--- Nop routines
	dl	SomethingNop
	dl	SpriteNop
	dl	ScrollL1Nop
	dl	ScrollL2Nop

	;--- $02a9ca backup(this is used for uninstall.)
	dl	!Val_02a9ca

	;--- this area is reserved for GIEPY.
	;    (uses for save table groups location)
	rep 5 : dl !NULL
_loc_end:

;-------------------------------------------------
; end of data
;-------------------------------------------------
	db	"TERM"

print "Insert size............. ", freespaceuse, " bytes."
print "Total bytes modified.... ", bytes, " bytes."

