---
layout: page
title: Sprites list format
description: 
---

The sprites list file has **Section** syntax and **List** syntax.

Use the **Section** syntax to switch the sprite type, and use the **List** syntax
to specify the insertion number.

# Syntax

## Section

    [Sprite]

"**[**" + sprite type + "**]**"

For the sprite type, the following can be specified.

|type|description|
|:---|:----------|
|Sprite|Switch to [Regular sprite mode](#regular-sprite-mode).|
|Extended|Switch to [Extended sprite mode](#extended--cluster-sprite-mode).|
|Cluster|Switch to [Cluster sprite mode](#extended--cluster-sprite-mode).|
|Remarks|Switch to remarks mode.<br>Ignore all strings except section syntax.|

## List

### Regular sprite mode

    01:2    goomba.cfg

#### Sprite insertion destination number

<span style="color:red;">**01**</span>:2 goomba.cfg

First field is sprite insertion destination number.

#### Sprite insertion destination group

01<span style="color:red;">:**2**</span> goomba.cfg

Second field is sprite insertion destination group number.

This value can be omitted.

Multiple groups can be specified simultaneously by separating with "**,**".  
You can specify the range by using "**-**".

#### Sprite config file

01:2 <span style="color:red;">**goomba.cfg**</span>

Third filed is a config file name.

You can specify a file of the following format.

- Sprite Tool's config file (**.cfg**)

- Json file (**.json**)



### Extended / Cluster sprite mode

    15      fire.asm

#### Sprite insertion destination number

<span style="color:red;">**15**</span> fire.asm

First field is a sprite insertion destination number.

#### Sprite asm file

15 <span style="color:red;">**fire.asm**</span>

Second field is sprite asm file.


## Comment

    // This is comment.

All strings after "//" become comments and are ignored.


# Example

**sprites.txt**

```
[Remarks]
  This is comment.
  Remarks section winn ignored any texts.

[Sprite]
00	venus.cfg	// It'll insert venus into group 2 and 3(Romi's Sprite Tool and PIXI compatible).
00:1	goomba.cfg
01:1-3	shy_guy.json	// It'll insert shy_guy into group 1, 2, and 3.
02:0	2Pcolor.json	// It'll replace from the SMW original sprite to custom sprite.

[Extended]
13	fireball.asm	// Custom extended sprites start from 13.

[Cluster]
09	rain.asm	// Custom cluster sprites start from 09.
```

---


