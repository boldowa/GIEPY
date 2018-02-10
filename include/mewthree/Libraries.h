#pragma once
/**
 * @file Libraries.h
 */

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

