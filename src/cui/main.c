/**
 * @file main.c
 *   - The sprite insertion tool cui interface main source
 */
#include <bolib.h>
#include <stdio.h>
#include <string.h>
#include "compiler.h"
#include "common/defaults.h"
#include "common/strres.h"
#include "common/version.h"
#include <assert.h>
#include <setjmp.h>
#if isWindows
#  include <windows.h>
#  include <tchar.h>
#endif
#include "common/puts.h"
#include "common/srcpath.h"
#include "common/AsmType.h"
#include "common/Observer.h"
#include "common/InsertList.h"
#include "common/CfgData.h"
#include "common/MewInterfaces.h"
#include "common/Environment.h"
#include "dll/MewthreeDll.h"
#include "cui/Console.h"
#include "cui/Option.h"
#include "cui/ParseCmdDefs.h"
#include "common/GetFmtStr.h"

#ifndef _tprintf
#define _tprintf printf
#endif

#ifndef _tmain
#  define _tmain(argc,argv) main(argc,argv)
#endif

typedef struct _OptionValue {
	bool enableExtraBytes;
	bool forceReInstall;
	bool isPixiCompatible;
	bool disableSscGen;
	const char* listName;
	const char* libsDirName;
	const char* spritesDirName;
	const char* extendedsDirName;
	const char* clustersDirName;
	const char* owsDirName;
	List* defineList;
} OptionValue;

/* global options ---------------------------------*/
bool vdebug		= false;	/* @brief option value for observer */
bool disableWarn	= false;	/* @brief option value for observer */
extern uint strid_base;
extern ProgressMode pMode;

/**
 * @brief Show version info
 */
static void ShowVersion()
{
	printf("%s v%s [Code: v0x%04x]\n", AppName, AppVersion, CodeVersion);
	printf("  by boldowa\n");
	printf("  since    : December 10 2017\n");
	printf("  compiled : %s\n", __DATE__);
	printf("             with %s ver.%s\n", GIEPY_C_COMPILER, GIEPY_C_COMPILER_VERSION);
	printf("  Mewthree : v%s\n", GetMewthreeVersionStr());
	printf("  Asar     : v%s API v%s\n",
			GetAsarVersion(), GetAsarAPIVersion() );
	printf("  Jansson  : v%s\n", GetJanssonVersion());
}

/**
 * @brief Show application usage
 *
 * @param opt option struct array pointer
 */
static void ShowUsage(const OptionStruct* opt)
{
	printf("Usage: %s [options] <rom>\n", Environment.ExeName);
	Option_Usage(opt);
}


/**
 * @brief Insert sprites into SMW ROM
 *
 * @param mew Mewthree interface object
 * @param opt options
 *
 * @return true:success / false:failure
 */
static bool Insert(MewInterface* mew, const OptionValue* opt)
{
	srcdirs dirs = {
		opt->spritesDirName,
		opt->extendedsDirName,
		opt->clustersDirName,
		opt->owsDirName
	};
	/* read list file */
	putinf(OBSFLG_IMPORTANT, GSID_INSERT_PARSE_LIST);
	if(false == mew->ParseList(mew, opt->listName))
	{
		puterr(0, GSID_LIST_PARSE_FAILED);
		return false;
	}

	/* initialize workspace */
	putdbg(0, GSID_INSERT_CREATE_WORKSP);
	mew->InitializeWorkspace(mew, opt->defineList);

	/* generate libraries data */
	putdbg(0, GSID_INSERT_COLLECT_LIBS);
	if(false == mew->CollectLibraries(mew, opt->libsDirName))
	{
		return false;
	}

	/* initialize rom */
	putinf(OBSFLG_IMPORTANT, GSID_INSERT_INITIALIZE_ROM);
	if(false == mew->InitializeRom(mew, opt->enableExtraBytes, opt->forceReInstall, opt->isPixiCompatible))
	{
		return false;
	}

	/* assemble all asm data */
	putinf(OBSFLG_IMPORTANT, GSID_INSERT_ASSEMBLE_ALL);
	if(false == mew->AssembleAll(mew, &dirs))
	{
		return false;
	}

	/* insert sprites */
	putinf(OBSFLG_IMPORTANT, GSID_INSERT_SPRITES);
	if(false == mew->InsertSprite(mew, opt->disableSscGen))
	{
		return false;
	}

	/* insert extended */
	putinf(OBSFLG_IMPORTANT, GSID_INSERT_EXTENDED);
	if(false == mew->InsertExtended(mew))
	{
		return false;
	}

	/* insert cluster */
	putinf(OBSFLG_IMPORTANT, GSID_INSERT_CLUSTER);
	if(false == mew->InsertCluster(mew))
	{
		return false;
	}

	/* insert owspr */
	/*putinf(OBSFLG_IMPORTANT, GSID_INSERT_OVERWORLD);*/
	if(false == mew->InsertOverworld(mew))
	{
		return false;
	}

	putinf(0, GSID_INSERT_CHK_DATA_LEAK);
	if(false == mew->CheckDataLeak(mew))
	{
		putwar(0, GSID_INSERT_DATA_LEAK_DETECT);
		return false;
	}

	/* clean data */
	putdbg(0, GSID_INSERT_CLEAN_WORKSP);
	mew->CleanWorkspace(mew);

	return true;
}



