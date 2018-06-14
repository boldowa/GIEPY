#pragma once
/**
 * @file Asar.h
 */
#ifdef __cplusplus
extern "C" {
#endif

bool AssembleAsar(
	MewEnvStruct* env,
	Observers* obs,
	bool isPixiCompatible,
	int pass,
	const char* const path,
	const char* tmpname,
	RomFile* rom,
	RomMap* map,
	LibsInsertMan* lman,
	InsertInf *inf,
	bool (*siginj)(TextFile*, void*),
	bool (*priinj)(const char*, InsertInf* inf, void* data, MewEnvStruct*, Observers*),
	bool (*labinj)(const struct labeldata*, int, InsertInf* inf, void* data, MewEnvStruct*, Observers*),
	void* data);

#ifdef __cplusplus
} /*extern "C" */
#endif
