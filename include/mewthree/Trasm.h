#pragma once
/**
 * @file Trasm.h
 */
#ifdef __cplusplus
extern "C" {
#endif

bool AssembleTrasm(
	MewEnvStruct* env,
	Observers* obs,
	const char* const path,
	const char* tmpname,
	RomFile* rom,
	RomMap* map,
	InsertInf *inf,
	bool (*siginj)(TextFile*, void*),
	void* data);

#ifdef __cplusplus
} /*extern "C" */
#endif
