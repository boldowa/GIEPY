#pragma once
/**
 * @file Export.h
 */

void ExportLabels(const struct labeldata* labs, int cnt, const char* const* ignores, MewEnvStruct* env, Observers* obs);
void ExportFileMacro(const char* fname, int entry, MewEnvStruct* env, Observers* obs);


