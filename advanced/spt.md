---
layout: page
title: Use Romi's Sprite Tool's sprites
description: 
---

# Requires

- [GIEPY](/#download)

- ROM file that has not installed any SpriteTools

# Procedure

**No special procedure is necessary.**

1. Edit the **[sprites list file](/fspecs/list)**.

2. Correct the sprite's **[config file](/fspecs/spconfig)** for GIEPY.  

3. Execute GIEPY.

# Notes

Some sprites probably need modifications.

## Sprites that generate custom sprites

Specifically, it's a sprite that calls this routine.

|Routine|Description                   |
|:------|:-----------------------------|
|$0187a7|Routine for initialize sprite.|

In GIEPY, since the content of extra bits ram has been changed,
it will not work without modification.

**Extra bits RAM Before \(Romi's Sprite Tool / PIXI\)**

| bit    |Description            |
|:-------|:----------------------|
| bit0-1 |Not used               |
| bit2   |Sprite operation option|
| bit3   |**Custom sprite bit**  |
| bit4-7 |Not used               |

**Extra bits RAM After \(GIEPY\)**

| bit    |Description                                                     |
|:-------|:---------------------------------------------------------------|
| bit0-1 |Not used                                                        |
| bit2-3 |[Sprite group](/fspecs/list#sprite-insertion-destination-group) |
| bit4-6 |Not used                                                        |
| bit7   |**Custom sprite bit**                                           |

Also, this information must be set before calling **$0187a7**.

### Specific modification example

I introduce a correction example of Boomelang bros(**boomelang\_bro.asm**) included to Sprite Tool.

1. Comment out lines **12** and **13** (For use the setting of GIEPY).

   ```asm
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;; Boomerang Brother, by mikeyk
   ;;
   ;; Description: Similar to his role in SMB3, this guy throws boomerangs at Mario. 
   ;;
   ;; BIG FAT NOTE: This sprite depends on the boomerang sprite.  Make sure you insert the
   ;; boomerang as the very next sprite.  (ex. If this is sprite 1B, make the boomerang 1C)
   ;;
   ;; Uses first extra bit: NO
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   
   ;                    EXTRA_BITS = $7FAB10
   ;                    NEW_SPRITE_NUM = $7FAB9E    ;08 bytes   custom sprite number
   
                       JUMP_TIMER = $163E
                       RAM_ThrowTimer = $1504
   ```

2. Change line **259 to line 261**.

   **Before**

   ```asm
                    JSL $0187A7             ;  get table values for custom sprite 
                    LDA #$08 
                    STA EXTRA_BITS,x
   ```

   **After**

   ```asm
                    LDA #$88                ;  Set custom sprite group2
                    STA EXTRA_BITS,x
                    JSL $0187A7             ;  get table values for custom sprite 
   ```

---


