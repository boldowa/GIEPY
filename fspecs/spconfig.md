---
layout: page
title: The sprite config files
description: 
---

Sprite files are require a config file.

# Supports

|                      |00-01<br>Tweak   |01-01<br>Regular|03-02<br>Shooter|03-03<br>Generator|03-04<br>Initializer|03-05<br>Scroller|
|:---------------------|:---------------:|:--------------:|:--------------:|:----------------:|:------------------:|:---------------:|
|**Tweaks**            | ○              | ○             | ×             | ×               | ×                 | ×              |
|**Extra props**       | ×              | ○             | ×             | ×               | ×                 | ×              |
|**ASM file name 1**   | ×              | ○             | ○             | ○               | ○                 | ○              |
|**Unique code**       | ×              | ○             | ○             | ○               | ○                 | ○              |
|**Extra bytes length**| ×              | ○<br>(max:4)  | ○<br>(max:1)  | ○<br>(max:1)    | ○<br>(max:251)    | ○<br>(max:1)   |
|**ASM file name 2**   | ×              | ×             | ×             | ×               | ×                 | ○              |

# files

You can use these files.

- [Config(.cfg)](#config-cfg)

- [Json(.json)](#json-json)

## Config (.cfg)

- Config Data (line1 - 10)

- Tag Data

### Detail

#### Config Data

##### line 1

Sprite type.

|Number|Type                                                   |
|:-----|:------------------------------------------------------|
|00    |Tweaks (Use **[Act Like](#line-2)** sprite's asm code) |
|other |Use custom asm codes                                   |

##### line 2

Act like number.

##### line 3

Tweaks.

##### line 4

Extra props.

##### line 5

ASM file name 1.

##### line 6

ASM file type. It's ignored in GIEPY.

##### line 7

Sprite sub-type.

|Number|Type                               |
|:-----|:----------------------------------|
|00    |(Reserved for SMW original sprites)|
|01    |Regular sprite (or Tweak only)     |
|02    |Shooter sprite                     |
|03    |Generator sprite                   |
|04    |Initializer sprite                 |
|05    |Scroller sprite                    |
|other |Invalid (same as 00)               |

##### line 8

Unique code.

##### line 9

Extra bytes length.

##### line 10

ASM file name 2.

It's used for scroller sprite.

#### Tag Data

##### Name
Sprite name \(\*for display\)

##### Description
Sprite description \(\*for tooltip\)

##### Position
X and Y position on Sprites Window(**X**,**Y**).

**example**
```
[Position]
7,7
```

##### Label
Use text instead of tiles when it exist.
It is useful for command sprites. (e.g. scroller)

##### Tiles
List of tile(**X**,**Y**,**Tile**).

**X** and **Y** is dec, **Tile** is hex.

  **example**
```
[Tiles]
0,0,301
0,-16,300
-8,0,302
```

### Example

```
FF			; ignored
FF			; ignored
FF FF FF FF FF FF	; ignored
FF FF			; ignored
coin.asm		; layer1 scroll asm (If not specified(empty), the layer 1 scroller is disabled)
FF			; ignored
05			; type 5: scroller
00			; unique info: none
00			; extra bytes: none
coin.asm		; layer2 scroll asm (If not specified(empty), the layer 2 scroller is disabled)

---
[Name]
Coin
[Description]
Get coins infinity.
[Label]
Coin
[Position]
7,7
```


## Json (.json)

### Detail

|Name                 |Remarks                                 |
|:--------------------|:---------------------------------------|
|Type                 |[Sprite type](#line-1)                  |
|SubType              |[Sprite sub-type](#line-7)              |
|ActLike              |[Act line number](#line-2)              |
|$1656                |[$1656](#json-1656)                     |
|$1662                |[$1662](#json-1662)                     |
|$166E                |[$167E](#json-166e)                     |
|$167A                |[$167A](#json-167a)                     |
|$1686                |[$1686](#json-1686)                     |
|$190F                |[$190F](#json-190f)                     |
|Extra Property Byte 1|                                        |
|Extra Property Byte 2|                                        |
|Unique Info          |                                        |
|AsmFile              |ASM file name 1                         |
|AsmFile2             |ASM file name 2 \(\*Scroller only\)     |
|Extra Bytes Length   |                                        |
|Name                 |Sprite name \(\*for display\)           |
|Description          |Sprite description \(\*for tooltip\)    |
|X                    |X position on Sprites Window            |
|Y                    |Y position on Sprites Window            |
|Label                |Use text instead of tiles when it exist |
|Tiles                |[Tiles](#json-tiles)                    |

#### $1656 # {#json-1656}

|Name                                        |Type |Remarks      |
|:-------------------------------------------|:---:|:------------|
|Object Clipping                             |int  |0 - 31       |
|Can be jumped on                            |bool |             |
|Dies when jumped on                         |bool |             |
|Hop in/kick shell                           |bool |             |
|Disappears in cloud of smoke                |bool |             |

#### $1662 # {#json-1662}

|Name                                        |Type |Remarks      |
|:-------------------------------------------|:---:|:------------|
|Sprite Clipping                             |int  |0 - 63       |
|Use shell as death frame                    |bool |             |
|Fall straight down when killed              |bool |             |

#### $166E # {#json-166e}

|Name                                        |Type |Remarks      |
|:-------------------------------------------|:---:|:------------|
|Use second graphics page                    |bool |             |
|Palette                                     |int  |0 - 7        |
|Disable fireball killing                    |bool |             |
|Disable cape killing                        |bool |             |
|Disable water splash                        |bool |             |
|Don't interact with Layer 2                 |bool |             |

#### $167A # {#json-167a}

|Name                                        |Type |Remarks      |
|:-------------------------------------------|:---:|:------------|
|Don't disable cliping when starkilled       |bool |             |
|Invincible to star/cape/fire/bounce blk.    |bool |             |
|Process when off screen                     |bool |             |
|Don't change into shell when stunned        |bool |             |
|Can't be kicked like shell                  |bool |             |
|Process interaction with Mario every frame  |bool |             |
|Gives power-up when eaten by Yoshi          |bool |             |
|Don't use default interaction with Mario    |bool |             |

#### $1686 # {#json-1686}

|Name                                        |Type |Remarks      |
|:-------------------------------------------|:---:|:------------|
|Inedible                                    |bool |             |
|Stay in Yoshi's mouth                       |bool |             |
|Weird ground behaviour                      |bool |             |
|Don't interact with other sprites           |bool |             |
|Don't change direction if touched           |bool |             |
|Don't turn into coin when goal passed       |bool |             |
|Spawn a new sprite                          |bool |             |
|Don't interact with objects                 |bool |             |

#### $190F # {#json-190f}

|Name                                        |Type |Remarks      |
|:-------------------------------------------|:---:|:------------|
|Make platform passable from below           |bool |             |
|Don't erase when goal passed                |bool |             |
|Can't be killed by sliding                  |bool |             |
|Takes 5 fireballs to kill                   |bool |             |
|Can be jumped on with upwards Y speed       |bool |             |
|Death frame two tiles high                  |bool |             |
|Don't turn into a coin with silver POW      |bool |             |
|Don't get stuck in walls (carryable sprites)|bool |             |

#### Tiles # {#json-tiles}

An array with the following as a unit.

|Name                                        |Type |Remarks                              |
|:-------------------------------------------|:---:|:------------------------------------|
|X                                           |int  |Relative X position on Sprites Window|
|Y                                           |int  |Relative Y position on Sprites Window|
|Tile                                        |int  |Sprite Map16 tile number             |



### Example

```json
{
	"Type":                                                     1,
	"SubType":                                                  1,
	"ActLike":                                                 54,

	"$1656": {
		"Object Clipping":                                  0,
		"Can be jumped on":                              true,
		"Dies when jumped on":                          false,
		"Hop in/kick shell":                            false,
		"Disappears in cloud of smoke":                 false
	},
	"$1662": {
		"Sprite Clipping":                                 55,
		"Use shell as death frame":                     false,
		"Fall straight down when killed":                true
	},
	"$166E": {
		"Use second graphics page":                      true,
		"Palette":                                          4,
		"Disable fireball killing":                     false,
		"Disable cape killing":                         false,
		"Disable water splash":                         false,
		"Don't interact with Layer 2":                  false
	},
	"$167A": {
		"Don't disable cliping when starkilled":         true,
		"Invincible to star/cape/fire/bounce blk.":     false,
		"Process when off screen":                      false,
		"Don't change into shell when stunned":         false,
		"Can't be kicked like shell":                   false,
		"Process interaction with Mario every frame":   false,
		"Gives power-up when eaten by Yoshi":           false,
		"Don't use default interaction with Mario":     false
	},
	"$1686": {
		"Inedible":                                     false,
		"Stay in Yoshi's mouth":                        false,
		"Weird ground behaviour":                       false,
		"Don't interact with other sprites":            false,
		"Don't change direction if touched":             true,
		"Don't turn into coin when goal passed":        false,
		"Spawn a new sprite":                           false,
		"Don't interact with objects":                  false
	},
	"$190F": {
		"Make platform passable from below":            false,
		"Don't erase when goal passed":                 false,
		"Can't be killed by sliding":                   false,
		"Takes 5 fireballs to kill":                    false,
		"Can be jumped on with upwards Y speed":        false,
		"Death frame two tiles high":                   false,
		"Don't turn into a coin with silver POW":       false,
		"Don't get stuck in walls (carryable sprites)": false
	},

	"Extra Property Byte 1":                                    0,
	"Extra Property Byte 2":                                    0,
	"Unique Info":                                             15,

	"AsmFile":                                      "FireBro.asm",
	"AsmFile2":                                                "",

	"Extra Bytes Length":                                       0,

	"Name": "Fire Bros",

	"Description": [
		"The Fire Bro from Super Mario Bros. 3.",
		"It walks around and occasionally hops.\\nIt periodically spits 2 fireballs at Mario."
	],

	"X": 7, "Y": 7,

	"Tiles": [
		{ "X":   0, "Y":   0, "Tile": 769 },
		{ "X":   0, "Y": -16, "Tile": 768 },
		{ "X":  -8, "Y":   0, "Tile": 770 }
	]
}
```


---


