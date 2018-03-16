;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; libAkagi.asm
;; ------------
;;
;;   Original source auther : Akaginite
;;   Modification for GIEPY : 6646
;;
;;   * Please don't send questions to Akaginite about this library.
;;     I don't want to trouble her.
;;
;;
;;   This file includes these routines.
;;
;;   - GetDrawInfo
;;         ... Get sprite's OAM Information subroutine.
;;             Be sure to call this function in the order of "JSR foo -> JSL GetDrawInfo".
;;
;;             e.g.
;;             -----------------------
;;                 JSR SUB_GFX
;;                      ;
;;                 RTL
;;
;;             SUB_GFX:
;;                 JSL GetDrawIndo
;;                      ;
;;                 RTS
;;             -----------------------
;;     [args]
;;         X  : Sprite index
;;     [return]
;;         Y  : OAM index
;;
;;   - SubOffScreen
;;         ... Kill sprite when sprite is off screen.
;;     [args]
;;         X  : Sprite index
;;         Y  : (type << 1)  (e.g. X1=$02, X2=$04 ... X7=$0E)
;;     [return]
;;         Carry Flag
;;           - ON  : Sprite is Off-Screen (Sprite is killed)
;;           - OFF : Sprite is On-Screen
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; GetDrawInfo - Optimized version
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

GetDrawInfo:	LDA   !sprite_x_high,x
		XBA
		LDA   !sprite_x_low,x
		REP   #$20
		SEC
		SBC   $1A			; Layer1 X pos
		STA   $00
		CLC
		ADC.w #$0040
		CMP.w #$0180
		SEP   #$20
		LDA   $01
		BEQ   +
		LDA   #$01
+		STA   !sprite_off_screen_horz,x
		TDC
		ROL   A
		STA   !sprite_off_screen,x
		BNE   .Invalid

		LDA   !sprite_y_high,x
		XBA
		LDA   !sprite_tweaker_1662,x
		AND   #$20			; bit5: sprite clipping 16-31
		BEQ   .CheckOnce
.CheckTwice	LDA   !sprite_y_low,x
		REP   #$21
		ADC.w #$001C
		SEC
		SBC   $1C			; Layer1 Y pos
		SEP   #$20
		LDA   !sprite_y_high,x
		XBA
		BEQ   .CheckOnce
		LDA   #$02
.CheckOnce	STA   !sprite_off_screen_vert,x
		LDA   !sprite_y_low,x
		REP   #$21
		ADC.w #$000C
		SEC
		SBC   $1C			; Layer1 Y pos
		SEP   #$21
		SBC   #$0C
		STA   $01
		XBA
		BEQ   .OnScreenY
		INC   !sprite_off_screen_vert,x
.OnScreenY	LDY   !sprite_oam_index,x
		RTL

.Invalid	REP   #$20	;
		PLY		;\  discard (GetDrawInfo's) JSL return address
		PLA		;/
		PLA		;   get 16bits(JSR) return address
		PHB		;\  set 24bits address
		PHA		;/
		SEP   #$20	;
		RTL


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; SubOffScreenA - Optimized version
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Argument
;   A : type (#$00=X0, #$01=X1 ... #$07=X7)
; Return
;   Carry flag :
;     - ON  : SpriteIsOffScreen (Sprite is killed)
;     - OFF : SpriteIsOnScreen
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
SubOffScreenA:
		AND.b	#7
		ASL
		TAY
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; SubOffScreen - Optimized version
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Argument
;   Y : type (#$00=X0, #$02=X1 ... #$0E=X7)
; Return
;   Carry flag :
;     - ON  : SpriteIsOffScreen (Sprite is killed)
;     - OFF : SpriteIsOnScreen
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
SubOffScreen:	CLC
		LDA   !sprite_off_screen_horz,x
		ORA   !sprite_off_screen_vert,x
		BEQ   .Return2

		LDA   !sprite_tweaker_167a,x
		AND   #$04			; bit 2 : Process when off screen
		BNE   .Return2

		PHB
		PHK
		PLB

		LDA   $5B			; screen mode
		LSR   A				; get bit0 : Vertical Layer 1 bit
		BCS   .VerticalLevel

		LDA   !sprite_y_low,x
		ADC   #$50
		LDA   !sprite_y_high,x
		ADC   #$00
		CMP   #$02
		BCS   .EraseSprite
		LDA   !sprite_x_high,x
		XBA
		LDA   !sprite_x_low,x
		REP   #$21
		ADC   .AddTable,y
		SEC
		SBC   $1A			; Layer1 X pos
		CMP   .CmpTable,y
.Common		SEP   #$20
		BCC   .RestoreDB
.EraseSprite	LDA   !sprite_status,x
		CMP   #$08			; #8 : Main routine executing
		BCC   .KillSprite
		LDY   !sprite_index_in_level,x
		CPY   #$FF
		BEQ   .KillSprite
if !sa1
		PHX
		TYX
		LDA   !sprite_load_table,x
		AND   #$FE
		STA   !sprite_load_table,x
		PLX
else
		LDA   !sprite_load_table,y
		AND   #$FE
		STA   !sprite_load_table,y
endif
.KillSprite	STZ   !sprite_status,x
		SEC

.RestoreDB	PLB
.Return2	RTL


.VerticalLevel	LDA   $13			; Frame counter
		LSR   A				; bit 0
		BCS   .CheckY
		LDA   !sprite_x_high,x
		XBA
		LDA   !sprite_x_low,x
		REP   #$21
		ADC.w #$0040
		CMP.w #$0280
		BRA   .Common
.CheckY		LDA   !sprite_y_high,x
		XBA
		LDA   !sprite_y_low,x
		REP   #$20
		SBC   $1C			; Layer1 Y pos
		CLC
		ADC.w #$0070
		CMP.w #$01D0
		BRA   .Common

.AddTable	dw $0040,$0040,$0010,$0070
		dw $0090,$0050,$0080,$FFC0
.CmpTable	dw $0170,$01E0,$01B0,$01D0
		dw $0230,$01B0,$0220,$0160
