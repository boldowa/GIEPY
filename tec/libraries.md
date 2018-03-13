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

|Name                |Type       |Description                            |
|:-------------------|:----------|:--------------------------------------|
|                    |JSL routine|                                       |


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


