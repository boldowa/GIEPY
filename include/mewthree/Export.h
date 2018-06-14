#pragma once
/**
 * @file Export.h
 */
#ifdef __cplusplus
extern "C" {
#endif

void ExportLabels(const struct labeldata* labs, int cnt, const char* const* ignores, MewEnvStruct* env, Observers* obs);
void ExportFileMacro(const char* fname, int entry, MewEnvStruct* env, Observers* obs);

#ifdef __cplusplus
} /*extern "C" */
#endif
