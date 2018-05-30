#pragma once
/**
 * @file version.h
 */

/**
 * CUI App Name
 */
#define AppName		"GIEPY"
#define _AppName	GIEPY
/**
 * CUI Version
 */
#define AppVersion		"1.0.2"
#define ResVersion		1,0,2,0
#define FileVersion		4

/**
 * Core DLL Name
 */
#define _DllAppName	Mewthree
#define DllAppName	"Mewthree"
/**
 * DLL Version
 */
#define DllAppVersion		1.0.2
#define DllAppVersionInt	10002
#define DllResVersion		1,0,2,0
#define DllFileVersion		4

/**
 * GUI App Name
 */
#define Win32AppName		"PIEE"
#define _Win32AppName		PIEE
/**
 * GUI App Version
 */
#define Win32AppVersion		"1.0.2"
#define Win32ResVersion		1,0,2,0
#define Win32FileVersion	3

/**
 * GIEPY main asm version
 *   You have to check these sources if you change it.
 *     - src/mewthree/Uninstall.c
 *         ... UninstallMainData() function
 *             It detects action for uninstall.
 *     - src/mewthree/InitializeRom.c
 *         ... InitializeRom() function
 *             It checks the version.
 *     - asm/giepy.asm
 *         ... It's necesary to match the version info
 *             of this one.
 */
#define CodeVer_Maj		0x01
#define CodeVer_Min		0x02
#define CodeVersion		((CodeVer_Maj<<8)+CodeVer_Min)

