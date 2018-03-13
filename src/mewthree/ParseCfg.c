/**
 * @file ParseCfg.c
 */
#include "common/types.h"
#include <setjmp.h>
#include <assert.h>
#include "common/Str.h"
#include "common/List.h"
#include "common/strres.h"
#include "common/Observer.h"
#include "common/Funex.h"
#include "common/InsertList.h"
#include "file/File.h"
#include "file/TextFile.h"
#include "file/libfile.h"
#include "mewthree/ParseCfg.h"

void DiscardCfgData(CfgData** cfgp, Observers* obs)
{
	CfgData *cfg;

	if(NULL == (*cfgp)) return;

	cfg = (*cfgp);
	free(cfg->basename);
	free(cfg->path);
	free(cfg->asm_name1);
	free(cfg->asm_name2);
	free(cfg->insinf1.path);
	free(cfg->insinf2.path);
	free(cfg->tag.name);
	free(cfg->tag.label);
	free(cfg->tag.description);
	delete_List(&cfg->tag.tiles);
	free(cfg);
	(*cfgp) = NULL;
}

static void RemoveComment(char* line)
{
	char* s;
	size_t i;
	size_t len;
	s = strstr(line, ";");
	if(NULL != s)
	{
		*s = '\0';
	}

	len = strlen(line);
	for(i = len-1; i>=0; i--)
	{
		if(false == IsSpace(line[i]))
		{
			break;
		}
		line[i] = '\0';
	}
}

static bool EmptyHandler(const char* line, void* data, Observers* obs)
{
	/* nothing to do, ignore line */
	return true;
}

static void UnEscape(char* str, size_t* len)
{
	static const char Escapes[] = {
		'[', ']'
	};
	static const size_t EscapesLen = sizeof(Escapes)/sizeof(char);
	size_t i;
	size_t iEsc;

	if(0<(*len))
	{
		for(i=0;i<(*len)-1;i++)
		{
			for(iEsc=0;iEsc<EscapesLen;iEsc++)
			{
				if('\\'==str[i] && Escapes[iEsc]==str[i+1])
				{
					/*strcpy_s(&str[i], (*len)-i, &str[i+1]);*/
					memcpy(&str[i], &str[i+1], (*len)-i+1);
					(*len)--;
					continue;
				}
			}
			if('\\'==str[i] && '\\'==str[i+1]) i++;
		}
	}
}


/**
 * @brief Read string data from line
 *
 * @param line cfg line data
 * @param data data pointer
 * @param obs observer
 *
 * @return handled (true) / unmatch description(false)
 */
static bool MatchStrSecCommon(const char* line, char** data, Observers* obs)
{
	char* work;
	char* newDesc;
	size_t len;

	len = strlen(line);
	work = malloc((len+3)*sizeof(char));
	assert(work);
	strcpy_s(work, len+3, line);
	len = strlen(work);

	UnEscape(work, &len);

	work[len+0] = '\\';
	work[len+1] = 'n';
	work[len+2] = '\0';

	if(NULL==(*data))
	{
		(*data) = work;
		return true;
	}
	newDesc = Str_concat((*data), work);
	free(work);
	if(NULL==newDesc)
	{
		obs->fatal(0, GSID_MEMALLOC_FAILED, __func__);
		return false;
	}

	free(*data);
	(*data) = newDesc;
	return true;
}

/**
 * @brief Read name data from line
 *
 * @param line cfg line data
 * @param data tag data pointer
 * @param obs observer
 *
 * @return match name, handled (true) / unmatch name(false)
 */
static bool MatchName(const char* line, void* data, Observers* obs)
{
	return MatchStrSecCommon(line, &((TagData*)data)->name, obs);
}

/**
 * @brief Read label data from line
 *
 * @param line cfg line data
 * @param data tag data pointer
 * @param obs observer
 *
 * @return match name, handled (true) / unmatch name(false)
 */
static bool MatchLabel(const char* line, void* data, Observers* obs)
{
	return MatchStrSecCommon(line, &((TagData*)data)->label, obs);
}

/**
 * @brief Read description data from line
 *
 * @param line cfg line data
 * @param data tag data pointer
 * @param obs observer
 *
 * @return match description, handled (true) / unmatch description(false)
 */
static bool MatchDescription(const char* line, void* data, Observers* obs)
{
	return MatchStrSecCommon(line, &((TagData*)data)->description, obs);
}

static bool MatchPosition(const char* line, void* data, Observers* obs)
{
	size_t i=0;
	size_t len;
	TagData* tag = (TagData*)data;

	len = strlen(line);
	SkipSpaces(line, &i, len);
	if(2 != sscanf(&line[i], "%d%*[^0123456789]%d", &tag->x, &tag->y)) return false;
	return true;
}

static bool MatchTiles(const char* line, void* data, Observers* obs)
{
	size_t i=0;
	size_t len;
	int* v;
	List* l = ((TagData*)data)->tiles;

	len = strlen(line);
	SkipSpaces(line, &i, len);
	v = malloc(3*sizeof(int));
	assert(v);
	if(3 != sscanf(&line[i], "%d%*[^0123456789-]%d%*[^0123456789abcdefABCDEF]%x", &v[0], &v[1], &v[2])) return false;
	l->push(l, v);
	return true;
}

