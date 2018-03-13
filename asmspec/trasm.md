---
layout: page
title: TRASM sprites coding guide
description: ''
---

Part of this specification conforms to that of **Romi's Sprite Tool**.

# Entry points

Sprite asm file has 2 entry points.

They are named [INIT](#init-routine) and [MAIN](#main-routine) respectively.

Routines that are not used can be omitted.

## Routines usage

|Sprite Type                 |INIT routine|MAIN routine|
|:---------------------------|:----------:|:----------:|
|Sprite - SMW original(00-00)|     ×     |     ×     |
|Sprite - SMW tweak(00-01)   |     ×     |     ×     |
|Sprite - Regular(01-01)     |     ○     |     ○     |
|Sprite - Shooter(03-02)     |     ×     |     ○     |
|Sprite - Generator(03-03)   |     ×     |     ○     |
|Sprite - Initializer(03-04) |     ○     |     ×     |
|Sprite - Scroller(03-05)    |     ○     |     ○     |
|Extended Sprite             |     ×     |     ○     |
|Cluster Sprite              |     ×     |     ○     |

## INIT routine

It's a routine for initialization.

It is called only once when loading a sprite.

This routine must end with RTL.

### Declaration

```asm
	dcb	"INIT"

	; ----
	; some codes
	; ----

	rtl
```

## MAIN routine

It's a routine for control a sprite.

It is called every frame.

This routine must end with RTL.

### Declaration

```asm
	dcb	"MAIN"

	; ----
	; some codes
	; ----

	rtl
```

In general, it's often used as follows.

```asm
	dcb	"MAIN"

	;--- switch to current bank
	phb
	phk
	plb

	;--- call main process
	jsr	SubProcess1
	bcc	RETURN
	jsr	SubProcess2

	;--- restore bank register
RETURN
	plb
	rtl

SubProcess1
	; ---
	; any codes
	; ---
	rts

SubProcess2
	; ---
	; any codes
	; ---
	rts

```

# Template

```asm
;-------------------------------------------------
; XXXXXXXXXX
; ----------
;
; Description
;
;
;  --- Sprite Type ---
;
;   Regular sprite (01) / Shooter sprite (02) / ...
;
;
;  --- Extra bits ---
;
;   Use extra bits: YES/NO
;     - bit 2: XXX
;     - bit 3: YYY
;
;
;  --- Unique info ---
;
;   Unique info: foobar / Not used
;
;
;  --- Extra Bytes ---
;
;   Length : 4 bytes / Not used
;     - Byte 0 : foo
;     - Byte 1 : bar
;     - Byte 2 : How much do you like me?
;     - Byte 3 : A lot <3
;
;
;-------------------------------------------------

;---------------------------------------
; Configs
;---------------------------------------
SOMETHING	= 3


;---------------------------------------
; Init entry point
;---------------------------------------
	dcb	"INIT"

	;--- TODO: Initialize

	rtl


;---------------------------------------
; Main entry point
;---------------------------------------
	dcb	"MAIN"

	;--- switch to current bank
	phb
	phk
	plb

	;--- call main process
	jsr	SpriteMain

	;--- restore bank register
	plb
	rtl

;---------------------------------------
; Main code
;---------------------------------------
SpriteMain
	;--- TODO: Control
	;--- TODO: Graphics
	lda	#SOMETHING
	rts
```

---


