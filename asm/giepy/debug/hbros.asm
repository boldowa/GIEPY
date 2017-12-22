@include
;-------------------------------------------------
; Hammer bros code for debug
;-------------------------------------------------

CoinMain:
		inc	$0db6|!base2
		inc	$0dbf|!base2
		lda.b	$14
		and.b	#1
		sta.w	$1dfc|!base2
CoinInit:
		rtl


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Hammer Brother, by mikeyk
;;
;; Description: This guy walks back and forth, throwing hammers at Mario.
;;
;; Uses first extra bit: NO
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	!RAM_ThrowTimer = !1504
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; sprite data
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                    
                    !HAMMER_TILE = $6D
                    
TILEMAP:            db $46,$4A,$7F,$00
                    db $46,$5A,$2F,$00
                    db $48,$4A,$7F,$00
                    db $48,$5A,$2F,$00

HORZ_DISP:          db $00,$00,$08
                    db $00,$08,$00
VERT_DISP:          db $F8,$08,$08
TILE_SIZE:          db $02,$00,$00

PROPERTIES:         db $40,$00             ;xyppccct format

SPEED_TABLE:        db $08,$F8             ; speed of hammer bro, right, left

                    !TIME_TO_SHOW = $18      ;time to display the boomerang before it is thrown
                    !TIME_TILL_THROW = $28

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; init JSL
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;print "INIT ",pc
HBrosInit:
		%putdebug("HBrosInit")
		PHY
		JSR SUB_HORZ_POS
		TYA
		STA !157c,x
		PLY

		TXA
		AND #$01
		ASL A 
		ASL A 
		ASL A 
		ASL A 
		CLC
		ADC #$20
		STA !RAM_ThrowTimer,x
		RTL
                    
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; main sprite JSL
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;print "MAIN ",pc
HBrosMain:
		%putdebug("HBrosMain")
HAMMER_BRO_JSL:
		PHB                     ; \
		PHK                     ;  | main sprite function, just calls local subroutine
		PLB                     ;  |
		JSR DECREMENTTIMERS
		JSR START_HB_CODE       ;  |
		PLB                     ;  |
		RTL                     ; /

DECREMENTTIMERS:
		LDA !14c8,x
		CMP #$08
		BNE .done
		LDA $9D
		BNE .done
		LDA !RAM_ThrowTimer,x
		BEQ .done
		DEC !RAM_ThrowTimer,x
.done:
		RTS
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; main sprite routine
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

STAR:               INC !RAM_ThrowTimer,x
RETURN:             RTS                    
START_HB_CODE:      JSR SUB_GFX             ; draw hammer bro gfx
                    LDA !14c8,x             ; \ if hammer bro status != 8...
                    CMP #$02                ;  }   ... not (killed with spin jump [4] or STAR[2])
                    BEQ STAR
                    CMP #$08
                    BNE STAR              ; /    ... RETURN
                    LDA $9D                 ; \ if sprites locked...
                    BNE RETURN              ; /    ... RETURN

						  LDA #$00                ; sub_off_screen_x0
                    JSR SUB_OFF_SCREEN         ; only process hammer bro while on screen
                    INC !1570,x             ; increment number of frames hammer bro has been on screen
                    LDA !1570,x             ; \ calculate which frame to show:
                    LSR A                   ;  | 
                    LSR A                   ;  | 
                    LSR A                   ;  | 
                    AND #$01                ;  | update every 16 cycles if normal
LABEL3:             STA !1602,x             ; / write frame to show

                    LDA !1540,x
                    BEQ DISABLE_NOT_SET
                    CMP #$01
                    BNE DISABLE_NOT_SET
                    LDA !1686,x
                    AND #$7F
                    STA !1686,x

DISABLE_NOT_SET:

                    LDA !RAM_ThrowTimer,x             ; \ if time until throw < !TIME_TO_SHOW
                    CMP #!TIME_TO_SHOW       ;  |
                    BCS NO_THROW            ;  | 
                    INC !1602,x             ;  | change image (hammer will be displayed)
                    INC !1602,x             ; /

                    LDA !RAM_ThrowTimer,x             ; \ if time until throw = 0
                    BNE NO_TIME_SET         ;  |
                    LDA #!TIME_TILL_THROW    ;  | set the timer
                    STA !RAM_ThrowTimer,x             ; /
NO_TIME_SET:        CMP #$01                ; \ call the hammer routine if the timer is 
                    BNE NO_THROW            ;  | about to tun out
                    JSR SUB_HAMMER_THROW    ; /
