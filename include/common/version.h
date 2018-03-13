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
#define AppVersion		"1.0.1"
#define ResVersion		1,0,1,0
#define FileVersion		3

/**
 * Core DLL Name
 */
#define _DllAppName	Mewthree
#define DllAppName	"Mewthree"
/**
 * DLL Version
 */
#define DllAppVersion		1.0.1
#define DllAppVersionInt	010001
#define DllResVersion		1,0,1,0
#define DllFileVersion		3

/**
 * GUI App Name
 */
#define Win32AppName		"PIEE"
#define _Win32AppName		PIEE
/**
 * GUI App Version
 */
#define Win32AppVersion		"1.0.1"
#define Win32ResVersion		1,0,1,0
#define Win32FileVersion	2

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
#define CodeVer_Min		0x01
#define CodeVersion		((CodeVer_Maj<<8)+CodeVer_Min)

