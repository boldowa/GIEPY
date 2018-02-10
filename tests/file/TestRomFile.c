/**
 * TestRomFile.c
 */
#include "common/types.h"
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <ctype.h>
#include "common/Str.h"
#include "common/ReadWrite.h"
#include "file/FilePath.h"
#include "file/File.h"
#include "file/RomFile.h"
#include "File.protected.h"
#include "RomFile.protected.h"
#include "TestRomFile.h"

/* define fill-byte */
#define FILL 0x00


/* prototypes */
static E_FileOpen dummyOpen(RomFile*);
static bool dummyReload(RomFile*);
static void dummyClose(RomFile*);
static bool dummyWrite(RomFile*);
static uint32 NullSnesAdr(RomFile*, const uint32);
static uint32 RatsSearch(RomFile*, const uint32, RatsSearcher_t);
static uint32 RatsClean(RomFile*, const uint32);
static uint32 RatsSearchFail(RomFile*, const uint32, RatsSearcher_t);
static uint32 RatsCleanFalse(RomFile*, const uint32);
#if 0
static long size_get(RomFile*);
static RomType type_get(RomFile*);
static MapMode mapmode_get(RomFile*);
static uint16 sum_get(RomFile*);
static uint8* GetPcPtr(RomFile*, const uint32);
static uint8* GetSnesPtr(RomFile*, const uint32);
static bool IsValidSum(RomFile*);
static void UseHiRomMapSA1(RomFile*, bool);
#endif


/*--------------- Constructor / Destructor ---------------*/

/**
 * @brief Create RomFile object
 *
 * @return the pointer of object
 */
RomFile* new_RomFile_ForTest(const char* s)
{
	RomFile* romFile;

	/* make objects */
	romFile = new_RomFile(NULL);
	assert(romFile);

	/* specify romtype from arg         */
	/* e.g. "l1024" : lorom, 1024Kbytes */
	switch(s[0])
	{
		case 'l':	/* lorom */
			romFile->pro->type = RomType_LoRom;
			romFile->pro->map = MapMode_20;
			break;

		case 'h':	/* hirom */
			romFile->pro->type = RomType_HiRom;
			romFile->pro->map = MapMode_21;
			break;

		case 's':	/* sa-1rom */
			romFile->pro->type = RomType_LoRom;
			romFile->pro->map = MapMode_SA1;
			break;

		case 'L':	/* exlorom */
			romFile->pro->type = RomType_ExLoRom;
			romFile->pro->map = MapMode_25;
			break;

		case 'H':	/* exhirom */
			romFile->pro->type = RomType_ExHiRom;
			romFile->pro->map = MapMode_25;
			break;

		default:
			romFile->pro->type = RomType_Unknown;
			romFile->pro->map = 0;
			break;
	}
	romFile->pro->size = (long)(atoi(&s[1]) * 1024); /* Kbytes */
	romFile->super.pro->size = romFile->pro->size;
	if(0x10000 > romFile->size_get(romFile))
	{
		delete_RomFile(&romFile);
		return NULL;
	}

	/* modify op */
	romFile->Open = dummyOpen;
	romFile->Close = dummyClose;
	romFile->Write = dummyWrite;
	romFile->Reload = dummyReload;

	return romFile;
}


/*--------------- internal methods ---------------*/

static E_FileOpen dummyOpen(RomFile* self)
{
	uint8* raw;
	uint8 mb;
	uint32 mbits;
	uint32 headerBase;

	assert(self);
	
	mb = 0;
	mbits = 0x400;
	while(mbits < self->size_get(self))
	{
		mb++;
		mbits <<= 1;
	}

	raw = (uint8*)malloc((size_t)self->pro->size * sizeof(uint8));
	assert(raw);

	switch(self->mapmode_get(self))
	{
		case MapMode_20:
		case MapMode_20H:
			headerBase = 0x8000 - 0x40;
			break;

		case MapMode_SA1:
			headerBase = 0x8000 - 0x40;
			break;

		case MapMode_21:
		case MapMode_21H:
			headerBase = 0x10000 - 0x40;
			break;

		case MapMode_25:
		case MapMode_25H:
			assert(0x410000<self->size_get(self));
			switch(self->type_get(self))
			{
				case RomType_ExLoRom:
					headerBase = 0x408000 - 0x40;
					break;

				case RomType_ExHiRom:
					headerBase = 0x410000 - 0x40;
					break;

				case RomType_LoRom:
				case RomType_HiRom:
				case RomType_Unknown:
				default:
					break;
			}
			break;

		case MapMode_Reserved:
		case MapMode_SPC7110:
		case MapMode_Unknown:
		default:
			break;
	}

	/* Generate snes header */
	raw[headerBase+0x15] = self->mapmode_get(self);
	raw[headerBase+0x17] = mb;
	write16(&raw[headerBase+0x1c], 0x0000);
	write16(&raw[headerBase+0x1e], 0xffff);
	self->pro->raw = raw;

	/* Set address convert methods */
	self->DetectRomType(self);

	/* calculate checksum & update */
	self->ChecksumUpdate(self);

	/* init search method */
	if(MapMode_Unknown != self->pro->map)
	{
		self->RatsSearch = RatsSearch;
		self->RatsClean = RatsClean;
	}

	return FileOpen_NoError;
}

