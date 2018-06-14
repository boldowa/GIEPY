/**
 * @file Observer.c
 */
#include <stdio.h>
#include <bolib.h>
#include <stdarg.h>
#include <ctype.h>
#include "common/Observer.h"
#include "dll/Mewthree.def"
#include "common/GetFmtStr.h"
#if isWindows
#  include <windows.h>
#endif
#include "cui/Console.h"

#ifndef _tprintf
#define _tprintf printf
#endif

extern bool vdebug;
extern bool disableWarn;
bool isPut = true;
ProgressMode pMode = ProgressMode_Normal;

static void putcommon(uint flag, FILE* tgt, char* buffer, uint id, va_list vl, bool newLine)
{
	static const ConsoleAttribute cas[] = { CA_Default, CA_Success, CA_Error, CA_Important };

	int attr = 0;
	if(0 != (OBSFLG_SUCCESS & flag)) attr=1;
	if(0 != (OBSFLG_FAILURE & flag)) attr=2;
	if(0 != (OBSFLG_IMPORTANT & flag)) attr=3;

	sprintf(buffer, "%s%s", GetFmtStr(id), newLine?"\n":"([Y]es/[N]o): ");

	if(attr) SetConsoleAttribute(stdout, cas[attr]);
	vfprintf(tgt, buffer, vl);
	if(attr) SetConsoleAttribute(stdout, CA_Default);

	isPut = true;
}

uint putdbg(uint flag, uint id, ...)
{
	va_list vl;
	char buf[2048];

	if(vdebug)
	{
		va_start(vl, id);

		fprintf(stdout, "[");
		fprintf(stdout, "%s", GetFmtStr(GSID_DEBUG));
		fprintf(stdout, "] ");
		putcommon(flag, stdout, buf, id, vl, true);

		va_end(vl);
	}

	return MEW_OBS_OK;
}

uint putinf(uint flag, uint id, ...)
{
	va_list vl;
	char buf[2048];

	va_start(vl, id);

	fprintf(stdout, "[");
	fprintf(stdout, "%s", GetFmtStr(GSID_INFO));
	fprintf(stdout, "] ");
	putcommon(flag, stdout, buf, id, vl, true);

	va_end(vl);

	return MEW_OBS_OK;
}

static bool getqres(char c, uint* result)
{
	switch(c)
	{
		case 'y':
			(*result) = MEW_OBS_YES;
			return true;

		case 'n':
			(*result) = MEW_OBS_NO;
			return true;

		default:
			break;
	}

	return false;
}
uint putques(uint flag, uint id, ...)
{
	va_list vl;
	char buf[2048];
	char s[2];

	uint result = MEW_OBS_CANCEL;

	va_start(vl, id);
	for(;;)
	{
		fprintf(stdout, "[");
		fprintf(stdout, "%s", GetFmtStr(GSID_INQUIRY));
		fprintf(stdout, "] ");
		putcommon(flag, stdout, buf, id, vl, false);

		/* read stdin */
		fgets(s, 2, stdin);
		if(s[0] != '\n') if(s[1] != '\n') while(getchar()!='\n'); /* flush */
		/*
		scanf("%1s%*[^\n]", s);
		getchar();
		*/

		/* get answer */
		if(getqres((char)tolower(s[0]), &result)) break;
	}
	va_end(vl);

	return result;
}

uint putwar(uint flag, uint id, ...)
{
	va_list vl;
	char buf[2048];

	if(!disableWarn)
	{
		va_start(vl, id);

		fprintf(stderr, "[");
		SetConsoleAttribute(stderr, CA_Warning);
		fprintf(stderr, "%s", GetFmtStr(GSID_WARN));
		SetConsoleAttribute(stderr, CA_Default);
		fprintf(stderr, "] ");
		putcommon(flag, stderr, buf, id, vl, true);

		va_end(vl);
	}

	return MEW_OBS_OK;
}

uint puterr(uint flag, uint id, ...)
{
	va_list vl;
	char buf[2048];

	va_start(vl, id);

	fprintf(stderr, "[");
	SetConsoleAttribute(stderr, CA_Error);
	fprintf(stderr, "%s", GetFmtStr(GSID_ERROR));
	SetConsoleAttribute(stderr, CA_Default);
	fprintf(stderr, "] ");
	putcommon(flag, stderr, buf, id, vl, true);

	va_end(vl);

	return MEW_OBS_OK;
}

uint putfatal(uint flag, uint id, ...)
{
	va_list vl;
	char buf[2048];

	va_start(vl, id);

	fprintf(stderr, "[");
	SetConsoleAttribute(stderr, CA_Fatal);
	fprintf(stderr, "%s", GetFmtStr(GSID_FATAL));
	SetConsoleAttribute(stderr, CA_Default);
	fprintf(stderr, "] ");
	putcommon(flag, stderr, buf, id, vl, true);

	va_end(vl);

	return MEW_OBS_OK;
}

void timesub(uint* time, const uint msec)
{
	time[3] = (msec/10);
	time[2] = (msec/1000);
	time[1] = (time[2]/60);
	time[0] = (time[1]/60);

	time[3] %= 100;
	time[2] %= 60;
	time[1] %= 60;
}

void progress(uint cur, uint all)
{
	uint rate;
	float rate_f;
	static const int pbwidth = 50;
	int i;
	int len;

	rate_f = (float)cur/(float)all;
	rate = (uint)(rate_f*100);
	len = (int)((float)pbwidth*rate_f);

	if(false == isPut)
		MoveBackLines(stdout, 2);
	isPut = false;

	switch(pMode)
	{
		case ProgressMode_Normal:
			printf(" %4d / %4d\n", cur, all);
			break;

		case ProgressMode_Timer:
			{
				uint ctime[4];
				uint atime[4];
				timesub(ctime, cur);
				timesub(atime, all);
				printf(" %02d:%02d:%02d /",  ctime[0],ctime[1],ctime[2]);
				printf(" %02d:%02d:%02d \n", atime[0],atime[1],atime[2]);
			}
			break;
	}
	printf("[");
	SetConsoleAttribute(stdout, CA_ProgressBar);
	for(i=0;i<len;i++)
	{
		printf("=");
	}
	for(;i<pbwidth;i++)
	{
		printf(" ");
	}
	SetConsoleAttribute(stdout, CA_Default);
	printf("] %3d%%\n", rate);
}
