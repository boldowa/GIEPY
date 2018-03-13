@include
;-------------------------------------------------------------------------------
; Macros for GIEPY
;-------------------------------------------------------------------------------
!__macros_inc ?= 0
if 0 == !__macros_inc
!__macros_inc := 1


;-------------------------------------------------
; Debug print
;   args:
;     - msg		... debug message
;-------------------------------------------------
macro putdebug(msg)
	if 0 != !DEBUG
		print "[Debug] $", pc, " ... <msg>"
	endif
endmacro


;-------------------------------------------------
; Define sprite table
;   args:
;     - name		... define's name
;     - lorom_adr	... LoRom address
;     - sa1_adr		... SA-1 address
;-------------------------------------------------
macro define_sprite_table(name, lorom_adr, sa1_adr)
	if 0 == !sa1
		!<name> := $<lorom_adr>
		!<lorom_adr> := $<lorom_adr>
	else
		!<name> := $<sa1_adr>
		!<lorom_adr> := $<sa1_adr>
	endif
endmacro


;-------------------------------------------------
; Define base2 address
;   args:
;     - name		... define's name
;     - absaddr		... absolute mode address
;-------------------------------------------------
macro define_base2_address(name, absaddr)
	if 0 == !sa1
		!<name> := $<absaddr>
		!<absaddr> := $<absaddr>
	else
		!<name> := $<absaddr>|!base2
		!<absaddr> := $<absaddr>|!base2
	endif
endmacro


;-------------------------------------------------
; ORG with ROM assertion (4 bytes)
; ***** This macro doesn't work collectly.
;       So, please use org_assert_long2 or
;       org_assert_long4.
;   args:
;     - addr		... origin address
;     - data            ... assertion data
;-------------------------------------------------
macro org_assert_long(addr, data)
	print "[Warning] This macro doesn't work collectly..."
	if 0 == !DEBUG
		assert ($<data> & $ffff) == read2(<addr>)
		assert ($<data> >> 16)   == read2(<addr>+2)
	endif
	org <addr>
endmacro


;-------------------------------------------------
; ORG with ROM assertion 2 (4 bytes)
;   args:
;     - addr		... origin address
;     - data1           ... assertion data high 16bits
;     - data2           ... assertion data low  16bits
;-------------------------------------------------
macro org_assert_long2(addr, data1, data2)
	if 0 == !DEBUG
		assert $<data2> == read2(<addr>)
		assert $<data1> == read2(<addr>+2)
	endif
	org <addr>
endmacro


;-------------------------------------------------
; ORG with ROM assertion 4 (4 bytes)
;   args:
;     - addr		... origin address
;     - data1           ... assertion data 1
;     - data2           ... assertion data 2
;     - data3           ... assertion data 3
;     - data4           ... assertion data 4
;-------------------------------------------------
macro org_assert_long4(addr, data1, data2, data3, data4)
	if 0 == !DEBUG
		assert $<data1> == read1(<addr>)
		assert $<data2> == read1(<addr>+1)
		assert $<data3> == read1(<addr>+2)
		assert $<data4> == read1(<addr>+3)
	endif
	org <addr>
endmacro


;-------------------------------------------------
; ORG with ROM assertion (2 bytes)
;   args:
;     - addr		... origin address
;     - data            ... assertion data
;-------------------------------------------------
macro org_assert_word(addr, data)
	if 0 == !DEBUG
		assert $<data> == read2(<addr>)
	endif
	org <addr>
endmacro

;-------------------------------------------------
; ORG with ROM assertion (jsl)
;   args:
;     - addr		... origin address
;     - data            ... assertion data
;-------------------------------------------------
macro org_assert_jsl(addr, data)
	if 0 == !DEBUG
		assert $22 == read1(<addr>)
		if !sa1
			assert <data> == read3(<addr>+1)
		else
			assert (<data>&$7fffff) == (read3(<addr>+1)&$7fffff)
		endif
	endif
	org <addr>
endmacro

;-------------------------------------------------
; Extra bits shift on compatible mode
;-------------------------------------------------
macro ebshift_compatible()
	if !true == !ST_COMPATIBLE
		lsr #2
	endif
endmacro


;-------------------------------------------------
; Extra bits shift on not compatible mode
;-------------------------------------------------
macro ebshift_not_compatible()
	if !false == !ST_COMPATIBLE
		asl #2
	endif
endmacro


;-------------------------------------------------
; JUMP with ROM assertion (3 bytes)
;   args:
;     - code		... opcode (jml or jsl)
;     - addr		... target address
;     - data            ... assertion data
;-------------------------------------------------
macro jml_assert_long(code, addr, data)
	if 0 == !DEBUG
		assert $<data> == read3(<addr>)
	endif
	<code>	<addr>|!bankB
endmacro

;-------------------------------------------------
; GIEPY only sprite assertion code
;-------------------------------------------------
macro GiepyOnly()
	if !PIXI_COMPATIBLE
		print "Error: This sprite is for GIEPY only."
		assert 0
	endif
endmacro

;-------------------------------------------------
; PIXI sprite assertion code
;-------------------------------------------------
macro PixiSprite()
	if !false == !PIXI_COMPATIBLE
		print "Error: This sprite is for PIXI."
		print "       Please enable PIXI compatible mode."
		assert 0
	endif
endmacro


endif
