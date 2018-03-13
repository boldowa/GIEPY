# GIEPY

![cui interface](pic/giepy_cui.png) ![gui interface](pic/piee_gui.png)

This is a sprite insertion tool for Super Mario World ROM.

Main program is written in C language.  
And the main patches are assembly file for Asar.


## Feature

This tool has the features like sprite tool, but has the following enhancements.

- More than 600 sprites can be inserted. (Including replacement of existing sprites, up to 1024 can be inserted.)

- Any type of sprite can be inserted into an arbitrary number. (For example, you can insert generator into sprite number 0.)

- Added new type sprites ... **Initializer**, **Scroller**


Since the meaning of the extra-bits has been changed so far, attention is necessary.

However, there is also a way to use it as before.



## How to use

Under constructing ...


## How to compile

This program require [cmake](https://cmake.org).

Please install it.

And if you are using the Linux OS, it needs libasar.so.

### Windows

1. Clone [this repository](https://github.com/boldowa/smw-utils).

2. Move to **GIEPY** directory.

3. Load **CMakeList.txt** with cmake(or cmake-gui).

4. Compile.

And if you are using python(3), you can use **release\_win32.py** to create a release package.  
(However, I think customization is essential.)

### Linux

1. Clone [this repository](https://github.com/boldowa/smw-utils).

2. Move to **GIEPY/build** directory.

3. Run "build.sh".