/**
 * @brief Read section information from line
 *
 * @param line list line data
 * @param data data storage pointer
 * @param obs observer
 *
 * @return match list, handled (true) / unmatch list(false)
 */
static bool MatchSection(const char* line, void* data, Observers* obs)
{
	size_t len;
	size_t i;
	size_t beg;
	char* strSection;
	size_t strSectionLen;

	static const struct {
		const char* sec;
		FunexFuncObs_t func;
	} fn[] = {
		{ "Name", MatchName },
		{ "Label", MatchLabel },
		{ "Description", MatchDescription },
		{ "Position", MatchPosition },
		{ "Tiles", MatchTiles },
		/* --- */
		{ NULL, NULL }
	};
	FunexFuncObs_t* func = (FunexFuncObs_t*)data;

	/* get the section signature */
	len = strlen(line);
	i = 0;
	SkipSpaces(line, &i, len);
	if('[' != line[i])
	{
		return false;
	}
	beg = ++i;
	SkipUntilChar(line, &i, ']', len);
	if(i != len-1)
	{
		return false;
	}

	strSectionLen = i - beg;
	strSection = calloc(strSectionLen+1, sizeof(char));
	if(NULL == strSection)
	{
		obs->fatal(0, GSID_MEMALLOC_FAILED, __func__);
		return false;
	}
	strncpy_s(strSection, strSectionLen+1, &line[beg], strSectionLen);

	/* search section */
	for(i=0; fn[i].sec != NULL; i++)
	{
		if(0 == strcmp(fn[i].sec, strSection))
		{
			/* switch section */
			obs->debug(0, GSID_LIST_SECTION_MATCH, strSection);
			(*func) = fn[i].func;
			free(strSection);
			return true;
		}
	}

	/* matches section syntax, but couldn't find a section */
	obs->warn(0, GSID_PARSECFG_TAG_INVALID, strSection);
	free(strSection);
	return true;
}

static void chomp(char* s)
{
	size_t len;

	if(NULL==s) return;

	len = strlen(s);
	if(1<len)
	{
		s[len-1] = '\0';
		s[len-2] = '\0';
	}
}

enum {
	FunexMatch_Section = 0,
	FunexMatch_Data
};
static bool ReadTagData(CfgData* data, TextFile* cfgFile, Observers* obs)
{
	const char* linebuf;
	char* work=NULL;
	jmp_buf e;

	FunexObsStruct fs[] = {
		{ MatchSection, NULL },
		{ EmptyHandler, NULL },
		/* --- */
		{ NULL, NULL }
	};
	fs[0].match = &fs[1].func;
	fs[1].match = &data->tag;

	if(0 == setjmp(e))
	{
		linebuf = cfgFile->GetLine(cfgFile);
		while(NULL != linebuf)
		{
			work = Str_copy(linebuf);
			RemoveComment(work);

			/* parse list */
			switch(FunexMatchObs(work, fs, obs))
			{
				case FunexMatch_Section:
					break;
				case FunexMatch_Data:
					if(fs[1].func==MatchName || fs[1].func==MatchPosition) /* force 1line */
						fs[1].func = EmptyHandler;
					break;

				default:	/* Not handled... */
					obs->err(0, GSID_PARSECFG_SYNTAX_ERROR,
							cfgFile->super.path_get(&cfgFile->super),
							cfgFile->row_get(cfgFile));
					longjmp(e, 1);
			}

			/* next line */
			free(work); work=NULL;
			linebuf = cfgFile->GetLine(cfgFile);
		}
	}
	else
	{
		free(work);
		return false;
	}

	chomp(data->tag.name);
	chomp(data->tag.description);
	chomp(data->tag.label);

	return true;
}

void VerifyCfgData(CfgData* data)
{
	if(0 != data->new_code_flag)
	{
		if(5 != data->sprite_type)	/* scroller */
		{
			free(data->asm_name2);
			data->asm_name2 = NULL;
		}
	}
}

/**
 * @brief Clone tiles array
 *        This function is used in List.
 *
 * @param src cp source data
 *
 * @return Cloned data
 */
void* CloneTiles(const void* vsrc)
{
	int* dst;
	const int* src = (const int*)vsrc;

	dst = malloc(3*sizeof(int));
	assert(dst);

	memcpy(dst, src, 3*sizeof(int));

	return dst;
}

