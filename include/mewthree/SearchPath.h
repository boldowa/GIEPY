#pragma once
/**
 * @file SearchPath.h
 */
#ifdef __cplusplus
extern "C" {
#endif

bool ConstructSearchPath(char** paths, const char* suffix, MewEnvStruct* env, Observers* obs);
void DestroySearchPath(char**);

#ifdef __cplusplus
} /*extern "C" */
#endif
