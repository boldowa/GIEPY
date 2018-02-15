+------------------------------------------------------------------------------+
| GIEPY - New type SMW Sprites insertion tool -                                |
| ---------------------------------------------                                |
|                                                                              |
|   - ver. 1.00                                                                |
|                                                                              |
|   Repository : https://github.com/boldowa/giepy/                             |
|   Web        : https://boldowa.github.io/giepy/                              |
|                                         not yet...                           |
|                                                                              |
|   Changelog)                                                                 |
|                                                                              |
|     01-12-2018 [v0.01] : beta version (for SMWC C3).                         |
|                                                                              |
|     02-15-2018 [v1.00] : first release version.                              |
|                                                                              |
+------------------------------------------------------------------------------+

  OH MY SHIT DOCUMENTS...........................!!! sorry.


[What is this?]
  This tool is the sprite insertion tool for Super Mario World.

  This tool supports these rom map type:
    - LoRom
    - ExLoRom (probably... tbh, idk :p )
    - SA-1Rom (Vitor's SA-1 patch compatible)


  This tool is compatible with both TRASM and ASAR.
  Therefore, sprite tool's sprite can be used almost as it is.
  (*However, custom sprites that generate custom sprites need to be modified.)


  The initialization code of this tool implements strict assertion function.
  So, later installation of this tool may make tool conflicts easier to detect.


  I haven't made the uninstall function of the old tool.
  I don't want to make it so please give up.
  If you are dissatisfied, please complain to other tool authors.



[Supported system]
  - Windows
  - Linux    (Binary and source isn't included. Please clone from repository.)

     ... other systems? I don't have machine. :P



[Folder structure]
  |
  |-- giepy.exe                        ... CUI interface program.
  |
  |-- piee.exe                         ... GUI interface program.
  |
  +-- InsertPrompt.bat                 ... Script with an insert interface like sprite tool.
  |                                        Use it if you want to use the same way as the sprite tool.
  |
  +-- giepy.bat                        ... The script for when renaming giepy.exe
  |                                        Normally it isn't used.
  |
  +-- components                       ... system module directory.
  |    |
  |    +-- mewthree.dll                ... It is GIEPY's main module.
  |    |
  |    +-- asar.dll                    ... Asar assembler module. (v1.5.0+)
  |    |
  |    +-- trasm_b.exe                 ... trasm assembler.
  |    |                                   (Modified version by Nanashi)
  |    |
  |    +-- jansson.dll                 ... The library for load (and write) json files.
  |    |
  |    +-- tee.bat                     ... tee command for windows. It's not used now.
  |
  +-- asm                              ... system asm directory.
  |    |
  |    +-- giepy.asm                   ... GIEPY's init patch. 
  |    |                                   This patch will apply to rom automatically.
  |    |
  |    +-- smwlibs.asm                 ... smw jsl libraries declaration file.
  |    |
  |    +-- giepy
  |    |     |
  |    |     +-- cluster.asm
  |    |     +-- extended.asm
  |    |     +-- generator.asm         ... These files are included from giepy.asm
  |    |     +-- initializer.asm
  |    |     +-- load.asm
  |    |     +-- normspr.asm
  |    |     +-- owspr.asm
  |    |     +-- scroll.asm
  |    |     +-- shooter.asm
  |    |     +-- tweaks.asm
  |    |
  |    |
  |    +-- include
  |          |
  |          +-- sa1def.inc            ... These files are included from all aprites / library
  |          +-- config.inc                if assemble with asar.
  |          +-- common.inc
  |          +-- macros.inc
  |          +-- inschk.inc
  |          |
  |          |
  |          +-- tr_lodef.inc          ... These files are included from all sprites if assemble with trasm.
  |          +-- tr_sa1def.inc             lodef is lorom defines, and sa1def is sa-1 defines are included.
  |          |
  |          |
  |          +-- asar.inc              ... These files are included from all sprites / library.
  |          +-- trasm.inc                 You can define some defines in this include file.
  |          |                             If local include(*) is exist, these file isn't included.
  |          |                               (*The details will be written later.)
  |          |
  |          +-- pixi.inc
  |          +-- pixi                  ... These are files that for PIXI compatible option.
  |                |
  |                +-- normal_header.inc
  |                +-- shooter_header.inc
  |                +-- generator_header.inc
  |                +-- extended_header.inc
  |                +-- cluster_header.inc
  |
  |
  +-- libraries                        ... Libraries directory.
  |    |                                   If you put any asm in this foler,
  |    +-- commonlib.asm                   it will install automatically.
  |    +-- foolib.asm
  |    +-- barlib.asm
  |           :
  |
  +-- sprites                          ... default directory for sprites
  |
  +-- extendedes                       ... default directory for extended sprites
  |
  +-- clusters                         ... default directory cluster sprites
  |
  +-- asar_licenses                    ... Asar's license files.
  |
  +-- list.txt                         ... default sprites list.
  |
  +-- readme.txt                       ... It's file that just you are reading.


  The source code is not included in the archive.
  Please clone from Github's repository if necessary. (Especially Linux user)





[CUI Command-line option]

Usage: giepy [options] <rom>

***
  -b                           ... Enable EXTRA BYTES feature.
  --bytes                          This option is valid only when installing the system.
                                   *** This feature requires version 1.80 or above version Lunar Magic. ***
***
  -p                           ... Install giepy with pixi compatible mode.
  --pixi
***
  -f                           ... Forcibly reinstall the system.
  --force
***
  -t                           ... Disable to generate Custom Tooltips files (.ssc/mwt/mw2)
  --notooltip
***
  -l           <file name>     ... specify the sprites list file name.
  --list       <file name>         default is "list.txt".
***
  -L           <dir name>      ... specify the libraries directory name.
  --library    <dir name>          default is "libraries".
***
  -s           <dir name>      ... specify the sprites directory name.
  --sprite     <dir name>          default is "sprites".
***
  -e           <dir name>      ... specify the extended sprites directory name.
  --extended   <dir name>          default is "extendedes".
***
  -c           <dir name>      ... specify the cluster sprites directory name.
  --cluster    <dir name>          default is "clusters".
***
  -D           (var)=(value)   ... add define for objects.
  --define     (var)
***
  -u                           ... uninstall all of data about GIEPY from ROM.
  --uninstall
***
  -w                           ... disable verbose warnings.
  --nowarn                         Please use this option if you are dissatisfied with warning filling the console.
                                   It may be better to use this option if you use
                                   st141's sprites and list without modifying these.
***
  -d                           ... enable to output debug info.
  --debug
***
  -v                           ... show program version info.
  --version
***
  -?                           ... show program usage.
  --help
***


[GUI ini file(piee.ini) customizable fields]
***
RecentRomNums                  ... Maximum number of ROM history.
                                   Default is 10.
***
RecentListNums                 ... Maximum number of list history.
                                   Default is 10.
***
DefinesListNums                ... Maximum number of define list saved.
                                   Default is 5.
***


[How to use GUI program]

  ... Is it necessary?
      Please use it in a sensation. :p

      ROM and List text box accept Drag & Drop.



[*** The biggest specification changes from sprite tool / PIXI ***]

  --- The meaning of extra info changes ---
    ... In this tool, extra info is treated as the sprite
        insertion group number. (group 0 to 3).
        
        So, you can insert sprites 4 * 256 = 1024.
        (However, sprites with an insertion number greater than F6 can not be placed with Lunar Magic.)
        (It is possible to generate from shooter etc.)

        When you insert it into group 0, you can replace the existing sprite.


  --- Add new type sprites ---
    In this tool, two types of sprites are added:

      *** Initializer
        This sprite is used for initialize cluster sprites
        in origial game. (Number E1-E6)
        This sprite is execute immediately when sprite is loaded in screen.
        (This sprite has "Init" routine only.)


      *** Scroller
         This sprite is used for Auto-scroll command and Layer2 smash
         in original game. (Number E7-F5)
         This sprite has 2 "Init" and "Main" routines for scroll layer1 and layer2.
         So, you can specify 2 asm files.

         Only one sprite can be placed in the level.
         It'll be ignored if you specify later.


     I don't investigate these types of sprites well,
     so even if I receive a question I can't answer anything.


  --- Procedure of generating custom sprite is different from sprites ---
    It is necessary to set the extra_bits RAM before calling the subroutine.
    Then, when creating a custom sprite, you need to turn bit 7 of Extra bits RAM on.

    For an actual example, please refer to the following file.
      sprites/shooter/grp3cshoot.asm          : line  79 -  91




[List file syntax]
*** ASM will be inserted in order from the top of the list.
+------------------------------------------------+
|//----------                                    |
|                                                |
|29    goomba.cfg                                |
|2a:1  koopa_redshell.cfg                        |
|2a:2  koopa_blueshell.cfg                       |
|2a:3  shyguy.cfg                                |
|                                                |
|[Sprite]                                        |
|00:1-3 hammer_bro.cfg                           |
|01:2,3 boomerang.cfg                            |
|                                                |
|[Extended]                                      |
|13     fireball.asm                             |
|                                                |
|[Cluster]                                       |
|09     rain.asm                                 |
|                                                |
|[Remarks]                                       |
|  This section's comment is ignored.            |
|  You can write formatted comments...           |
|                                                |
|[Sprite]                                        |
|10:0-1,3  bowser.cfg                            |
|      :                                         |
|      :                                         |
+------------------------------------------------+
  - comment
    "//"          ... You can use line comment.

  - Section switcher
    "[Sprite]"    ... switch "Sprite" list mode.

    "[Extended]"  ... switch "Extended" list mode.
    "[Cluster]"   ... switch "Cluster" list mode.
    "[Remarks]"   ... switch "Remarks" mode.
                      this section's parameters will be ignored.

    "[Overworld]" ... For future use.
                      Same as "Remarks" now.

  - list
    00:1
    || |
    || +------------- insertion destination of sprite group. (This value is the value of extra bits.)
    ||                (Available only in "Sprite" section.)
    ||
    ||                if omitted it, sprites insert into group 2 and 3(for sprite tool compatible).
    ||                Multiple group numbers can be specified at the same time.
    ||                Group 0 is for smw original sprites.
    ||                Specifying group 0 as the insertion destination
    ||                causes custom sprites to be generated instead of original sprites.
    ||                For some sprites such as goals, extra bits are used, so please be careful.
    ||
    ||
    ++--------------- Sprite insert numer.
                      In the "Sprite" section, specify the cfg file,
                      otherwise specify the asm file.


[CFG file syntax]
*** You can use ";" comments in cfg file.
+------------------------------------------------+
|01                 ; line  1 : new code flag    |
|36                 ; line  2 : orig sprite num  |
|11 22 33 44 55 66  ; line  3 : tweaks           |
|01 02              ; line  4 : extra prop       |
|luigi.asm          ; line  5 : asm name 1       |  *** in scroller, this is used for layer1 scroller.
|01                 ; line  6 : ignored          |  *** change
|05                 ; line  7 : sprite type      |  *** new
|05                 ; line  8 : unique info      |  *** new
|00                 ; line  9 : extra bytes len  |  *** new
|mario.asm          ; line 10 : asm name 2       |  *** new
|                                                |  \  *** End of config field (new line + "---")
|---------------------                           |  /
|[Name]                                          |  \  *** Sprite's name
|Test sprite                                     |   |
|[Description]                                   |   | *** Sprite's information.
|\[AAA\]Foo Bar Sprite.                          |   |     These fields are used to generate custom tooltips.
|XXXXXX                                          |   |
|[Position]                                      |   |
|7,7                                             |   | *** sprite position ... X and Y
|[Tiles]                                         |   |
|0,0,105                                         |   | *** sprite tile ... X, Y and TileNum
|0,0,114                                         |  /
+------------------------------------------------+

  *** line  6 : ignored
    ... GIEPY detects asm type automaticaly.
        So, this information is ignored.

  *** line  7 : sprite type
    ... Specify sprite type.
            00 ... (reserved for original sprite)
            01 ... regular custom sprite
            02 ... custom shooter
            03 ... custom generator
            04 ... custom initializer
            05 ... custom scroller
            other ... invalid parameter.

  *** line  8 : unique number
    ... This value is stored in the upper 8 bits of the
        A register when the sprite is executed.
        Unlike extra props, this value doesn't change from sprite.
        So, please use it properly with extra props.

  *** line  9 : extra bytes length
    ... specify extra bytes length. (00 - 04)
        *** extra bytes feature is required LM1.80 or above. ***
        
        The initializer can pick up information indefinitely (up to $ fd),
        regular sprites up to 4 bytes, others up to 1 byte.

  *** line 10 : asm name 2
    ... This is for scroller (sprite type 5) only (for layer2 scroll).
        Ignore for other types.

  The 10th and subsequent lines are ignored.

  Also you can use TAG based fields.
  Supported tags: Name, Description, Position, Label, Tiles
  You need to escape "[" and "]" with "\".
  "Tiles" tag is ignored if you specified "Label" tag.



[JSON file specs]

  Please see the following files.

    - sprites/normal/pixi/FireBro.json
    - sprites/scroller/coin.json
    - sprites/scroller/l2_onoff_scr.json


[ASM format]

  There is no specification change from sprite tool.



[local include]

  If you place a file with the following configuration in the same directory as
  the list file, these are read in place of the files in the system directory.
  I think that it is useful when you participate in multiple projects.
  
  e.g.

  |
  +-- list.txt
  |
  +-- include
        |
        +-- asar.inc    ... common include for asar.
        |
        +-- trasm.inc   ... common include for trasm.


[Share code feature]

  *** Because there was a problem with the specification of PIXI using macros,
      specification of macro call was abolished.
      If you want to enable subrooutine calls in PIXI format, please use
      PIXI compatible option.

  The ASM in the libraries folder will insert as a shared code.

  When referring to the label of these files from the sprite,
  it will be inserted automatically.
  (*However, TRASM isn't supported this feature..)

  Labels in the library are all expored,
  except for labels with "+" and "-" labels and names beginning with "_".
  
  So, if there is a label in the library, you can simply access the library
  from the sprite by simply calling the label with JSL (or LDA Label,x).



[SMW libraries]
  SMW libraries are declarated in asm/smwlibs.asm.
  You can use it in this way.
    jsl  SMW_KillMario     ; jsl library
    lda  SMW_IndexToBit,x  ; table

  This library can also be referenced by TRASM assembly codes.


[MapMode]
  Unko detects mapmode automatically.
  if you want to get mapmode in asm, you can get rom type from "!map" define.
    case
      0 ... Unknown
      1 ... LoRom
      2 ... HiRom
      3 ... SA1Rom
      4 ... ExLoRom
      5 ... ExHiRom


[Sprite valiable]
  You can these variables in object asm.

    - !GIEPY_VER          ... GIEPY version code. (The format major*10000 + minor*100 + bugfix*1)
    - !sa1 (or !SA1)      ... When the ROM type is SA1ROM, a value other than 0 is set.
    - !PIXI_COMPATIBLE    ... When PIXI compatible mode is enabled, a value other than 0 is set.
    - !EXTRA_BYTES_EXT    ... When extra bytes feature is enabled, a value other than 0 is set.
    - !g_extra_bytes_len  ... Extra bytes length is stored. It becomes 0 when the feature is disabled.
    - !g_sprite_group     ... the group number  (0 - 3)      (*1)
    - !g_sprite_number    ... the sprite number ($00 - $ff)  (*2)
    - !g_sprite_type      ... 1 = sprite,  2 = extended, 3 = cluster(, 4 = overworld)
    - !g_sprite_subtype   ... below

          The sprite subtype means
            0 ... (reserved for smw original sprites)
            1 ... Regular
            2 ... Shooter
            3 ... Generator
            4 ... Initializer
            5 ... Scroller

  (*1) ... When inserting into multiple numbers, the smallers number is adopted.
  (*2) ... When inserting into multiple numbers, the first number specified will be adopted.



[Search path]
  GIEPY searches list file and libraries and sprites sources in
    (1) Current directory     +  .. Search rank High
    (2) List file directory   |
    (3) ROM file directory    |
    (4) GIEPY exe directory   |
    (5) [absolute path]       +  .. Search rank Low

  If file names conflict, insert one with the highest search rank.


[Debug macro]
  GIEPY has this debug macro.
  %putdebug("<message>")    ... print message ==> "[Debug] $?????? ... <message>"

  It can output messages only if you define DEBUG on the command line or sources.
  I think that it is convenient because it is not displayed unless you define the DEBUG symbol.

  The definition of this macro is in asm/include/macros.inc.

  *** Do not enable DEBUG at system install. ROM will be broken. ***


[Environment dependent code macros]

  - %GiepyOnly()  : If PIXI compatible mode is enabled, assemble will fails.

  - %PixiSprite() : If PIXI compatible mode is disabled, assemble will fails.


[GIEPY programs library]
  GIEPY uses thiese libraries and program.

    - Asar     ...  (C)Alcaro , Currentry managed by RPGHacker
                       License: LGPLv3
                       https://github.com/RPGHacker/asar

    - Trasm    ...  (C)1994 Norman Yen AKA 1000 Miles [Tricks]
      (win32)          License: Public Domain
                       https://www18.atwiki.jp/sm4wiki_mix/pages/88.html#id_bef5f2fc

                       I'm managing it below:
                       https://github.com/boldowa/Trasm


    - Jansson  ...  (C)2009-2016 Petri Lehtinen
                       License: MIT
                       https://github.com/akheron/jansson

    - bolib    ...  (C)2017 boldowa
                       License: MIT
                       https://github.com/boldowa/bolib



[GIEPY program License]

    MIT License.


[Thanks]

    - FuSoYa                 : Lunar Magic author. god.

    - Alcaro                 : He provided we a useful library. I admire him.
                               Source codes of asar have been very helpful to me.

    - RPGHacker              : He is currently managing Asar.
                               Thank you for your hard work.

    - sprite tool developers : mikeyk, Romi, and more ...

    - JackTheSpades          : PIXI auther.
                               Some asms are based on his.

    - Katrina                : Daiyousei auther.
                               Splitting of the source code imitated her sources.

    - Vitor Vilela           : VilelaBot is VilelaBot. beep.
                               I'm lovin' it.

    - Akaginite              : She gave me various advice while developing. (  ^v')b
                               I'm using the library she created.

    - Ladida                 :
    - Sonikku                : I used the sprite that they created to check the operation of the tool. ;)
    - LX5                    :





































[Useless information]

  - "<ROM name>.ssx" file : When this file exists, the contents of this file are
                            automatically added to the "<ROM name>.ssc" file.
                            I think that it is convenient for overwriting
                            the description of SMW original sprites.

  - Sprite insertion speed:   giepy(FAST!!)  >>  piee(Slow... oh no)

  - Ease of use:              piee(EASY)  >>>  giepy(Simple, but difficult)

  - Since there is no merit of inserting Overworld sprites together,
    there is no plan for implementation in giepy(and piee).

  - Even TRASM, you can generate code for SA1.
    See <del>sprites/normal/trsa1/venus.asm</del> removed.

  - Due to optimization of processing,
    the insertion speed of ASM for TRASM (on Windows) is 4 times faster than sprite tool.

  - The error output of TRASM is not very useful.
    Sometimes a different source error is displayed.

  - I feel I spend too much time for TRASM.