NO_THROW:             

                    LDA !1588,x             ; \  if sprite is not on ground...
                    AND #$04                ;  }    ...(4 = on ground) ...
                    BEQ NO_JUMP             ; /     ...goto NO_JUMP
                    LDA #$10                ; \  y speed = 10
                    STA !aa,x               ; /

                    JSR SUB_HORZ_POS           ; \ always face mario
                    TYA                     ;  | 
                    STA !157c,x             ; /
                    
                    LDA !1570,x             ; \ makes hammer bro jump
                    CLC                     ;  |
                    ADC #$77                ;  |
                    AND #$E7                ;  |
                    BNE NO_JUMP             ;  |
                    
                    JSR SUB_GET_SPEED

                    LDA !1686,x
                    ORA #$80
                    STA !1686,x
                    
        

NO_JUMP:            LDA $14                 ; \ set x speed
                    AND #$3F                ;  |A:014C X:0009 Y:0001 D:0000 DB:03 S:01E9 P:envMXdizcHC:1328 VC:089 00 FL:19405
                    BNE LABEL4              ;  |A:0140 X:0009 Y:0001 D:0000 DB:03 S:01E9 P:envMXdizcHC:1344 VC:089 00 FL:19405
                    LDA !151c,x             ;  |
                    EOR #$01                ;  |
                    STA !151c,x             ;  |
LABEL4:             LDA !151c,x             ;  |
                    AND #$01                ;  |
                    TAY                     ;  |
                    LDA SPEED_TABLE,y       ;  |
                    STA $B6,x               ; / A:01F9 X:0009 Y:0001 D:0000 DB:03 S:01E9 P:eNvMXdizcHC:0014 VC:090 00 FL:19405
                    
                    JSL $01802A             ; update position based on speed values
                    
                    LDA !1588,x             ; \ if hammer bro is touching the side of an object...
                    AND #$03                ;  |
                    BEQ DONT_CHANGE_DIR     ;  |
                    LDA !151c,x             ;  |
                    EOR #$01                ;  |    ... change hammer bro direction
                    STA !151c,x             ; /

DONT_CHANGE_DIR:    JSL $018032             ; interact with other sprites               
                    JSL $01A7DC             ; check for mario/hammer bro contact

NO_CONTACT:         RTS                     ; RETURN



JUMP_HEIGHT:        db $B0,$D0
DISABLE_TIME:       db $24,$28

SUB_GET_SPEED:      LDA !14d4,x
                    XBA
                    LDA !d8,x
                    PHP
                    REP #$30                    
                    CMP.w #$0138
                    BCS JUMP_UP
                    CMP.w #$00F8
                    BCC JUMP_DOWN
                    PLP
                    JSL $01ACF9
                    AND #$01
                    
                    PHY
                    TAY
SET_HEIGHT:         LDA DISABLE_TIME,y 
                    STA !1540,x
                    LDA JUMP_HEIGHT,y 
                    STA !aa,x                                   
                    PLY
                    RTS
                    
JUMP_UP:            PLP
                    PHY
                    LDY #$00
                    BRA SET_HEIGHT

JUMP_DOWN:          PLP
                    PHY
                    LDY #$01
                    BRA SET_HEIGHT                  

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; hammer routine
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

X_OFFSET:            db $F6,$0A
X_SPEED:	            db $12,$EE
	
                    !Y_SPEED = $C8
						  
SUB_HAMMER_THROW:						  						  
						  LDY !157c,x
						  LDA X_OFFSET,y
						  STA $00
						  LDA #$F7
						  STA $01
						  LDA X_SPEED,y
						  STA $02
						  LDA #!Y_SPEED
						  STA $03
						  
						  LDA #$04  					; extended sprite = hammer
						  JSR SPAWN_EXTENDED
						  RTS


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; graphics routine
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SUB_GFX:            JSR GET_DRAW_INFO       ; after: Y = index to sprite tile map ($300)
                                            ;      $00 = sprite x position relative to screen boarder 
                                            ;      $01 = sprite y position relative to screen boarder  
                    LDA !1602,x             ; \
                    ASL A
                    ASL A                   ;  | $03 = index to frame start (frame to show * 2 tile per frame)
                    STA $03                 ; /
                    LDA !157c,x             ; \ $02 = sprite direction
                    STA $02                 ; /
                    PHX                     ; push sprite index

                    LDX #$02                ; loop counter = (number of tiles per frame) - 1
LOOP_START:         PHX                     ; push current tile number
                   
                    PHX
                    TXA
                    LDX $02
                    BNE NO_ADJ
                    CLC
                    ADC #$03
