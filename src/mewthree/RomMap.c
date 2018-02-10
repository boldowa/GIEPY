/**
 * @file RomMap.c
 */

#include "common/types.h"
#include "file/File.h"
#include "file/RomFile.h"

#include "mewthree/RomMap.h"

void getmapmode(RomFile* rom, RomMap* map)
{
	memset((char*)map->name, '\0', sizeof(char)*10);
	map->val = MT_Unknown;

	switch(rom->mapmode_get(rom))
	{
		case MapMode_20:
		case MapMode_20H:
			strcpy_s((char*)map->name, 10, "lorom");
			map->val = MT_LoRom;
			return;

		case MapMode_21:
		case MapMode_21H:
			strcpy_s((char*)map->name, 10, "hirom");
			map->val = MT_HiRom;
			return;

		case MapMode_SA1:
			strcpy_s((char*)map->name, 10, "sa1rom");
			map->val = MT_SA1Rom;
			return;

		case MapMode_25:
		case MapMode_25H:
			switch(rom->type_get(rom))
			{
				case RomType_ExLoRom:
					strcpy_s((char*)map->name, 10, "exlorom");
					map->val = MT_ExLoRom;
					return;

				case RomType_ExHiRom:
					strcpy_s((char*)map->name, 10, "exhirom");
					map->val = MT_ExHiRom;
					return;

				case RomType_LoRom:
				case RomType_HiRom:
				case RomType_Unknown:
				default:
					break;
			}
			break;

		case MapMode_SPC7110:
		case MapMode_Reserved:
		case MapMode_Unknown:
		default:
			break;
	}

	return;
}
