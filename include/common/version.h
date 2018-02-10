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
#define AppVersion		0.50
#define ResVersion		0,5,0,0
#define FileVersion		1

/**
 * Core DLL Name
 */
#define _DllAppName	Mewthree
#define DllAppName	"Mewthree"
/**
 * DLL Version
 */
#define DllAppVersion		0.50
#define DllAppVersionInt	005000
#define DllResVersion		0,5,0,0
#define DllFileVersion		1

/**
 * GUI App Name
 */
#define Win32AppName		"PIEE"
#define _Win32AppName		PIEE
/**
 * GUI App Version
 */
#define Win32AppVersion		0.01
#define Win32ResVersion		0,0,1,0
#define Win32FileVersion	0

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
#define CodeVer_Maj		0x00
#define CodeVer_Min		0x01
#define CodeVersion		((CodeVer_Maj<<8)+CodeVer_Min)

