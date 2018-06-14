#pragma once
/**
 * @file ExtClsSpr.h
 */
#ifdef __cplusplus
extern "C" {
#endif

bool InsertExtended(RomFile* rom, MewInsInfo* inf, List* iList, MewEnvStruct* env, Observers* obs);
bool InsertCluster(RomFile* rom, MewInsInfo* inf, List* iList, MewEnvStruct* env, Observers* obs);

#ifdef __cplusplus
} /*extern "C" */
#endif
