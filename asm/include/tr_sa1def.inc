;-------------------------------------------------
; TRASM Address conversion include
;   (SA-1 sides)
;-------------------------------------------------
Base1			= $3000
Base2			= $6000
BankA			= $400000
BankB			= $000000
CustomBit		= $80
SprSize			= 22

;---------------------------------------
;--- New memory address
;---------------------------------------
extra_bits		= $400040
S7FAB10			= $400040
new_code_flag		= $400056
S7FAB1C			= $400056
extra_prop_1		= $400057
S7FAB28			= $400057
extra_prop_2		= $40006D
S7FAB34			= $40006D
new_sprite_num		= $400083
S7FAB9E			= $400083
extra_bit_sh		= $400099
S7FABAA			= $400099
extra_bits_gen		= $4000A1
S7FABB2			= $4000A1
extra_bits_l1scr	= $4000A2
S7FABB3			= $4000A2
extra_bits_l2scr	= $4000A3
S7FABB4			= $4000A3
extra_byte_1		= $4000A4
extra_byte_2		= $4000BA
extra_byte_3		= $4000D0
extra_byte_4		= $4000E6
extra_byte_sh		= $41C800
extra_byte_gen		= $4000FC
extra_byte_scr		= $4000FD


;---------------------------------------
;--- Shooter memory address
;---------------------------------------
shoot_num		= $7783
S1783			= $7783
shoot_y_low		= $778B
S178B			= $778B
shoot_y_high		= $7793
S1793			= $7793
shoot_x_low		= $779B
S179B			= $779B
shoot_x_high		= $77A3
S17A3			= $77A3
shoot_timer		= $77AB
S17AB			= $77AB

;---------------------------------------
;--- Generator memory address
;---------------------------------------
gen_num			= $78B9
S18B9			= $78B9

;---------------------------------------
;--- Scroller memory address
;---------------------------------------
scroll_num_l1		= $743E
S143E			= $743E
scroll_num_l2		= $743F
S143F			= $743F
scroll_layer_bit	= $7456
S1456			= $7456

;---------------------------------------
;--- Cluster sprite memory address
;---------------------------------------
cluster_num		= $7892
S1892			= $7892
cluster_y_low		= $7E02
S1E02			= $7E02
cluster_y_high		= $7E2A
S1E2A			= $7E2A
cluster_x_low		= $7E16
S1E16			= $7E16
cluster_x_high		= $7E3E
S1E3E			= $7E3E

;---------------------------------------
;--- Extended sprite memory address
;---------------------------------------
extended_num		= $770B
S170B			= $770B
extended_y_low		= $7715
S1715			= $7715
extended_y_high		= $7729
S1729			= $7729
extended_x_low		= $771F
S171F			= $771F
extended_x_high		= $7733
S1733			= $7733
extended_y_speed	= $773D
S173D			= $773D
extended_x_speed	= $7747
S1747			= $7747
extended_table		= $7765
S1765			= $7765
extended_timer		= $776F
S176F			= $776F
extended_behind		= $7779
S1779			= $7779

;---------------------------------------
;--- Regular sprite memory address
;---------------------------------------
sprite_num			= $3200
S9E				= $3200
sprite_speed_y			= $9E
SAA				= $9E
sprite_speed_x			= $B6
SB6				= $B6
sprite_misc_c2			= $D8
SC2				= $D8
sprite_y_low			= $3216
SD8				= $3216
sprite_x_low			= $322C
SE4				= $322C
sprite_status			= $3242
S14C8				= $3242
sprite_y_high			= $3258
S14D4				= $3258
sprite_x_high			= $326E
S14E0				= $326E
sprite_speed_y_frac		= $74C8
S14EC				= $74C8
sprite_speed_x_frac		= $74DE
S14F8				= $74DE
sprite_misc_1504		= $74F4
S1504				= $74F4
sprite_misc_1510		= $750A
S1510				= $750A
sprite_misc_151c		= $3284
S151C				= $3284
sprite_misc_1528		= $329A
S1528				= $329A
sprite_misc_1534		= $32B0
S1534				= $32B0
sprite_misc_1540		= $32C6
S1540				= $32C6
sprite_misc_154c		= $32DC
S154C				= $32DC
sprite_misc_1558		= $32F2
S1558				= $32F2
sprite_misc_1564		= $3308
S1564				= $3308
sprite_misc_1570		= $331E
S1570				= $331E
sprite_misc_157c		= $3334
S157C				= $3334
sprite_blocked_status		= $334A
S1588				= $334A
sprite_misc_1594		= $3360
S1594				= $3360
sprite_off_screen_horz		= $3376
S15A0				= $3376
sprite_misc_15ac		= $338C
S15AC				= $338C
sprite_slope			= $7520
S15B8				= $7520
sprite_off_screen		= $7536
S15C4				= $7536
sprite_being_eaten		= $754C
S15D0				= $754C
sprite_obj_interact		= $7562
S15DC				= $7562
sprite_oam_index		= $33A2
S15EA				= $33A2
sprite_oam_properties		= $33B8
S15F6				= $33B8
sprite_misc_1602		= $33CE
S1602				= $33CE
sprite_misc_160e		= $33E4
S160E				= $33E4
sprite_index_in_level		= $7578
S161A				= $7578
sprite_misc_1626		= $758E
S1626				= $758E
sprite_behind_scenery		= $75A4
S1632				= $75A4
sprite_misc_163e		= $33FA
S163E				= $33FA
sprite_in_water			= $75BA
S164A				= $75BA
sprite_tweaker_1656		= $75D0
S1656				= $75D0
sprite_tweaker_1662		= $75EA
S1662				= $75EA
sprite_tweaker_166e		= $7600
S166E				= $7600
sprite_tweaker_167a		= $7616
S167A				= $7616
sprite_tweaker_1686		= $762C
S1686				= $762C
sprite_off_screen_vert		= $7642
S186C				= $7642
sprite_misc_187b		= $3410
S187B				= $3410
sprite_load_table		= $418A00
S1938				= $418A00
sprite_tweaker_190f		= $7658
S190F				= $7658
sprite_misc_1fd6		= $766E
S1FD6				= $766E
sprite_cape_disable_time	= $7FD6
S1FE2				= $7FD6


