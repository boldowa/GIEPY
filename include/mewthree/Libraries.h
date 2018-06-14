#pragma once
/**
 * @file Libraries.h
 */
#ifdef __cplusplus
extern "C" {
#endif

bool InsertLibraries(
		RomFile* rom,
		RomMap* map,
		bool isPixiCompatible,
		int pass,
		List* labs,
		List* macros,
		LibsInsertMan* lman,
		MewEnvStruct* env,
		Observers* obs);

#ifdef __cplusplus
} /*extern "C" */
#endif