NO_ADJ:             TAX                    
                    LDA $00                 ; \ tile x position = sprite x location ($00)
                    CLC
                    ADC HORZ_DISP,x
                    STA $0300|!base2,y      ; /                    
                    PLX
                                        
                    LDA $01                 ; \ tile y position = sprite y location ($01) + tile displacement
                    CLC                     ;  |
                    ADC VERT_DISP,x         ;  |
                    STA $0301|!base2,y      ; /
                    
                    LDA TILE_SIZE,x
                    PHX
                    PHA
                    TYA                     ; \ get index to sprite property map ($460)...
                    LSR A                   ; |    ...we use the sprite OAM index...
                    LSR A                   ; |    ...and divide by 4 because a 16x16 tile is 4 8x8 tiles
                    TAX                     ; | 
                    PLA
                    STA $0460|!base2,x      ; /  
                    PLX                  

                    TXA                     ; \ X = index to horizontal displacement
                    ORA $03                 ; / get index of tile (index to first tile of frame + current tile number)
                    TAX                     ; \ 
                                 
                    LDA TILEMAP,x           ; \ store tile
                    STA $0302|!base2,y      ; / 
        
                    LDX $02                 ; \
                    LDA PROPERTIES,x        ;  | get tile PROPERTIES using sprite direction
                    LDX $15E9|!base2        ;  |
                    ORA !15f6,x             ;  | get palette info
                    ORA $64                 ;  | ?? what is in 64, level PROPERTIES... disable layer priority??
                    STA $0303|!base2,y      ; / store tile PROPERTIES
                    
                    PLX                     ; \ pull, X = current tile of the frame we're drawing
                    INY                     ;  | increase index to sprite tile map ($300)...
                    INY                     ;  |    ...we wrote 1 16x16 tile...
                    INY                     ;  |    ...sprite OAM is 8x8...
                    INY                     ;  |    ...so increment 4 times
                    DEX                     ;  | go to next tile of frame and loop
                    BPL LOOP_START          ; / 

                    PLX                     ; pull, X = sprite index

                    LDA !RAM_ThrowTimer,x
                    CMP #$02
                    BCC NO_SHOW_HAMMER
                    CMP #30
                    BCS NO_SHOW_HAMMER
                    LDA !1602,x
                    CMP #$02
                    BCS SHOW_HAMMER_TOO
                    
NO_SHOW_HAMMER:     LDY #$FF                ; \ 02, because we didn't write to 460 yet
                    LDA #$02                ;  | A = number of tiles drawn - 1
                    JSL $01B7B3             ; / don't draw if offscreen
                    RTS                     ; RETURN

HAMMER_OFFSET:      db $F6,$0A

SHOW_HAMMER_TOO:    PHX
                    
                    LDA $00
                    LDX $02
                    CLC
                    ADC HAMMER_OFFSET,x
                    STA $0300|!base2,y
                    
                    LDA $01                 ; \ tile y position = sprite y location ($01) + tile displacement
                    CLC                     ;  |
                    ADC #$F2
                    STA $0301|!base2,y      ; /
                    
                    LDA #!HAMMER_TILE       ; \ store tile
                    STA $0302|!base2,y      ; / 

                    PHX
                    TYA                     ; \ get index to sprite property map ($460)...
                    LSR A                   ; |    ...we use the sprite OAM index...
                    LSR A                   ; |    ...and divide by 4 because a 16x16 tile is 4 8x8 tiles
                    TAX                     ; | 
                    LDA #$02
                    STA $0460|!base2,x      ; /  
                    PLX     

                    LDA #$07 
                    CPX #$00
                    BNE NO_FLIP_HAMMER     
                    ORA #$40
NO_FLIP_HAMMER:     ORA $64                 ;  | put in level PROPERTIES
                    STA $0303|!base2,y      ; / store tile PROPERTIES
                    
                    PLX
                    INY                     ;  | increase index to sprite tile map ($300)...
                    INY                     ;  |    ...we wrote 1 16x16 tile...
                    INY                     ;  |    ...sprite OAM is 8x8...
                    INY                     ;  |    ...so increment 4 times

                    LDY #$FF                ; \ 02, because we didn't write to 460 yet
                    LDA #$03                ;  | A = number of tiles drawn - 1
                    JSL $01B7B3             ; / don't draw if offscreen
                    RTS                     ; RETURN





;-------------------------------------------------
; Libraries
;-------------------------------------------------
;GetDrawInfo
GET_DRAW_INFO:
   LDA !14e0,x
   XBA
   LDA !e4,x
   REP #$20
   SEC : SBC $1A
   STA $00
   CLC
   ADC.w #$0040
   CMP.w #$0180
   SEP #$20
   LDA $01
   BEQ +
     LDA #$01
   +   STA !15a0,x
   TDC
   ROL A
   STA !15c4,x
   BNE .Invalid

   LDA !14d4,x
   XBA
   LDA !1662,x
   AND #$20
   BEQ .CheckOnce
.CheckTwice
   LDA !d8,x
   REP #$21
   ADC.w #$001C
   SEC : SBC $1C
   SEP #$20
   LDA !14d4,x
   XBA
   BEQ .CheckOnce
   LDA #$02
.CheckOnce
   STA !186c,x
   LDA !d8,x
   REP #$21
   ADC.w #$000C
   SEC : SBC $1C
   SEP #$21
   SBC #$0C
   STA $01
   XBA
   BEQ .OnScreenY
   INC !186c,x