/**
 * @brief Uninstall GIEPY from ROM
 *
 * @param mew Mewthree interface object
 * @param opt options(not to use)
 *
 * @return true:success / false:failure
 */
static bool Uninstall(MewInterface* mew, const OptionValue* opt)
{
	putinf(OBSFLG_IMPORTANT, GSID_UNINSTALLING, AppName);
	return mew->UninstallAll(mew);
}


/**
 * @brief Write data into rom.
 *
 * @param rompath
 * @param opt     cmd line options
 * @param proc    process function pointer
 *
 * @return 
 */
static bool WriteRom(MewInterface* mew, const char *rompath, const OptionValue* opt, bool (*proc)(MewInterface*, const OptionValue*))
{
	bool result;

	assert(rompath);
	assert(proc);

	if(false == mew->OpenRomFile(mew, rompath))
	{
		return false;
	}

	/* execute rom access proc */
	result = proc(mew, opt);

	/* write rom */
	if(true == result)
	{
		result = mew->WriteRomFile(mew);
		if(false == result)
		{
			putfatal(OBSFLG_FAILURE, GSID_ROM_WRITE_FAILED);
		}
	}

	mew->CloseRomFile(mew);
	return result;
}


/**
 * @brief Giepy main
 *
 * @param argc arguments count
 * @param argv arguments value
 *
 * @return result
 */