CfgData* ParseCfg(const char* cfgPath, int number, Observers* obs)
{
	CfgData* data;
	TextFile* cfgFile;
	const char* linebuf;
	char* work=NULL;
	jmp_buf e;
	uint lines;
	bool isPrevEmpty = false;
	bool enableTagData = false;

	data = calloc(1, sizeof(CfgData));
	assert(data);
	data->tag.x = 7;
	data->tag.y = 7;
	data->tag.tiles = new_List(CloneTiles, free);
	assert(data->tag.tiles);

	if(0 == setjmp(e)) /* try */
	{
		cfgFile = new_TextFile(cfgPath);
		if(FileOpen_NoError != cfgFile->Open(cfgFile))
		{
			obs->err(0, GSID_DEBUG);
			longjmp(e, 1);
		}

		obs->debug(0, GSID_PARSECFG_READING_FILE, cfgPath);

		/* read config file */
		linebuf = cfgFile->GetLine(cfgFile);
		while(NULL != linebuf)
		{
			if(true == isPrevEmpty)
			{
				if(0 == strncmp("---", linebuf, 3))
				{
					enableTagData = true;
					work=NULL;
					break;
				}
			}
			work = Str_copy(linebuf);
			RemoveComment(work);
			if('\0' == (*work)) isPrevEmpty = true;
			else                isPrevEmpty = false;

			switch(cfgFile->row_get(cfgFile))
			{
				case 1:		/* new_code_flag */
					if(1 != sscanf(work, "%2x", &data->new_code_flag))
					{
						obs->err(0, GSID_PARSECFG_SYNTAX_ERROR, cfgPath, cfgFile->row_get(cfgFile));
						longjmp(e, 1);
					}
					break;

				case 2:		/* orig_sprite_number */
					if(1 != sscanf(work, "%2x", &data->orig_sprite_number))
					{
						obs->err(0, GSID_PARSECFG_SYNTAX_ERROR, cfgPath, cfgFile->row_get(cfgFile));
						longjmp(e, 1);
					}
					break;

				case 3:		/* tweaks */
					if(6 != sscanf(work, "%2x %2x %2x %2x %2x %2x",
								&data->tweaks[0],
								&data->tweaks[1],
								&data->tweaks[2],
								&data->tweaks[3],
								&data->tweaks[4],
								&data->tweaks[5]))
					{
						obs->err(0, GSID_PARSECFG_SYNTAX_ERROR, cfgPath, cfgFile->row_get(cfgFile));
						longjmp(e, 1);
					}
					break;

				case 4:		/* extra_props */
					if(2 != sscanf(work, "%2x %2x",
								&data->extra_props[0],
								&data->extra_props[1]))
					{
						obs->err(0, GSID_PARSECFG_SYNTAX_ERROR, cfgPath, cfgFile->row_get(cfgFile));
						longjmp(e, 1);
					}
					break;

				case 5:		/* asm_name1 */
					if('\0' != work[0]) data->asm_name1 = Str_copy(work);
					break;

				case 6:		/* asm_type */
					/* GIEPY detects asm type automaticaly. this fiels is ignored. */
					break;

				case 7:		/* sprite_type */
					if(1 != sscanf(work, "%2x", &data->sprite_type))
					{
						obs->err(0, GSID_PARSECFG_SYNTAX_ERROR, cfgPath, cfgFile->row_get(cfgFile));
						longjmp(e, 1);
					}
					break;

				case 8:		/* unique_info */
					if(1 != sscanf(work, "%2x", &data->unique_info))
					{
						obs->err(0, GSID_PARSECFG_SYNTAX_ERROR, cfgPath, cfgFile->row_get(cfgFile));
						longjmp(e, 1);
					}
					break;

				case 9:		/* extra_bytes */
					if(1 != sscanf(work, "%2x", &data->extra_byte_nums))
					{
						obs->err(0, GSID_PARSECFG_SYNTAX_ERROR, cfgPath, cfgFile->row_get(cfgFile));
						longjmp(e, 1);
					}
					break;

				case 10:	/* asm_name2 */
					if('\0' != work[0]) data->asm_name2 = Str_copy(work);
					break;

				default:
					break;
			}

			/* next line */
			free(work);
			linebuf = cfgFile->GetLine(cfgFile);
		}

		/* line check */
		lines = cfgFile->row_get(cfgFile);
		if(enableTagData) lines-=2;
		if(5>lines)
		{
			obs->err(0, GSID_PARSECFG_FIELDS_TOO_FEW, cfgPath);
			longjmp(e, 1);
		}
		else if(7>lines)
		{
			/* old type cfg file. */
			if(data->new_code_flag)
			{
				if(0xc0>number || 0xdf<number)
				{
					obs->warn(0, GSID_PARSECFG_OLD_FORMAT_SP, cfgPath);
					data->sprite_type = 1;
				}
				else if(0xd0>number)
				{
					obs->warn(0, GSID_PARSECFG_OLD_FORMAT_SH, cfgPath);
					data->sprite_type = 2;
				}
				else
				{
					obs->warn(0, GSID_PARSECFG_OLD_FORMAT_GEN, cfgPath);
					data->sprite_type = 3;
				}
			}
		}

		if(enableTagData)
		{
			if(!ReadTagData(data, cfgFile, obs))
			{
				longjmp(e, 1);
			}
		}

		delete_TextFile(&cfgFile);
	}
	else /* catch */
	{
		delete_TextFile(&cfgFile);
		free(work);
		DiscardCfgData(&data, obs);
		return NULL;
	}

	/* verify */
	VerifyCfgData(data);

	return data;
}

