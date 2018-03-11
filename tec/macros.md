---
layout: page
title: ASM defines and macros
description: 
---

# Defines

|Name                                    |Description                                             |
|:---------------------------------------|:-------------------------------------------------------|
|[GIEPY_VER](#giepy_ver)                 |Version of GIEPY.                                       |
|[sa1](#sa1)                             |Whether it's SA-1 ROM or not.                           |
|[SA1](#sa1)                             |Alias for **sa1**.                                      |
|[PIXI_COMPATIBLE](#pixi_compatible)     |Whether PIXI compatible mode is enabled or not.         |
|[EXTRA_BYTES_EXT](#extra_bytes_ext)     |Whether Extra bytes feature is enabled or not.          |
|[g_extra_bytes_len](#g_extra_bytes_len) |Length of Extra bytes.                                  |
|[g_sprite_group](#g_sprite_group)       |Group number of sprite currently being processed.       |
|[g_sprite_number](#g_sprite_number)     |Insertion number of sprite currently being processed.   |
|[g_sprite_type](#g_sprite_type)         |Type of sprite currently being processed.               |
|[g_sprite_subtype](#g_sprite_subtype)   |Sub-type of sprite currently being processed.           |

## GIEPY_VER

Version of GIEPY.

The format : (**major** \* 10000) + (**minor** \* 100) + (**bugfix** \* 1)

**e.g.** v1.01 becomes **010001**

## sa1

Whether it's SA-1 ROM or not.

It becomes not 0 when SA-1 ROM, otherwise it becomes 0.

## PIXI_COMPATIBLE

Whether PIXI compatible mode is enabled or not.

It becomes not 0 when PIXI compatible mode is enabled, otherwise it becomes 0.

## EXTRA_BYTES_EXT

Whether Extra bytes feature is enabled or not.

It becomes not 0 when Extra bytes feature is enabled, otherwise it becomes 0.

## g_extra_bytes_len

Length of Extra bytes.

It becomes 0 when Extra bytes feature is disabled.

## g_sprite_group

[Group number of sprite](/fspecs/list#sprite-insertion-destination-group) currently being processed.

## g_sprite_number

[Insertion number of sprite](/fspecs/list#sprite-insertion-destination-number) currently being processed.

## g_sprite_type

Type of sprite currently being processed.

|Number|Type           |
|:-----|:--------------|
|01    |Sprite         |
|02    |Extended sprite|
|03    |Cluster sprite |

## g_sprite_subtype

[Sub-type of sprite](/fspecs/spconfig#line-7) currently being processed.




---


# Macros

These macros are available in ASAR only.

|Name                                    |Description                                             |
|:---------------------------------------|:-------------------------------------------------------|
|[putdebug](#putdebug)                   |Print debug message when \[!DEBUG != 0\]                |
|[GiepyOnly](#giepyonly)                 |Generate an error when PIXI compatible mode is enabled. |
|[PixiSprite](#pixisprite)               |Generate an error when PIXI compatible mode is disabled.|

## putdebug

`%putdebug(msg)`

|Argument|Description         |
|:-------|:-------------------|
|msg     |A message for print.|

This macro prints debug message when **DEBUG** define is enabled.

**e.g.**

`%putdebug("Hello")` will be expanded to `[Debug] $XXXXXX ... Hello`



## GiepyOnly

`%GiepyOnly()`

Generate an error when PIXI compatible mode is enabled.


## PixiSprite

`%PixiSprite()`

Generate an error when PIXI compatible mode is disabled.


---


