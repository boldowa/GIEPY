#pragma once
/**
 * @file srcpath.h
 */
#ifdef __cplusplus
extern "C" {
#endif

#if isWindows
#  define DELIM "\\"
#else
#  define DELIM "/"
#endif

#define SA1Def		"asm" DELIM "include" DELIM "sa1def.inc" 
#define SystemAsm	"asm" DELIM "giepy.asm"
#define SmwLibAsm	"asm" DELIM "smwlibs.asm"
#define TrasmInc	"include" DELIM "trasm.inc" 
#define AsarInc		"include" DELIM "asar.inc" 
#define TrasmIncSys	"asm" DELIM "include" DELIM "trasm.inc" 
#define AsarIncSys	"asm" DELIM "include" DELIM "asar.inc" 
#define PixiDef		"asm" DELIM "include" DELIM "pixi.inc" 

#define AsarDefAsm	"def_asar.inc"
#define AsarLibAsm	"lib_asar.inc"
#define AsarFNMacAsm	"fnm_asar.inc"
#define TrasmDefAsm	"def_trasm.inc"
#define TrasmLibAsm	"lib_trasm.inc"

#define TrasmLoDef	"asm" DELIM "include" DELIM "tr_lodef.inc"
#define TrasmSA1Def	"asm" DELIM "include" DELIM "tr_sa1def.inc"

typedef struct _srcdirs {
	const char* sprite;
	const char* extended;
	const char* cluster;
	const char* overworld;
}srcdirs;

#ifdef __cplusplus
} /*extern "C" */
#endif
