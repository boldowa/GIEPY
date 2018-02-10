#pragma once
/**
 * @file CollectInfo.h
 */

#include "mewthree/InsInfo.h"

#define IFLG_SA1		0x0001
#define IFLG_EXTRA_BYTES	0x0002
#define IFLG_PIXI_COMPATIBLE	0x0004

bool CollectInfo(RomFile* rom, MewInsInfo* inf, Observers* obs);


