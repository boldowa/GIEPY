/**
 * @file Workspace.c
 */
#include "common/types.h"
#include "common/List.h"
#include "common/ReadWrite.h"
#include "common/defines.h"
#include "common/srcpath.h"
#include <assert.h>
#include "common/Str.h"
#include "common/Observer.h"
#include "file/File.h"
#include "file/TextFile.h"
#include "mewthree/MewEnv.h"
#include "mewthree/InsInfo.h"
#include "mewthree/Workspace.h"

typedef enum {
	type_trasm = 0,
	type_asar
}asmtype;

static void createEmptyFile(const char* const dir, const char* const name)
{
	char* path;

	path = Str_concat(dir, name);
	fclose(fopen(path, "w"));
	free(path);
}
static void createDefineFile(const char* const dir, const char* const name, const asmtype type, List* defs)
{
	char* path;
	TextFile *file;
	Define* def;
	Iterator* it;

	path = Str_concat(dir, name);
	file = new_TextFile(path);
	free(path);
	assert(file);

	file->Open2(file, "w");
	switch(type)
	{
		case type_trasm:
			for(it=defs->begin(defs); it != NULL; it=it->next(it))
			{
				def = it->data(it);
				file->Printf(file, "%s = %s\n", def->name, def->val);
			}
			break;

		case type_asar:
			for(it=defs->begin(defs); it != NULL; it=it->next(it))
			{
				def = it->data(it);
				file->Printf(file, "!%s = %s\n", def->name, def->val);
			}
			break;

		default:
			break;
	}
	file->super.Close(&file->super);
	delete_TextFile(&file);
}

void InitializeWorkspace(MewEnvStruct* env, MewInsInfo* inf, List* defs)
{
	createDefineFile(env->WorkDir, AsarDefAsm, type_asar, defs);
	createDefineFile(env->WorkDir, TrasmDefAsm, type_trasm, defs);
	createEmptyFile(env->WorkDir, AsarLibAsm);
	createEmptyFile(env->WorkDir, AsarFNMacAsm);
	createEmptyFile(env->WorkDir, TrasmLibAsm);
}

#ifdef DEBUG
#  define deleteFile(a,b) 
#else
static void deleteFile(const char* const dir, const char* const name)
{
	char* path;

	path = Str_concat(dir, name);
	remove(path);
	free(path);
}
#endif

void CleanWorkspace(MewEnvStruct* env)
{
	deleteFile(env->WorkDir, AsarDefAsm);
	deleteFile(env->WorkDir, AsarLibAsm);
	deleteFile(env->WorkDir, AsarFNMacAsm);
	deleteFile(env->WorkDir, TrasmDefAsm);
	deleteFile(env->WorkDir, TrasmLibAsm);
}

