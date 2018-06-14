#pragma once
/**
 * @file AssembleAll.h
 */
#ifdef __cplusplus
extern "C" {
#endif

bool AssembleAll(RomFile* rom, RomMap* map, srcdirs* dirs, List* list, LibsInsertMan* lman, MewInsInfo* inf, MewEnvStruct* env, Observers* obs);

#ifdef __cplusplus
} /*extern "C" */
#endif
