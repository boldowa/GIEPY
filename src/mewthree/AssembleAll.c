/**
 * @file AssembleAll.c
 */
#include "common/types.h"
#include <assert.h>
#include <setjmp.h>
#if !isWindows
#include <strings.h>
#endif
#include "common/Str.h"
#include "common/List.h"
#include "common/ReadWrite.h"
#include "common/srcpath.h"
#include "common/strres.h"
#include "common/Observer.h"
#include "common/InsertList.h"
#include "common/Funex.h"
#include "file/libfile.h"
#include "file/FilePath.h"
#include "file/File.h"
#include "file/RomFile.h"
#include "file/TextFile.h"
#include "mewthree/Signature.h"
#include "mewthree/RomMap.h"
#include "mewthree/MewEnv.h"
#include "mewthree/InsInfo.h"
#include "mewthree/LibsInsertMan.h"
#include "mewthree/ParseCfg.h"
#include "mewthree/Json.h"
#include "mewthree/SearchPath.h"
#include "mewthree/ChkAsmType.h"
#include "mewthree/Trasm.h"
#include "dll/asardll.h"
#include "mewthree/Asar.h"

#include "mewthree/AssembleAll.h"

static bool IsAlreadyReadCfg(const void* sval, const void* lval)
{
	char* s = (char*)sval;
	InsertItem* lItem = (InsertItem*)lval;

	if(NULL != lItem->cfg)
	{
		if(NULL != lItem->cfg->basename)
			if(0 == strcmp(lItem->cfg->basename, s)) return true;
	}
	return false;
}
static bool IsInserted(const void* sval, const void* lval)
{
	char* s = (char*)sval;
	InsertItem* lItem = (InsertItem*)lval;

	if(NULL != lItem->cfg)
	{
		if(NULL != lItem->cfg->asm_name1)
			if(0 == strcmp(lItem->cfg->asm_name1, s)) return true;
		if(NULL != lItem->cfg->asm_name2)
			if(0 == strcmp(lItem->cfg->asm_name2, s)) return true;
	}
	if(0 == strcmp(lItem->fname, s)) return true;

	return false;
}
static CfgData* ReadCfgSub(char* const * dirs, const char* cfgname, int number, Observers* obs)
{
	int i;
	char* tmpPath = NULL;
	char* cfgPath = NULL;
	FilePath* path;
	CfgData* cfg;

	/* search object asm */
	for(i=0; NULL != dirs[i]; i++)
	{
		free(tmpPath);
		tmpPath = Str_concat(dirs[i], cfgname);
		if(fexists(tmpPath))
		{
			cfgPath = tmpPath;
			break;
		}
	}
	if(NULL == cfgPath)	/* not found */
	{
		/* try to open with absolute path */
		free(tmpPath);
		cfgPath = Str_copy(cfgname);
		if(!fexists(cfgPath))
		{
			/* cfg not found */
			obs->err(0, GSID_ASSEMBLE_CFG_NOT_FOUND, cfgname);
			free(cfgPath);
			return NULL;
		}
	}

	/* parse sprite config */
	path = new_FilePath(cfgPath);
	if(0 == strncasecmp(".json", path->ext_get(path), 5))
	{
		cfg = ParseJson(cfgPath, number, obs);
	}
	else
	{
		cfg = ParseCfg(cfgPath, number, obs);
	}
	delete_FilePath(&path);

	if(NULL != cfg)
	{
		cfg->path = cfgPath;
		cfg->basename = Str_copy(cfgname);
	}
	return cfg;
}
static bool CreateSearchPath(char** dirs, const char* dir, MewEnvStruct* env, Observers* obs)
{
	size_t len;
	char* path;
	len = strlen(dir);
	path = calloc(len+2, sizeof(char));
	assert(path);
	strcpy_s(path, len+2, dir);
#if isWindows
	if(('\\' != path[len-1]) || ('/' != path[len-1]))
	{
		path[len] = '\\';
		path[len+1] = '\0';
	}
#else
	if('/' != path[len-1])
	{
		path[len] = '/';
		path[len+1] = '\0';
	}
#endif
	if(false == ConstructSearchPath(dirs, path, env, obs))
	{
		return false;
	}
	free(path);

	return true;
}
static CfgData* ReadCfg(const char* dir, int number, const char* cfgname, MewEnvStruct* env, Observers* obs)
{
	char* dirs[SPMAX];
	CfgData* cfg;

	if(false == CreateSearchPath(dirs, dir, env, obs))
	{
		return NULL;
	}
	cfg = ReadCfgSub(dirs, cfgname, number, obs);
	DestroySearchPath(dirs);

	return cfg;
}