.OnScreenY
   LDY !15ea,x
   RTS
 
.Invalid
   PLA             ; destroy the JSL
   PLA
   PLA
   PLA             ; sneak in the bank
   PLY
   PHB
   PHY
   PHA
   RTS


;SubHorzPos
SUB_HORZ_POS:
	LDY #$00
	LDA $94
	SEC
	SBC !e4,x
	STA $0E
	LDA $95
	SBC !14e0,x
	STA $0F
	BPL LABEL16
	INY
LABEL16:
	RTS

;SubOffScreen
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; SubOffScreen - Optimized version
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Argument
; A: type (#$00=X0, #$01=X1 ... #$07=X7)
; Only last 3 bits are used
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SUB_OFF_SCREEN:
 
    AND #$07
    ASL
 
    TAY
    LDA !15a0,x
    ORA !186c,x
    BEQ .Return2
    LDA !167a,x
    AND #$04
    BNE .Return2
    LDA $5B
    LSR A
    BCS .VerticalLevel
    LDA !d8,x
    ADC #$50
    LDA !14d4,x
    ADC #$00
    CMP #$02
    BCS .EraseSprite
    LDA !14e0,x
    XBA
    LDA !e4,x
    REP #$21
    ADC .AddTable,y
    SEC : SBC $1A
    CMP .CmpTable,y
.Common
    SEP #$20
    BCC .Return2
.EraseSprite
    LDA !14c8,x
    CMP #$08
    BCC .KillSprite
    LDY !161a,x
    CPY #$FF
    BEQ .KillSprite
    if !sa1 == 0
        LDA $1938,y
        AND #$FE
        STA $1938,y
    else
        PHX : TYX
        LDA $418A00,x
        AND #$FE
        STA $418A00,x
        PLX
    endif
.KillSprite
    STZ !14c8,x
.Return2
    RTS
 
.VerticalLevel
    LDA $13
    LSR A
    BCS .CheckY
    LDA !14e0,x
    XBA
    LDA !e4,x
    REP #$21
    ADC.w #$0040
    CMP.w #$0280
    BRA .Common
.CheckY
    LDA !14d4,x
    XBA
    LDA !d8,x
    REP #$20
    SBC $1C
    CLC
    ADC.w #$0070
    CMP.w #$01D0
    BRA .Common
 
.AddTable
    dw $0040,$0040,$0010,$0070
    dw $0090,$0050,$0080,$FFC0
.CmpTable
    dw $0170,$01E0,$01B0,$01D0
    dw $0230,$01B0,$0220,$0160

; Spwn Extended
SPAWN_EXTENDED:
;Routine that spawns an extended sprite with initial speed at the position (+offset)
;of the calling sprite and returns the sprite index in Y
;For a list of extended sprites see here: http://www.smwiki.net/wiki/RAM_Address/$7E:170B

;Input:  A   = number
;        $00 = x offset  \
;        $01 = y offset  | you could also just ignore these and set them later
;        $02 = x speed   | using the returned Y.
;        $03 = y speed   /
;
;Output: Y   = index to extended sprite (#$FF means no sprite spawned)
;        C   = Carry Set = spawn failed, Carry Clear = spawn successful.

	XBA						; Save A
	LDA !15a0,x				; \
	ORA !186c,x				; | return if offscreen or about to be eaten
	ORA !15d0,x				; |
	BNE .ret					; /
	
; Get Index
	LDY #$07					; loop over 8 extra sprite slots (last 2 are for fireballs)
.loop
	LDA $170B|!base2,y	; \ if empty, proceed
	BEQ +						; /
	DEY						; \
	BPL .loop				; | if not, decrease Y and continue with loop
	SEC						; | set carry if none is spawned
	RTS						; /
+
	XBA						; get number back in A
	STA $170B|!base2,y 	;
		
		
	LDA $00					; \
	CLC : ADC !e4,x		; |
	STA $171F|!base2,y	; | store x position + x offset (low byte)
	LDA #$00					; |
	BIT $00					; | create high byte based on $00 in A and add
	BPL +						; | to x position
	DEC						; |
+	ADC !14e0,x				; |
	STA $1733|!base2,y	; /
		
	LDA $01					; \ 
 	CLC : ADC !d8,x		; |
	STA $1715|!base2,y	; | store y position + y offset	
	LDA #$00					; |
	BIT $01					; | create high byte based on $01 in A and add
	BPL +						; | to y position
	DEC						; |
+	ADC !14d4,x				; |
	STA $1729|!base2,y	; /
	
	LDA $02					; \ store x speed
	STA $1747|!base2,y	; /
	LDA $03					; \ store y speed
	STA $173D|!base2,y	; /
	
.ret
	CLC
	RTS
	
