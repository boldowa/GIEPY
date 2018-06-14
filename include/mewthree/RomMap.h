#pragma once
/**
 * @file RomMap.h
 */
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	MT_Unknown = 0,
	MT_LoRom,
	MT_HiRom,
	MT_SA1Rom,
	MT_ExLoRom,
	MT_ExHiRom,
}MapType;

typedef struct _RomMap {
	const char	name[10];
	MapType		val;
}RomMap;

void getmapmode(RomFile*, RomMap*);

#ifdef __cplusplus
} /*extern "C" */
#endif
