/**
 * @file ChkAsmType.c
 */
#include <bolib.h>
#include <string.h>
#include <stdlib.h>
#if !isWindows
#  include <strings.h>
#endif
#include <bolib/file/TextFile.h>
#include "common/Observer.h"
#include "common/Funex.h"
#include "common/strres.h"
#include "mewthree/ChkAsmType.h"

static void RemoveComment(char* line)
{
	char* s;
	int i;
	int len;

	s = strstr(line, ";");
	if(NULL != s)
	{
		(*s) = '\0';
	}

	len = (int)strlen(line);
	for(i = len-1; i>=0; i--)
	{
		if(false == IsSpace(line[i])) break;
		line[i] = '\0';
	}
}

static bool matchesSub(const char** p, const char* op)
{
	for(; NULL!=(*p); p++)
	{
		if(0 == strcasecmp((*p), op))
		{
			return true;
		}
	}

	return false;
}

static bool matchXkasOperator(const char* op)
{
	static const char* xkas_ops[] = {
		"print",
		"macro",
		NULL
	};

	return matchesSub(xkas_ops, op);
}

static bool matchTrasmOperator(const char* op)
{
	static const char* trasm_ops[] = {
		"dcb", "dc.b",
		"src", "bin",
		NULL
	};

	return matchesSub(trasm_ops, op);
}

AsmType ChkAsmType(const char* asmPath, Observers* obs)
{
	TextFile* asmFile;
	const char* linebuf;
	char* work;
	char *f1, *f2, *f3;
	bool hasColon = false;
	size_t i;
	size_t len;

	if(NULL == asmPath) return AsmType_Unknown;

	asmFile = new_TextFile(asmPath);
	if(NULL == asmFile)
	{
		obs->fatal(0, GSID_MEMALLOC_FAILED, __FILE__, __LINE__, __func__);
		return AsmType_Unknown;
	}
	if(FileOpen_NoError != asmFile->open2(asmFile, "r"))
	{
		obs->err(0, GSID_ASM_OPEN_FAILED, asmPath);
		delete_TextFile(&asmFile);
		return AsmType_Unknown;
	}

	linebuf = asmFile->getline(asmFile);
	while(NULL != linebuf)
	{
		work = Str_copy(linebuf);
		RemoveComment(work);

		f1 = f2 = f3 = NULL;
		len = strlen(work);
		if(0 == len) goto Next;

		/* get first field */
		i = 0;
		SkipSpaces(work, &i, len);
		f1 = &work[i];
		SkipUntilSpaces(work, &i, len);
		work[i] = '\0';
		if(i<len) i++;

		/* get second field */
		SkipSpaces(work, &i, len);
		f2 = &work[i];
		SkipUntilSpaces(work, &i, len);
		work[i] = '\0';
		if(i<len) i++;

		/* get third field */
		SkipSpaces(work, &i, len);
		f3 = &work[i];
		SkipUntilSpaces(work, &i, len);
		work[i] = '\0';

		/* field3(operand) check */
		if('\0' != (*f3))
		{
			/* <lab>(:)  <operator>  <operand> */
			if(true == matchTrasmOperator(f2))
			{
				obs->debug(0, GSID_CHKASM_JUDGE_TRASM, asmPath, asmFile->row_get(asmFile));
				free(work);
				delete_TextFile(&asmFile);
				return AsmType_TRASM;
			}
			if(true == matchXkasOperator(f2))
			{
				obs->debug(0, GSID_CHKASM_JUDGE_ASAR, asmPath, asmFile->row_get(asmFile));
				free(work);
				delete_TextFile(&asmFile);
				return AsmType_ASAR;
			}

			/* print "xxx ", xxxxx */
			if(true == matchTrasmOperator(f1))
			{
				obs->debug(0, GSID_CHKASM_JUDGE_TRASM, asmPath, asmFile->row_get(asmFile));
				free(work);
				delete_TextFile(&asmFile);
				return AsmType_TRASM;
			}
			if(true == matchXkasOperator(f1))
			{
				obs->debug(0, GSID_CHKASM_JUDGE_ASAR, asmPath, asmFile->row_get(asmFile));
				free(work);
				delete_TextFile(&asmFile);
				return AsmType_ASAR;
			}

			goto LabelChk;
		}

		/* field2(operand or operator) */
		if('\0' != (*f2))
		{
			/* <operator>  <operand> */
			if(true == matchTrasmOperator(f1))
			{
				obs->debug(0, GSID_CHKASM_JUDGE_TRASM, asmPath, asmFile->row_get(asmFile));
				free(work);
				delete_TextFile(&asmFile);
				return AsmType_TRASM;
			}
			if(true == matchXkasOperator(f1))
			{
				obs->debug(0, GSID_CHKASM_JUDGE_ASAR, asmPath, asmFile->row_get(asmFile));
				free(work);
				delete_TextFile(&asmFile);
				return AsmType_ASAR;
			}

			/* <lab>(:)  <operator> */
			goto LabelChk;
		}

LabelChk:
		/* field1 only(label or operator) */
		if(':' == f1[strlen(f1)-1])
		{
			hasColon = true;
		}
Next:
		free(work); work=NULL;
		linebuf = asmFile->getline(asmFile);
	}

	delete_TextFile(&asmFile);
	/* if colon is not exist, judge trasm */
	if(false == hasColon)
	{
		obs->debug(0, GSID_CHKASM_NON_COLON_TRASM, asmPath);
		return AsmType_TRASM;
	}

	obs->debug(0, GSID_CHKASM_COLON_ASAR, asmPath);
	return AsmType_ASAR;
}

