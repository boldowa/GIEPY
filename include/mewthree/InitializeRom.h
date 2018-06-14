#pragma once
/**
 * @file InitializeRom.h
 */
#ifdef __cplusplus
extern "C" {
#endif

bool InitializeRom(RomFile* rom, RomMap* map, LibsInsertMan* lman, MewInsInfo* inf, bool exb, bool force, bool pixi, MewEnvStruct* env, Observers* obs);

#ifdef __cplusplus
} /*extern "C" */
#endif