int Giepy(int argc, TCHAR** argv)
{
	/* options */
	bool showHelp = false;
	bool showVersion = false;
	bool uninstallMode = false;
	bool asmTypeCheckMode = false;
	bool cfgSyntaxCheckMode = false;
	bool jsonSyntaxCheckMode = false;
	uint  rtMinutes = 0;
	OptionValue opt;
	List* defineList;
	MewInterface* mew;

	/* var */
	bool result = false;

	/* command-line options */
	SetOptStruct defOptSt = { ParseCmdDefs, &defineList };
	OptionStruct options[] = {
		{ "bytes",	'b', "enable extra bytes feature", OptionType_Bool, &opt.enableExtraBytes },
		{ "force",	'f', "force re-install " AppName , OptionType_Bool, &opt.forceReInstall },
		{ "pixi",	'p', "PIXI compatible mode" , OptionType_Bool, &opt.isPixiCompatible },
		{ "notooltip",	't', "disable to generate custom tooltips" , OptionType_Bool, &opt.disableSscGen },
		{ "list",	'l', "specify list file path (default: " DefaultListName ")", OptionType_String, (void*)&opt.listName },
		{ "library",	'L', "specify library dir (default: " DefaultLibsDirName ")", OptionType_String, (void*)&opt.libsDirName },
		{ "sprite",	's', "specify sprites dir (default: " DefaultSprDirName ")", OptionType_String, (void*)&opt.spritesDirName },
		{ "extended",	'e', "specify extended sprites dir (default: " DefaultExtDirName ")", OptionType_String, (void*)&opt.extendedsDirName },
		{ "cluster",	'c', "specify cluster sprites dir (default: " DefaultClsDirName ")", OptionType_String, (void*)&opt.clustersDirName },
		{ "ow",		'o', "specify ow sprites dir (default: " DefaultOWSDirName ")", OptionType_String|OPT_Hyde, (void*)&opt.owsDirName },
		{ "define",	'D', "add asm define", OptionType_FunctionString, &defOptSt },
		{ "ramen",	'r', "ramen timer", OptionType_Int|OPT_Hyde, &rtMinutes },
		{ "typechk",	't', "check input asm type (feature for debug)", OptionType_Bool|OPT_Hyde, &asmTypeCheckMode },
		{ "cfgcheck",	'C', "check cfg file syntax", OptionType_Bool|OPT_Hyde, &cfgSyntaxCheckMode },
		{ "jsoncheck",	'J', "check json file syntax", OptionType_Bool|OPT_Hyde, &jsonSyntaxCheckMode },
		{ "uninstall",	'u', "uninstall " AppName, OptionType_Bool, &uninstallMode },
		{ "nowarn",	'w', "disable to verbose warningas", OptionType_Bool, &disableWarn },
		{ "debug",	'd', "verbose debug info", OptionType_Bool, &vdebug },
		{ "version",	'v', "show version", OptionType_Bool, &showVersion },
		{ "help",	'?', "show help message", OptionType_Bool, &showHelp },
		/* term */
		{ NULL, '\0', NULL, OptionType_Term, NULL },
	};

	/* observers */
	Observers obs = {
		putdbg, /*MewObserver debug;*/
		putinf, /*MewObserver inf;*/
		putques, /*MewObserver ques;*/
		putwar, /*MewObserver warn;*/
		puterr, /*MewObserver err;*/
		putfatal, /*MewObserver fatal;*/
		progress/*ProgressNotifier_t progress;*/
	};


	/***----------------------------------------------------***/
	/*                         MAIN PROCESS                   */
	/***----------------------------------------------------***/

	InitConsole();

	/* set exe/cur path */
	SetSystemEnvironment();

	if(0 == strcmp(JP_SPECIALNAME, Environment.ExeName))
	{
		printf(OYAPIPPI "\n");
		strid_base = GSID_JP_INDEX;
	}

	defineList = new_List(NULL, DelDefine);
	if(NULL == defineList)
	{
		putmemerr();
		return -1;
	}

	/* init options */
	opt.enableExtraBytes = false;
	opt.forceReInstall = false;
	opt.isPixiCompatible = false;
	opt.disableSscGen = false;
	opt.listName = DefaultListName;
	opt.libsDirName = DefaultLibsDirName;
	opt.spritesDirName = DefaultSprDirName;
	opt.extendedsDirName = DefaultExtDirName;
	opt.clustersDirName = DefaultClsDirName;
	opt.owsDirName = DefaultOWSDirName;
	opt.defineList = defineList;

	/* dynamic link to mewthree.dll */
	if(false == LoadMewthree(Environment.ComponentsDir)) /* Link to DLL */
	{
		putfatal(0,GSID_MEW_LOAD_FAILED);
		delete_List(&defineList);
		return -1;
	}

	/* get mewthree interface */
	mew = CreateMew();
	if(NULL == mew)
	{
		CloseMewthree();
		delete_List(&defineList);
		return -1;
	}

	/* Get command-line options */
	if(false == Option_Parse(&argc, &argv, options))
	{
		puterr(0,GSID_OPTION_PARSE_FAILED);
		DiscardMew(&mew);
		CloseMewthree();
		delete_List(&defineList);
		return -1;
	}

	/* register observer */
	if(vdebug) obs.progress = NULL;
	mew->RegisterObserver(mew,obs);

	/* set system path for dll */
	mew->SetSystemPath(mew, Environment.CurDir, Environment.ExeDir);

	/* Show help / version */
	if(true == showVersion)
	{
		ShowVersion();
	}
	if(true == showHelp)
	{
		ShowUsage(options);
	}

	/* Show usage message */
	if((argc != 2) && (0 == rtMinutes))
	{
		if ((false == showVersion) && (false == showHelp))
		{
			putinf(0,GSID_OPTION_USAGE, Environment.ExeName);
			putinf(0,GSID_OPTION_MOREINFO);
		}
		DiscardMew(&mew);
		CloseMewthree();
		delete_List(&defineList);
		return 0;
	}

	if(0 != rtMinutes)
	{
		pMode = ProgressMode_Timer;
		mew->RoughTimer(mew, 60000*rtMinutes, 500);
#if isWindows
		MessageBox(GetConsoleHwnd(), GetFmtStr(GSID_RAMEN_END),GetFmtStr(GSID_RAMEN_END_TITLE), MB_OK|MB_ICONINFORMATION);
#else
		putinf(0, GSID_RAMEN_END);
		printf("\a");
#endif
		result = true;
	}
	else if(true == asmTypeCheckMode)
	{
		/* Check ASM type (test feature) */
		AsmType type;
		type = mew->ChkAsmType(mew, argv[1]);
		result = true;
		switch(type)
		{
			case AsmType_TRASM:
				printf("TRASM asm file\n");
				break;

			case AsmType_ASAR:
				printf("ASAR or XKAS asm file\n");
				break;

			default:
				printf("ERR: Unknown asm\n");
				result = false;
				break;
		}
	}
	else if((true == cfgSyntaxCheckMode) || (true == jsonSyntaxCheckMode))
	{
		CfgData* c;
		if(cfgSyntaxCheckMode)
		{
			c = mew->ParseCfg(mew, argv[1]);
		}
		else
		{
			c = mew->ParseJson(mew, argv[1]);
		}
		if(NULL != c)
		{
			printf("[%s]\n", argv[1]);
			printf("  new_code_flag      : $%02x\n", c->new_code_flag);
			printf("  orig_sprite_number : $%02x\n", c->orig_sprite_number);
			printf("  tweaks[0]          : $%02x\n", c->tweaks[0]);
			printf("  tweaks[1]          : $%02x\n", c->tweaks[1]);
			printf("  tweaks[2]          : $%02x\n", c->tweaks[2]);
			printf("  tweaks[3]          : $%02x\n", c->tweaks[3]);
			printf("  tweaks[4]          : $%02x\n", c->tweaks[4]);
			printf("  tweaks[5]          : $%02x\n", c->tweaks[5]);
			printf("  extra props[0]     : $%02x\n", c->extra_props[0]);
			printf("  extra props[1]     : $%02x\n", c->extra_props[1]);
			printf("  asm_name1          : %s\n", c->asm_name1);
			printf("  sprite_type        : $%02x\n", c->sprite_type);
			printf("  unique info        : $%02x\n", c->unique_info);
			printf("  extra_byte_nums    : $%02x\n", c->extra_byte_nums);
			printf("  asm_name2          : %s\n", c->asm_name2);

			mew->DiscardCfgData(mew, &c);
			result = true;
		}
	}
	else if(true == uninstallMode)
	{
		/* Uninstall GIEPY */
		result = WriteRom(mew, argv[1], NULL, Uninstall);
	}
	else
	{
		/* install GIEPY / insert sprites */
		result = WriteRom(mew, argv[1], &opt, Insert);
	}

	DiscardMew(&mew);
	CloseMewthree();  /* Unlink DLL */
	delete_List(&defineList);

	if(false == result)
	{
		putinf(OBSFLG_FAILURE, GSID_PROCESS_FAILED);
		return -1;
	}

	putinf(OBSFLG_SUCCESS, GSID_PROCESS_SUCCEED);
	return 0;
}


#ifndef CPPUTEST
/**
 * @brief Program entry point
 *
 * @param argc arguments count
 * @param argv arguments values
 *
 * @return result
 */
int _tmain(int argc, TCHAR** argv)
{
	return Giepy(argc, argv);
}
#endif
