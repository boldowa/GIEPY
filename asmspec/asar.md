---
layout: page
title: ASAR sprites coding guide
description: ''
---

Part of this specification conforms to that of **Romi's Sprite Tool** or **PIXI**.

# Entry points

Sprite asm file has 2 entry points.

They are named [INIT](#init-routine) and [MAIN](#main-routine) respectively.

Routines that are not used can be omitted.

## Routines usage

|Sprite Type                 |INIT routine|MAIN routine|
|:---------------------------|:----------:|:----------:|
|Sprite - SMW original(00-00)|     ×     |     ×     |
|Sprite - SMW tweak(01-00)   |     ×     |     ×     |
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

You can declare the routine by print syntax or label name.

Label name isn't case sensitive.

#### Declaration by print syntax

```asm
	print	"INIT ", pc

	; ----
	; some codes
	; ----

	rtl
```

#### Declaration by label name

```asm
init:
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

You can declare the routine by print syntax or label name.

Label name isn't case sensitive.

#### Declaration by print syntax

```asm
	print	"MAIN ", pc

	; ----
	; some codes
	; ----

	rtl
```

#### Declaration by label name

```asm
main:
	; ----
	; some codes
	; ----

	rtl
```

In general, it's often used as follows.

```asm
	print	"MAIN ", pc

	;--- switch to current bank
	phb
	phk
	plb

	;--- call main process
	jsr	SubProcess1
	bcc	+
	jsr	SubProcess2

	;--- restore bank register
+	plb
	rtl

SubProcess1:
	; ---
	; any codes
	; ---
	rts

SubProcess2:
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
;  --- Require libraries ---
;
;    - libAkagi
;        + GetDrawInfo
;        + SubOffScreen
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
!SOMETHING	= 3


;---------------------------------------
; Init entry point
;---------------------------------------
	print	"INIT ", pc

	;--- TODO: Initialize

	rtl


;---------------------------------------
; Main entry point
;---------------------------------------
	print	"MAIN ", pc

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
SpriteMain:
	;--- TODO: Control
	;--- TODO: Graphics
	lda	#!SOMETHING
	rts
```

---



