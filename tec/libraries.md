---
layout: page
title: ASM libraries
description: 
---

# Glossary

|Name          |Description                                                                                                                  |
|:-------------|-----------------------------------------------------------------------------------------------------------------------------|
|Accumlator    |An alias for the A register.                                                                                                 |
|Index Register|Alias for X and Y register.                                                                                                  |
|AL            |Lower 8 bits of A register.                                                                                                  |
|AH            |Higher 8 bits of A register. You can't access this value unless you use 16 bits mode or swap the value with XBA.             |
|XL            |Lower 8 bits of X register.                                                                                                  |
|XH            |Higher 8 bits of X register. You can't access this value unless you use 16 bits mode or swap the value with TAX, XBA and TXA.|
|YL            |Lower 8 bits of Y register.                                                                                                  |
|YH            |Higher 8 bits of Y register. You can't access this value unless you use 16 bits mode or swap the value with TAY, XBA and TYA.|


---


# smwlibs.asm

**Under constructing...**

|Address|Name                                 |Type       |Description                            |
|:------|:------------------------------------|:----------|:--------------------------------------|
|$0086DF|SMW\_ExecutePtr                      |JSL routine|                                       |
|$0086FA|SMW\_ExecutePtrLong                  |JSL routine|                                       |
|$00F5B7|SMW\_HurtMario                       |JSL routine|                                       |
|$00F606|SMW\_KillMario                       |JSL routine|                                       |
|$05B160|SMW\_SubSideExit                     |JSL routine|                                       |
|$0286BF|SMW\_YoshiStomp                      |JSL routine|                                       |
|$0294C1|SMW\_GroundPound                     |JSL routine|                                       |
|$00BEB0|SMW\_GenerateTile                    |JSL routine|                                       |
|$01ACF9|SMW\_GetRand                         |JSL routine|                                       |
|$02ACE5|SMW\_GivePoints                      |JSL routine|                                       |
|$05B329|SMW\_GiveCoins                       |JSL routine|                                       |
|$03A6C8|SMW\_KillMostSprites                 |JSL routine|                                       |
|$01AA33|SMW\_BoostMarioSpeed                 |JSL routine|                                       |
|$03B664|SMW\_GetMarioClipping                |JSL routine|                                       |
|$03B69F|SMW\_GetSpriteClippingA              |JSL routine|                                       |
|$03B6E5|SMW\_GetSpriteClippingB              |JSL routine|                                       |
|$03B72B|SMW\_CheckContack                    |JSL routine|                                       |
|$07F7D2|SMW\_InitSpriteTables                |JSL routine|                                       |
|$01801A|SMW\_UpdateSpriteYPosNoGravity       |JSL routine|                                       |
|$018022|SMW\_UpdateSpriteXPosNoGravity       |JSL routine|                                       |
|$01802A|SMW\_UpdateSpritePosWithGravity      |JSL routine|                                       |
|$018032|SMW\_SpriteSpriteInteraction         |JSL routine|                                       |
|$01A7DC|SMW\_SpriteMarioInteraction          |JSL routine|                                       |
|$01803A|SMW\_SpriteInteraction               |JSL routine|                                       |
|$019138|SMW\_SpriteObjectInteraction         |JSL routine|                                       |
|$01B7B3|SMW\_SpriteFinishOAMWrite            |JSL routine|                                       |
|$0DA8A6|SMW\_IndexToBit                      |Data       |                                       |
|$07F7DB|SMW\_CircleCoords                    |Data       |                                       |


---


# libAkagi.asm

The library of frequently used routines.

|Name                         |Type       |Description                            |
|:----------------------------|:----------|:--------------------------------------|
|[GetDrawInfo](#akagi-gdi)    |JSL routine|Get sprite tile map index.             |
|[SubOffScreen](#akagi-sos)   |JSL routine|Kill sprite when sprite is off screen. |

## GetDrawInfo # {#akagi-gdi}

Get sprite tile map index.

### Register width

|Register      |Width |
|:-------------|:-----|
|Accumlator (A)|8 bits|
|Index    (X,Y)|8 bits|

### Inputs

|Input      |Description                                                                                                                                  |
|:----------|:--------------------------------------------------------------------------------------------------------------------------------------------|
|XL         |Index of currently executing sprite                                                                                                          |

### Return

|Return     |Description                    |
|:----------|:------------------------------|
|YL         |Index to sprite tile map($0300)|


## SubOffScreen # {#akagi-sos}

Kill sprite when sprite is off screen.

**Caution: The Inputs specification differs from PIXI's one.**

### Register width

|Register      |Width |
|:-------------|:-----|
|Accumlator (A)|8 bits|
|Index    (X,Y)|8 bits|

### Inputs

|Input      |Description                                                                                                                                  |
|:----------|:--------------------------------------------------------------------------------------------------------------------------------------------|
|YL         |Sub off screen type<br>  **0**: X0<br>  **2**: X1<br> **4**: X2<br>  **6**: X3<br>  **8**: X4<br>  **10**: X5<br>  **12**: X6<br>  **14**: X7|
|XL         |Index of currently executing sprite                                                                                                          |

### Return

none.

### Note

The operation from X4 to X7 processing isn't guaranteed.


---


# libScroller.asm

Library about scroller process.

|Name                              |Type       |Description                            |
|:---------------------------------|:----------|:--------------------------------------|
|[UpdateLayerPos](#scr-ulp)        |JSL routine|The routine for update layer position. |

## UpdateLayerPos # {#scr-ulp}

The routine for update layer position.

Add the value of $1446+X to $146E+X (and $1462+X).

### Register width

|Register      |Width  |
|:-------------|:------|
|Accumlator (A)|16 bits|
|Index    (X,Y)| 8 bits|

### Inputs

|Input      |Description                                                                                      |
|:----------|:------------------------------------------------------------------------------------------------|
|XL         |Position type<br>  **0**: Layer1 X<br>  **2**: Layer1 Y<br>  **4**: Layer2 X<br>  **6**: Layer2 Y|

### Return

none.


---


# libGarbage.asm

Misc library.

|Name                              |Type       |Description                            |
|:---------------------------------|:----------|:--------------------------------------|
|[ShowShooterSmoke](#gbg-sss)      |JSL routine|Generate smoke for shooter.            |

## ShowShooterSmoke # {#gbg-sss}

Generate smoke for shooter.

### Register width

|Register      |Width |
|:-------------|:-----|
|Accumlator (A)|8 bits|
|Index    (X,Y)|8 bits|

### Inputs

|Input      |Description                               |
|:----------|:-----------------------------------------|
|AL         |The number of extended sprite for generate|
|XL         |Index of currently executing shooter      |

### Return

none.


---


