;-------------------------------------------------
; TRASM Address conversion include
;   (LoROM sides)
;-------------------------------------------------
Base1			= $0000
Base2			= $0000
BankA			= $7E0000
BankB			= $800000
CustomBit		= $80
SprSize			= 12

;---------------------------------------
;--- New memory address
;---------------------------------------
extra_bits		= $7FAB10
S7FAB10			= $7FAB10
new_code_flag		= $7FAB1C
S7FAB1C			= $7FAB1C
extra_prop_1		= $7FAB28
S7FAB28			= $7FAB28
extra_prop_2		= $7FAB34
S7FAB34			= $7FAB34
new_sprite_num		= $7FAB9E
S7FAB9E			= $7FAB9E
extra_bit_sh		= $7FABAA
S7FABAA			= $7FABAA
extra_bits_gen		= $7FABB2
S7FABB2			= $7FABB2
extra_bits_l1scr	= $7FABB3
S7FABB3			= $7FABB3
extra_bits_l2scr	= $7FABB4
S7FABB4			= $7FABB4
extra_byte_1		= $7FAB40
extra_byte_2		= $7FAB4C
extra_byte_3		= $7FAB58
extra_byte_4		= $7FAB64
extra_byte_sh		= $7FAB70
extra_byte_gen		= $7FAB78
extra_byte_scr		= $7FAB79


;---------------------------------------
;--- Shooter memory address
;---------------------------------------
shoot_num		= $1783
S1783			= $1783
shoot_y_low		= $178B
S178B			= $178B
shoot_y_high		= $1793
S1793			= $1793
shoot_x_low		= $179B
S179B			= $179B
shoot_x_high		= $17A3
S17A3			= $17A3
shoot_timer		= $17AB
S17AB			= $17AB

;---------------------------------------
;--- Generator memory address
;---------------------------------------
gen_num			= $18B9
S18B9			= $18B9

;---------------------------------------
;--- Scroller memory address
;---------------------------------------
scroll_num_l1		= $143E
S143E			= $143E
scroll_num_l2		= $143F
S143F			= $143F
scroll_layer_bit	= $1456
S1456			= $1456

;---------------------------------------
;--- Cluster sprite memory address
;---------------------------------------
cluster_num		= $1892
S1892			= $1892
cluster_y_low		= $1E02
S1E02			= $1E02
cluster_y_high		= $1E2A
S1E2A			= $1E2A
cluster_x_low		= $1E16
S1E16			= $1E16
cluster_x_high		= $1E3E
S1E3E			= $1E3E

;---------------------------------------
;--- Extended sprite memory address
;---------------------------------------
extended_num		= $170B
S170B			= $170B
extended_y_low		= $1715
S1715			= $1715
extended_y_high		= $1729
S1729			= $1729
extended_x_low		= $171F
S171F			= $171F
extended_x_high		= $1733
S1733			= $1733
extended_y_speed	= $173D
S173D			= $173D
extended_x_speed	= $1747
S1747			= $1747
extended_table		= $1765
S1765			= $1765
extended_timer		= $176F
S176F			= $176F
extended_behind		= $1779
S1779			= $1779

;---------------------------------------
;--- Regular sprite memory address
;---------------------------------------
sprite_num			= $9E
S9E				= $9E
sprite_speed_y			= $AA
SAA				= $AA
sprite_speed_x			= $B6
SB6				= $B6
sprite_misc_c2			= $C2
SC2				= $C2
sprite_y_low			= $D8
SD8				= $D8
sprite_x_low			= $E4
SE4				= $E4
sprite_status			= $14C8
S14C8				= $14C8
sprite_y_high			= $14D4
S14D4				= $14D4
sprite_x_high			= $14E0
S14E0				= $14E0
sprite_speed_y_frac		= $14EC
S14EC				= $14EC
sprite_speed_x_frac		= $14F8
S14F8				= $14F8
sprite_misc_1504		= $1504
S1504				= $1504
sprite_misc_1510		= $1510
S1510				= $1510
sprite_misc_151c		= $151C
S151C				= $151C
sprite_misc_1528		= $1528
S1528				= $1528
sprite_misc_1534		= $1534
S1534				= $1534
sprite_misc_1540		= $1540
S1540				= $1540
sprite_misc_154c		= $154C
S154C				= $154C
sprite_misc_1558		= $1558
S1558				= $1558
sprite_misc_1564		= $1564
S1564				= $1564
sprite_misc_1570		= $1570
S1570				= $1570
sprite_misc_157c		= $157C
S157C				= $157C
sprite_blocked_status		= $1588
S1588				= $1588
sprite_misc_1594		= $1594
S1594				= $1594
sprite_off_screen_horz		= $15A0
S15A0				= $15A0
sprite_misc_15ac		= $15AC
S15AC				= $15AC
sprite_slope			= $15B8
S15B8				= $15B8
sprite_off_screen		= $15C4
S15C4				= $15C4
sprite_being_eaten		= $15D0
S15D0				= $15D0
sprite_obj_interact		= $15DC
S15DC				= $15DC
sprite_oam_index		= $15EA
S15EA				= $15EA
sprite_oam_properties		= $15F6
S15F6				= $15F6
sprite_misc_1602		= $1602
S1602				= $1602
sprite_misc_160e		= $160E
S160E				= $160E
sprite_index_in_level		= $161A
S161A				= $161A
sprite_misc_1626		= $1626
S1626				= $1626
sprite_behind_scenery		= $1632
S1632				= $1632
sprite_misc_163e		= $163E
S163E				= $163E
sprite_in_water			= $164A
S164A				= $164A
sprite_tweaker_1656		= $1656
S1656				= $1656
sprite_tweaker_1662		= $1662
S1662				= $1662
sprite_tweaker_166e		= $166E
S166E				= $166E
sprite_tweaker_167a		= $167A
S167A				= $167A
sprite_tweaker_1686		= $1686
S1686				= $1686
sprite_off_screen_vert		= $186C
S186C				= $186C
sprite_misc_187b		= $187B
S187B				= $187B
sprite_load_table		= $1938
S1938				= $1938
sprite_tweaker_190f		= $190F
S190F				= $190F
sprite_misc_1fd6		= $1FD6
S1FD6				= $1FD6
sprite_cape_disable_time	= $1FE2
S1FE2				= $1FE2

