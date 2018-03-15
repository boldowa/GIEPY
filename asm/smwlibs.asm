;-------------------------------------------------
; smw libraries declaration
;-------------------------------------------------
org $008000
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

org $05b329
.GetCoins

org $03b664
.GetMarioClipping

org $03b69f
.GetSpriteClippingA

org $03b6e5
.GetSpriteClippingB

org $03b72b
.CheckContact

org $07f7d2
.InitSpriteTables

org $01801a
.UpdateSpriteYPosNoGravity

org $018022
.UpdateSpriteXPosNoGravity

org $01802A
.UpdateSpritePosWithGravity

org $018032
.SpriteSpriteInteraction

org $01a7dc
.SpriteMarioInteraction

org $01803a
.SpriteInteraction

org $019138
.SpriteObjectInteraction

org $01b7b3
.SpriteFinishOAMWrite

; JSR libraries -------------
;
;  troublesome...

; Data libraries ------------
org $0da8a6
.IndexToBit

org $07f7db
.CircleCoords

