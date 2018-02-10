/**
 * @file Export.c
 *   ... export library's labels
 */
#include "common/types.h"
#include "common/List.h"
#include "common/Str.h"
#include "common/Observer.h"
#include "common/srcpath.h"
#include "file/File.h"
#include "file/TextFile.h"
#include "mewthree/InsInfo.h"
#include "mewthree/MewEnv.h"
#include "dll/asardll.h"

#include "mewthree/Export.h"

/**
 * @brief Export labels
 *
 * @param labs asar's label data structure pointer
 * @param cnt labels count
 * @param ignores ignore label prefixs
 * @param env Environment
 * @param obs Observers
 */
void ExportLabels(const struct labeldata* labs, int cnt, const char* const* ignores, MewEnvStruct* env, Observers* obs)
{
	char* path;
	TextFile *f_trasm, *f_asar;
	int i;

	path = Str_concat(env->WorkDir, TrasmLibAsm);
	f_trasm = new_TextFile(path);
	free(path);
	f_trasm->Open2(f_trasm, "w");

	path = Str_concat(env->WorkDir, AsarLibAsm);
	f_asar = new_TextFile(path);
	free(path);
	f_asar->Open2(f_asar, "w");

	for(;0<cnt;cnt--)
	{
		/* omit labels */
		for(i=0; NULL!=ignores[i]; i++)
		{
			if(0 == strncmp(ignores[i], labs->name, strlen(ignores[i]))) goto Next;
		}

		/* trasm */
		f_trasm->Printf(f_trasm, "%s = $%06x\n", labs->name, labs->location);

		/* asar */
		f_asar->Printf(f_asar, "org $%06x\n", labs->location);
		f_asar->Printf(f_asar, "\t%s:\n", labs->name);

	Next:
		labs++;
	}

	f_trasm->super.Close(&f_trasm->super);
	f_asar->super.Close(&f_asar->super);
	delete_TextFile(&f_trasm);
	delete_TextFile(&f_asar);
}

void ExportFileMacro(const char* fname, int entry, MewEnvStruct* env, Observers* obs)
{
	char* path;
	TextFile *f_asar;

	path = Str_concat(env->WorkDir, AsarFNMacAsm);
	f_asar = new_TextFile(path);
	free(path);
	f_asar->Open2(f_asar, "a");

	f_asar->Printf(f_asar, "macro %s()\n", fname);
	f_asar->Printf(f_asar, "\tjsl $%06x\n", entry);
	f_asar->Printf(f_asar, "endmacro\n", fname);

	f_asar->super.Close(&f_asar->super);
	delete_TextFile(&f_asar);
}
