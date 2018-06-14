#pragma once
/**
 * @file Sprites.h
 */
#ifdef __cplusplus
extern "C" {
#endif

bool InsertSprites(RomFile* rom, MewInsInfo* inf, List* iList, bool disableSscGen, MewEnvStruct* env, Observers* obs);

#ifdef __cplusplus
} /*extern "C" */
#endif
