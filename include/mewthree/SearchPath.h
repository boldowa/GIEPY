#pragma once
/**
 * @file SearchPath.h
 */

bool ConstructSearchPath(char** paths, const char* suffix, MewEnvStruct* env, Observers* obs);
void DestroySearchPath(char**);

