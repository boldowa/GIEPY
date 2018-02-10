#pragma once
/**
 * @file InsInfo.h
 *   GIEPY Install information struct
 */

#define SA_NULL		0	/* SNES ADDRESS NULL */

/**
 * @brief GIEPY System information struct 
 *        This struct includes ROM pointer.
 *        So, please don't access this object's
 *        pointer after ROM closed.
 */
typedef struct _MewInsInfo {
	/*--- Lunar Magic & ROM info ---*/
	double		lmVersion;
	bool		lmExtBytesEnabled;
	bool		isSA1;
	uint8		lmSpriteLengthBackup[4][256];

	/*--- GIEPY common info ---*/
	uint32		giepyMainAdr;
	uint16		giepyCodeVer;
	bool		giepyIsInstalled;
	bool		giepyIsSA1Code;
	bool		giepyIsExtBytesCode;
	bool		giepyIsPixiCompatible;

	/*--- data ptr--- */
	/* sys */
	uint8*		giepyCodePtr;
	uint8*		giepyExtBytesPtr;
	uint8*		giepySpTypeTableAdrPtr;

	/* sprite */
	uint8*		giepySpTweaksPtrAdrPtr;
	uint8*		giepySpInitPtrAdrPtr;
	uint8*		giepySpMainPtrAdrPtr;

	/* shooter */
	uint8*		giepyShootPtrAdrPtr;

	/* generator */
	uint8*		giepyGenPtrAdrPtr;

	/* initializer */
	uint8*		giepyInitPtrAdrPtr;

	/* scroller */
	uint8*		giepyL1ScrInitAdrPtr;
	uint8*		giepyL1ScrMainAdrPtr;
	uint8*		giepyL2ScrInitAdrPtr;
	uint8*		giepyL2ScrMainAdrPtr;

	/* location info */
	uint8*		giepyClsRelocAdrPtr;
	uint8*		giepyExtRelocAdrPtr;
	uint8*		giepyExecutePtrLongPbAdrPtr;
	uint8*		giepySomethingNopAdrPtr;
	uint8*		giepySpriteNopAdrPtr;
	uint8*		giepyScrollL1NopPtr;
	uint8*		giepyScrollL2NopPtr;
	uint8*		giepyBackup02a9caPtr;
	uint8*		giepyTableGrps[5];
}MewInsInfo;

