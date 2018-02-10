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

CfgData* ParseCfg(const char* cfgPath, int number, Observers* obs)
{
	CfgData* data;
	TextFile* cfgFile;
	const char* linebuf;
	char* work=NULL;
	jmp_buf e;
	uint lines;

	data = calloc(1, sizeof(CfgData));
	assert(data);

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
			work = Str_copy(linebuf);
			RemoveComment(work);

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

