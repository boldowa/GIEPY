;-------------------------------------------------
; smw libraries declaration
;-------------------------------------------------
org $000000
SMW:
; JSL libraries -------------
org $0086df
.ExecutePtr

org $0086fa
.ExecutePtrLong

org $00f5b7
.HurtMario

org $00f606
.KillMario

org $05b160
.SubSideExit

org $0286bf
.YoshiStomp

org $0294c1
.GroundPound

org $00beb0
.GenerateTile

org $01acf9
.GetRand

org $02ace5
.GivePoints

org $03a6c8
.KillMostSprites

org $01aa33
.BoostMarioSpeed

; JSR libraries -------------
;
;  troublesome...

; Data libraries ------------
org $0da8a6
.IndexToBit