static void dummyClose(RomFile* self)
{
	assert(self);
	free(self->pro->raw);
	self->pro->raw = NULL;
	self->pro->rom = NULL;

	self->Snes2PcAdr = NullSnesAdr;
	self->Pc2SnesAdr = NullSnesAdr;
	self->RatsSearch = RatsSearchFail;
	self->RatsClean = RatsCleanFalse;
}

static bool dummyReload(RomFile* self)
{
	assert(self);

	return false;
}

static bool dummyWrite(RomFile* self)
{
	assert(self);

	/* re-calculate checksum */
	return self->ChecksumUpdate(self);
}

/*=== Default methods ====================================*/
static uint32 NullSnesAdr(RomFile* self, const uint32 ad)
{
	return ROMADDRESS_NULL;
}
static uint32 RatsSearchFail(RomFile* self, const uint32 ad, RatsSearcher_t search)
{
	return ROMADDRESS_NULL;
}
static uint32 RatsCleanFalse(RomFile* self, const uint32 ad)
{
	return 0;
}

static uint8* memsearch(const uint8 *p1, const uint8 *p2, const uint32 len, const uint8* end)
{
	int match;
	for(; p1 <= end; p1++)
	{
		if(p1[0] == p2[0])
		{
			match = 1;
			while(p1[match] == p2[match])
			{
				match++;
				if(match == len)
				{
					return (uint8*)p1;
				}
			}
			p1 += match;
		}
	}
	return NULL;
}
static uint32 RatsSearch(RomFile* self, const uint32 sna, RatsSearcher_t search)
{
	uint32 adr;
	uint8* ptr;
	uint16 sz;
	uint16 szc;

	assert(self);

	adr = self->Snes2PcAdr(self, sna);
	if(ROMADDRESS_NULL == adr) return ROMADDRESS_NULL;

	ptr = &self->pro->rom[adr];

	for ( ; ptr != NULL ; )
	{
		/* Search rats tag */
		ptr = memsearch(ptr, (const uint8*)"STAR", 4, &self->pro->rom[self->pro->size]);
		if(NULL != ptr)
		{
			/* check valid rats tag */
			sz =  read16(ptr+4);
			szc = read16(ptr+6);
			if(sz == (szc^0xffff))
			{
				if(NULL == search || search(ptr+8, (uint32)(sz+1)))
				{
					return self->Pc2SnesAdr(self, (uint32)(ptr - self->pro->rom));
				}
			}

			/* unmatch */
			ptr += sz + 9;
		}
	}

	return ROMADDRESS_NULL;
}

static uint32 RatsClean(RomFile* self, const uint32 sna)
{
	uint32 adr;
	uint8* ptr;
	uint16 sz;
	uint16 szc;

	assert(self);

	/* Get rom address */
	adr = self->Snes2PcAdr(self, sna);
	if(ROMADDRESS_NULL == adr) return false;
	ptr = &self->pro->rom[adr];

	/* Check RATS tag */
	if(0 != memcmp(ptr, (const uint8*)"STAR", 4)) return false;

	sz =  read16(ptr+4);
	szc = read16(ptr+6);
	if(sz != (szc^0xffff)) return false;

	/* Fill data */
	sz = (uint16)(sz + 9);
	memset(ptr, FILL, sz);

	return (uint32)sz;
}
