---
layout: page
title: Library file coding & using guide
description: 
---

# Create library file

The library file is ASAR exclusive.

Library files for TRASM can't be created.

## Format

It is very simple.

Just write the label and process.

The routine must end with RTL.

```asm
CalcSumthing:
	rep	#$20
	lda.b	$00
	clc
	adc.w	#0100
	sta.w	$00
	sep	#$20
	lda.b	$02
	adc.b	#00
	sta.b	$02
	rtl

TableData:
	db	$30, $40, $50
```

In this example, the **CalcSumthing** routine and **TableData** are defined as a library.

Multiple libraries can be defined in one file.

## Label name not to be exported

These will not be exported.

- Label name beginning with "**\_**" \(e.g. `_Return`\)

- "**+**" label

- "**-**" label


# Use library

It is very simple.

Simply put the file in the library directory and
call the routine\(or load the table\) contained in the library.

```asm
	lda.l	TableData
	jsl	CalcSumthing
```

The library automatically inserts what is used.
Insertion is done in library file units.

Be careful not to make many small files, as the capacity
used in the signature will increase.




---


