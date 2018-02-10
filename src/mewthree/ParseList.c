/**
 * @file ParseList.c
 */
#include "common/types.h"
#include <setjmp.h>
#include <assert.h>
#include "common/Str.h"
#include "common/strres.h"
#include "common/Observer.h"
#include "common/Funex.h"
#include "common/InsertList.h"
#include "file/File.h"
#include "file/TextFile.h"
#include "file/libfile.h"
#include "common/List.h"
#include "mewthree/MewEnv.h"
#include "mewthree/ParseCfg.h"
#include "mewthree/ParseList.h"

/**
 * @brief Remove comments from input string
 *
 * @param line string
 */
static void RemoveComment(char* line)
{
	char* s;
	size_t i;
	size_t len;
	s = strstr(line, "//");
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

static bool CheckConflict(const void* sval, const void* lval)
{
	InsertItem *inew = (InsertItem*)sval;
	InsertItem *ilist = (InsertItem*)lval;

	if(inew->grp != ilist->grp) return false;
	if(inew->number != ilist->number) return false;

	return true;
}

static bool MatchRemarks(const char* line, void* data, Observers* obs)
{
	/* nothing to do, ignore line */
	return true;
}

static bool MatchAsmTypeSpr(const char* line, List* list, Observers* obs, SpriteType stype, uint min, uint max)
{
	size_t len;
	size_t i;
	char* asmstr;
	size_t asmlen;
	uint32 hex = 0;
	InsertItem* item;
	Iterator* it;

	len = strlen(line);
	i = 0;

	SkipSpaces(line, &i, len);

	/* get index */
	if(false == ishex(line[i]))
	{
		return false;
	}
	for(; i<len; i++)
	{
		if(false == ishex(line[i])) break;
		hex <<= 4;
		hex = hex + (uint32)atoh(line[i]);
	}
	if(max<hex)
	{
		obs->err(0, GSID_LIST_INSERT_NUM_LARGE, max);
		return false;
	}
	if(min>hex)
	{
		obs->err(0, GSID_LIST_INSERT_NUM_FEW, min);
		return false;
	}

	SkipSpaces(line, &i, len);

	/* filename */
	asmlen = strlen(&line[i]);
	if(0 == asmlen)
	{
		/* number only */
		return true;
	}
	asmstr = calloc(asmlen+1, sizeof(char));
	assert(asmstr);
	strcpy_s(asmstr, asmlen+1, &line[i]);

	/* create item */
	item = calloc(1, sizeof(InsertItem));
	assert(item);
	item->type = stype;
	item->grp = 0;
	item->number = (uint8)hex;
	item->fname = asmstr;
	it = list->search(list, item, CheckConflict);
	if(NULL != it)
	{
		InsertItem* c;
		c = it->data(it);
		obs->err(0, GSID_LIST_INSERT_CONFLICT, c->number, c->grp, c->fname);
		/* insert conflicted */
		free(item->fname);
		free(item);
		return false;
	}
	list->push(list, item);

	return true;
}

/**
 * @brief Read cluster sprite list information from line
 *
 * @param line list line data
 * @param data data storage pointer
 * @param obs observer
 *
 * @return match list, handled (true) / unmatch list(false)
 */
static bool MatchOverworld(const char* line, void* data, Observers* obs)
{
	/* nothing to do, ignore line */
	return true;

	/* return MatchAsmTypeSpr(line, (List*)data, obs, SpriteType_Overworld); */
}

/**
 * @brief Read extended sprite list information from line
 *
 * @param line list line data
 * @param data data storage pointer
 * @param obs observer
 *
 * @return match list, handled (true) / unmatch list(false)
 */
static bool MatchExtended(const char* line, void* data, Observers* obs)
{
	return MatchAsmTypeSpr(line, (List*)data, obs, SpriteType_Extended, 19, 255);
}

/**
 * @brief Read cluster sprite list information from line
 *
 * @param line list line data
 * @param data data storage pointer
 * @param obs observer
 *
 * @return match list, handled (true) / unmatch list(false)
 */
static bool MatchCluster(const char* line, void* data, Observers* obs)
{
	return MatchAsmTypeSpr(line, (List*)data, obs, SpriteType_Cluster, 9, 255);
}

/**
 * @brief get grp
 
 * @param str
 * @param isInsert
 *
 * @return 
 */
static bool GetGroupInfo(const char* str, bool isInsert[4])
{
	size_t len;
	size_t i;
	int beg, end;

	len = strlen(str);
	i = 0;

	if('0' > str[i] || '3' < str[i])
	{
		/* invalid number */
		return false;
	}
	beg = str[i++]-'0';
	SkipSpaces(str, &i, len);
	if(len <= i)
	{
		isInsert[beg] = true;
	}

	if(',' == str[i])
	{
		i++;
		SkipSpaces(str, &i, len);
		if(false == GetGroupInfo(&str[i],isInsert))
		{
			return false;
		}
		isInsert[beg] = true;
	}
	else if('-' == str[i])
	{
		int inx;

		i++;
		SkipSpaces(str, &i, len);
		if(len <= i)
		{
			/* not found end grp */
			return false;
		}

		if('0' > str[i] || '3' < str[i])
		{
			return false;
		}
		end = str[i++]-'0';

		if(beg>end)
		{
			/* invalid specify */
			return false;
		}

		for(inx=beg; inx<=end; inx++)
		{
			isInsert[inx] = true;
		}
	}

	return true;
}
/**
 * @brief get insertion information of sprite
 *
 * @param str insertion destination information string
 * @param isInsert whether to insert into a group
 * @param insertInx insertion destination number
 * @param obs observer
 *
 * @return handled(true) / unmatch(false)
 */
static bool GetInsertInfo(const char* str, bool isInsert[4], uint* insertInx, Observers* obs)
{
	size_t len;
	size_t i;
	char* work;
	uint32 hex = 0;

	for(i=0; i<4; i++)
	{
		isInsert[i] = false;
	}

	work = Str_copy(str);
	len = strlen(work);
	i = 0;

	/* group specify search */
	SkipUntilChar(work, &i, ':', len);
	if(len != i)
	{
		work[i++] = '\0';
		if(false == GetGroupInfo(&work[i], isInsert))
		{
			/* unknown group */
			free(work);
			return false;
		}
	}
	else
	{
		/* Insert group 2 and 3 if there is no group information */
		/* (for sprite tool / PIXI compatible) */
		isInsert[0] = false;
		isInsert[1] = false;
		isInsert[2] = true;
		isInsert[3] = true;
	}
	i = 0;	/* rewind */
	SkipSpaces(work, &i, len);
	if(len<=i)
	{
		/* index hex isn't specified */
		free(work);
		return false;
	}
	if(false == ishex(work[i]))
	{
		/* invalid hex */
		free(work);
		return false;
	}
	for(; i<len; i++)
	{
		if(false == ishex(work[i])) break;
		hex <<= 4;
		hex = hex + (uint32)atoh(work[i]);
	}
	if(255<hex)
	{
		free(work);
		obs->err(0, GSID_LIST_INSERT_NUM_LARGE, 255);
		return false;
	}
	(*insertInx) = hex;

	return true;
}

/**
 * @brief Read sprite list information from line
 *
 * @param line list line data
 * @param data data storage pointer
 * @param obs observer
 *
 * @return match list, handled (true) / unmatch list(false)
 */
static bool MatchSprites(const char* line, void* data, Observers* obs)
{
	size_t len;
	size_t i;
	char* strasm;
	char* work;
	size_t strasmlen;
	char* strinx;
	List* lst = (List*)data;	/* InsertItem list */
	InsertItem* item;
	bool isInsert[4];
	uint insertInx;
	Iterator* it;

	work = Str_copy(line);
	len = strlen(work);
	i = 0;

	/* get index beg column */
	SkipSpaces(work, &i, len);
	strinx = &work[i];

	SkipUntilSpaces(work, &i, len);
	if(len == i)
	{
		/* number only */
		free(work);
		return true;
	}
	work[i++] = '\0';

	/* get cfg name ptr */
	SkipSpaces(work, &i, len);
	strasmlen = strlen(&work[i]);
	if(0 == strasmlen)
	{
		/* number only */
		free(work);
		return true;
	}

	/* get insert index info */
	if(false == GetInsertInfo(strinx, isInsert, &insertInx, obs))
	{
		/* unknown number */
		free(work);
		return false;
	}

	/* get asm name */
	strasm = &work[i];

	/* add insert list */
	for(i=0; i<4; i++)
	{
		if(isInsert[i])
		{
			item = calloc(1, sizeof(InsertItem));
			assert(item);
			item->type = SpriteType_Sprite;
			item->grp = (uint8)i;
			item->number = (uint8)insertInx;
			item->fname = Str_copy(strasm);
			it = lst->search(lst, item, CheckConflict);
			if(NULL != it)
			{
				/* insert conflicted */
				InsertItem* c;
				c = it->data(it);
				obs->err(0, GSID_LIST_INSERT_CONFLICT, c->number, c->grp, c->fname);
				free(item->fname);
				free(item);
				free(work);
				return false;
			}
			lst->push(lst, item);
		}
	}

	free(work);
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
		{ "Sprite", MatchSprites },
		{ "Extended", MatchExtended },
		{ "Cluster", MatchCluster },
		{ "Overworld", MatchOverworld },
		{ "Remarks", MatchRemarks },
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
	obs->warn(0, GSID_LIST_INVALID_SECTION, strSection);
	free(strSection);
	return true;
}

/**
 * @brief Delete insert list
 *        *** This function is called from List destructor.
 *
 * @param data InsertItem pointer
 */
static void DeleteListItem(void* data)
{
	InsertItem *item = (InsertItem*)data;
	assert(item);

	free(item->fname);
	free(item->insinf.path);
	DiscardCfgData(&item->cfg, NULL);
	free(item);
}

enum {
	FunexMatch_Section = 0,
	FunexMatch_List
};
/**
 * @brief Parse sprites list. It's stored the information in List that is 
 *
 * @param listName list file name (or path)
 * @param listPtr Pointer for insert list
 * @param env Environment
 * @param obs Observer
 *
 * @return succeeded(true) / failure(false)
 */
bool ParseList(const char* listName, List** listPtr, MewEnvStruct* env, Observers* obs)
{
	List* list = NULL;	/* InsertItem List */
	char* tmpPath = NULL;
	char* lstPath = NULL;
	TextFile* lstFile;
	const char* linebuf;
	char* work;
	jmp_buf e;
	int i;

	FunexObsStruct fs[] = {
		{ MatchSection, NULL },
		{ MatchSprites, NULL },
		/* --- */
		{ NULL, NULL }
	};
	fs[0].match = &fs[1].func;

	/* create list */
	list = new_List(NULL, DeleteListItem);
	if(NULL == list)
	{
		obs->fatal(0, GSID_MEMALLOC_FAILED, __func__);
		return false;
	}
	fs[1].match = list;

	/* search list file */
	for(i=0; NULL != env->SearchPath[i]; i++)
	{
		free(tmpPath);
		tmpPath = Str_concat(env->SearchPath[i], listName);
		if(fexists(tmpPath))
		{
			lstPath = tmpPath;
			break;
		}
	}
	if(NULL == lstPath) /* not found */
	{
		/* try to open with absolute path */
		free(tmpPath);
		lstPath = Str_copy(listName);
		if(!fexists(lstPath))
		{
			/* list not found */
			obs->err(0, GSID_LIST_NOT_FOUND, listName);
			free(lstPath);
			delete_List(&list);
			return false;
		}
	}

	/* open list file */
	if(0 == setjmp(e)) /* try */
	{
		lstFile = new_TextFile(lstPath);
		free(lstPath);
		if(FileOpen_NoError != lstFile->Open(lstFile))
		{
			obs->err(0, GSID_LIST_OPEN_FAILED, lstFile->super.path_get(&lstFile->super));
			longjmp(e, 1);
		}

		obs->debug(0, GSID_LIST_PARSE_START, lstFile->super.path_get(&lstFile->super));

		/* read list file */
		linebuf = lstFile->GetLine(lstFile);
		while(NULL != linebuf)
		{
			work = Str_copy(linebuf);
			RemoveComment(work);

			/* empty line */
			if('\0' == *work)
			{
				free(work); work = NULL;
				linebuf = lstFile->GetLine(lstFile);
				continue;
			}

			/* parse list */
			switch(FunexMatchObs(work, fs, obs))
			{
				case FunexMatch_Section:
				case FunexMatch_List:
					break;

				default:	/* Not handled... */
					obs->err(0, GSID_LIST_SYNTAX_ERROR,
							lstFile->super.path_get(&lstFile->super),
							lstFile->row_get(lstFile));
					longjmp(e, 1);
			}

			/* next line */
			free(work); work=NULL;
			linebuf = lstFile->GetLine(lstFile);
		}

		env->ListDir = Str_copy(lstFile->super.dir_get(&lstFile->super));
		delete_TextFile(&lstFile);
	}
	else
	{
		delete_TextFile(&lstFile);
		free(work);
		return false;
	}

	/* delete previous list */
	if(NULL != (*listPtr))
	{
		delete_List(listPtr);
	}

	/* reset search path because list path changed */
	SetSearchPath(env);
	(*listPtr) = list;
	return true;
}