typedef struct _numinfo{
	int		grp;
	int		num;
	SpriteType	stype;
	uint8		spsubtype;
	uint8		exblen;
}numinfo;
static bool sigInjection_t(TextFile* f, void* d)
{
	numinfo* ni = (numinfo*)d;

	f->Printf(f, "g_sprite_group = $%02x\n",ni->grp);
	f->Printf(f, "g_sprite_number = $%02x\n",ni->num);
	f->Printf(f, "g_sprite_type = $%02x\n",ni->stype);
	f->Printf(f, "g_sprite_subtype = $%02x\n",ni->spsubtype);
	f->Printf(f, "dcb " SpSignature_db "\n");
	return true;
}
static bool sigInjection_a(TextFile* f, void* d)
{
	numinfo* ni = (numinfo*)d;

	f->Printf(f, "!DEBUG := 0\n");
	f->Printf(f, "!SYSTEM_INITIALIZING := 0\n");
	f->Printf(f, "!LIBRARY_ASSEMBLING := 0\n");
	f->Printf(f, "!g_sprite_group := $%02x\n",ni->grp);
	f->Printf(f, "!g_sprite_number := $%02x\n",ni->num);
	f->Printf(f, "!g_sprite_type := $%02x\n",ni->stype);
	f->Printf(f, "!g_sprite_subtype := $%02x\n",ni->spsubtype);
	f->Printf(f, "!g_extra_bytes_len := $%02x\n",ni->exblen);
	f->Printf(f, "freecode\n");
	f->Printf(f, "db " SpSignature_db "\n");
	return true;
}
static bool MatchLocCommon(const char* line, uint32* ip, Observers* obs, const char* identifier)
{
	size_t i;
	size_t len;

	if(0 == strncasecmp(identifier, line, 4))
	{
		len = strlen(line);
		i = 4;
		SkipSpaces(line, &i, len);
		if(len <= i) return false;

		if(1 == sscanf(&line[i], "%6x", ip))
		{
			return true;
		}
	}
	return false;
}
static bool MatchInit(const char* line, void* data, Observers* obs)
{
	return MatchLocCommon(line, (uint32*)data, obs, "INIT");
}
static bool MatchMain(const char* line, void* data, Observers* obs)
{
	return MatchLocCommon(line, (uint32*)data, obs, "MAIN");
}
bool printInjection(const char* print, InsertInf* inf, void* d, MewEnvStruct* env, Observers* obs)
{
	static uint32 initloc;
	static uint32 mainloc;
	static FunexObsStruct fs[] = {
		{ MatchInit, &initloc },
		{ MatchMain, &mainloc },
		/* --- */
		{ NULL, NULL  }
	};
	enum {
		Match_Init = 0,
		Match_Main,
		Match_Export
	};

	switch(FunexMatchObs(print, fs, obs))
	{
		case Match_Init:
			inf->initloc = initloc;
			return true;

		case Match_Main:
			inf->mainloc = mainloc;
			return true;

		default:
			break;
	}

	return false;
}
bool labInjection(const struct labeldata* labs, int cnt, InsertInf* inf, void* d, MewEnvStruct* env, Observers* obs)
{
	bool mainfound = false;
	/* check main label location */
	for(;0<cnt;cnt--)
	{
		if(0 == strcmp("__main__", labs->name))
		{
			if((labs->location & 0x7fffff) < 0x108000)
			{
				obs->err(0, GSID_ASSEMBLE_EMPTY_ASM);
				return false;
			}
			mainfound = true;
		}
		else if(0 == strcasecmp("init", labs->name))
		{
			if(0 == inf->initloc) inf->initloc = (uint32)labs->location;
		}
		else if(0 == strcasecmp("main", labs->name))
		{
			if(0 == inf->mainloc) inf->mainloc = (uint32)labs->location;
		}
		labs++;
	}
	return mainfound;
}
bool InsertAsm(RomFile* rom, RomMap* map, bool isPixiCompatible, int grp, int num, SpriteType stype, uint8 spsubtype, uint8 exblen, const char* dir, const char* name, InsertInf* inf, LibsInsertMan* lman, MewEnvStruct* env, Observers* obs)
{
	bool result = false;
	char* path = NULL;
	numinfo ni = {grp, num, stype, spsubtype, exblen};

	path = Str_concat(dir, name);
	assert(path);

	switch(ChkAsmType(path, obs))
	{
		case AsmType_ASAR:
			obs->debug(0, GSID_ASSEMBLE_USING_ASAR, path);
			result = AssembleAsar(
					env, obs, isPixiCompatible,
					5,	/* MAX: 5 pass */
					path, "tmpspr_a.asm",
					rom, map, lman,
					inf,
					sigInjection_a,
					printInjection,
					labInjection,
					&ni);
			break;

		case AsmType_TRASM:
			obs->debug(0, GSID_ASSEMBLE_USING_TRASM, path);
			result = AssembleTrasm(
					env, obs,
					path, "tmpspr_t.asm",
					rom, map,
					inf,
					sigInjection_t,
					&ni);
			break;

		default:
			/* can't assemble this asm */
			break;
	}

	free(path);
	return result;
}
static bool SetLocFromPrevious(InsertInf* inf, const char* name, InsertItem* prevItem)
{
	if(NULL != prevItem->cfg)
	{
		if(0 == strcmp(prevItem->cfg->asm_name1, name))
		{
			inf->insloc = prevItem->cfg->insinf1.insloc;
			inf->initloc = prevItem->cfg->insinf1.initloc;
			inf->mainloc = prevItem->cfg->insinf1.mainloc;
			return true;
		}
		if(0 == strcmp(prevItem->cfg->asm_name2, name))
		{
			inf->insloc = prevItem->cfg->insinf2.insloc;
			inf->initloc = prevItem->cfg->insinf2.initloc;
			inf->mainloc = prevItem->cfg->insinf2.mainloc;
			return true;
		}
	}
	if(0 == strcmp(prevItem->fname, name))
	{
		inf->insloc = prevItem->insinf.insloc;
		inf->initloc = prevItem->insinf.initloc;
		inf->mainloc = prevItem->insinf.mainloc;
		return true;
	}

	return false;
}
char* FindAsmDir(const char* sp, const char* name, MewEnvStruct* env, Observers* obs)
{
	char* dirs[SPMAX];
	char* path = NULL;
	char* tmpPath = NULL;
	int i;

	if(false == CreateSearchPath(dirs, sp, env, obs))
	{
		return NULL;
	}

	/* search asm */
	for(i=0; NULL!=dirs[i]; i++)
	{
		free(tmpPath);
		tmpPath = Str_concat(dirs[i], name);
		if(fexists(tmpPath))
		{
			free(tmpPath);
			path = Str_copy(dirs[i]);
			break;
		}
	}
	if(NULL == path) /* not found */
	{
		/* try to open with absolute path */
		free(tmpPath);
		path = Str_copy("");
		if(!fexists(name))
		{
			/* asm not found */
			free(path);
			path = NULL;
		}
	}
	DestroySearchPath(dirs);

	return path;
}
bool AssembleSub(
		RomFile* rom, RomMap* map, bool isPixiCompatible,
		const char* dir,
		List* list, LibsInsertMan* lman,
		MewEnvStruct* env, Observers* obs,
		Iterator* it,
		InsertItem* item,
		uint8 spsubtype,
		uint8 exblen,
		const char* name, InsertInf* in)
{
	Iterator* isearch;

	/* search previous insert */
	isearch = list->searchex(list, name, IsInserted, it->prev(it), true);
	if(NULL != isearch)
	{
		/* code is already inserted,           */
		/* so set location from previous code. */
		return SetLocFromPrevious(in, name, (InsertItem*)isearch->data(isearch));
	}
	/* insert new assembly code */
	return InsertAsm(rom, map, isPixiCompatible, item->grp, item->number, item->type, spsubtype, exblen, dir, name, in, lman, env, obs);
}
bool AssembleAll(RomFile* rom, RomMap* map, srcdirs* dirs, List* list, LibsInsertMan* lman, MewInsInfo* inf, MewEnvStruct* env, Observers* obs)
{
	Iterator* it;
	Iterator* itcfg;
	InsertItem* item;
	FilePath* fp;
	char* realDir = NULL;
	jmp_buf e;
	char* asmdir = NULL;
	const char* sp = NULL;
	uint ctr = 0;

	obs->progress(0, (uint)list->length(list));
	for(it=list->begin(list);NULL!=it;it=it->next(it))
	{
		item = (InsertItem*)it->data(it);

		if(SpriteType_Sprite == item->type)
		{
			CfgData* cfg;

			/* search previous read */
			itcfg = list->searchex(list, item->fname, IsAlreadyReadCfg, it->prev(it), true);
			if(NULL != itcfg)
			{
				/* copy cfg data from previous */
				CfgData* src = ((InsertItem*)itcfg->data(itcfg))->cfg;

				cfg = malloc(sizeof(CfgData));
				assert(cfg);
				memcpy(cfg, src, sizeof(CfgData));
				cfg->basename = Str_copy(src->basename);
				cfg->path = Str_copy(src->path);
				cfg->asm_name1 = Str_copy(src->asm_name1);
				cfg->asm_name2 = Str_copy(src->asm_name2);
				cfg->insinf1.path = Str_copy(src->insinf1.path);
				cfg->insinf2.path = Str_copy(src->insinf2.path);

				item->cfg = cfg;

				obs->progress(++ctr, (uint)list->length(list));
				continue;
			}

			/* read new cfg data */
			cfg = ReadCfg(dirs->sprite, item->number, item->fname, env, obs);
			if(NULL == cfg)
			{
				obs->err(0, GSID_ASSEMBLE_CFG_READ_ERROR, item->fname);
				return false;
			}

			if(false == inf->giepyIsExtBytesCode)
			{
				cfg->extra_byte_nums = 0;
			}
	
			item->cfg = cfg;

			/* insert asm */
			if(cfg->new_code_flag)
			{
				fp = new_FilePath(cfg->path);
				realDir = abspath(fp->dir_get(fp));
				if(0 == setjmp(e)) /* try */
				{
					if(NULL != cfg->asm_name1)
					{
						if(false == AssembleSub(
									rom, map, inf->giepyIsPixiCompatible,
									realDir,
									list, lman,
									env, obs,
									it, item, cfg->sprite_type, cfg->extra_byte_nums,
									cfg->asm_name1, &cfg->insinf1
						  ))
						{
							longjmp(e, 1);
						}
					}
					else
					{
						cfg->insinf1.insloc = read24(inf->giepySpriteNopAdrPtr);
						cfg->insinf1.initloc = cfg->insinf1.insloc;
						cfg->insinf1.mainloc = cfg->insinf1.insloc;
					}

					if(NULL != cfg->asm_name2)
					{
						if(false == AssembleSub(
									rom, map, inf->giepyIsPixiCompatible,
									realDir,
									list, lman,
									env, obs,
									it, item, cfg->sprite_type, cfg->extra_byte_nums,
									cfg->asm_name2, &cfg->insinf2
						  ))
						{
							longjmp(e, 1);
						}
					}
					else
					{
						cfg->insinf2.insloc = read24(inf->giepySpriteNopAdrPtr);
						cfg->insinf2.initloc = cfg->insinf2.insloc;
						cfg->insinf2.mainloc = cfg->insinf2.insloc;
					}
				}
				else /* catch */
				{
					obs->err(0, GSID_ASSEMBLE_CFG_PROC_FAILED, fp->path_get(fp));
					free(realDir);
					delete_FilePath(&fp);
					return false;
				}
				delete_FilePath(&fp);
				free(realDir);
			} /* cfg->new_code_flag */
		} /* SpriteType_Sprite == item->type */
		else
		{
			switch(item->type)
			{
				case SpriteType_Extended:
					sp = dirs->extended;
					break;

				case SpriteType_Cluster:
					sp = dirs->cluster;
					break;

				case SpriteType_Overworld:
					sp = dirs->overworld;
					break;

				default:
					return false;
			}
			asmdir = FindAsmDir(sp, item->fname, env, obs);
			if(NULL == asmdir)
			{
				obs->err(0, GSID_ASSEMBLE_ASM_NOT_FOUND, item->fname);
				return false;
			}
			if(0 == setjmp(e))
			{
				if(false == AssembleSub(
							rom, map, inf->giepyIsPixiCompatible,
							asmdir,
							list, lman,
							env, obs,
							it, item, 0, 0,
							item->fname, &item->insinf
						       ))
				{
					longjmp(e, 1);
				}
			}
			else
			{
				free(asmdir);
				return false;
			}
		}
		obs->progress(++ctr, (uint)list->length(list));
	}

	return true;
}

