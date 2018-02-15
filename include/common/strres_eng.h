#pragma once
/**
 * @file strres_eng.h
 *   English text resources
 */

/* 0 - 99 : title */
#define		GSID_STR_NOT_FOUND		(GSID_BEGIN +    0)
#define		GSTR_STR_NOT_FOUND		"(***Missing string***)"

#define		GSID_DEBUG			(GSID_BEGIN +    1)
#define		GSTR_DEBUG			"   DEBUG   "
#define		GSID_INFO			(GSID_BEGIN +    2)
#define		GSTR_INFO			"   INFO    "
#define		GSID_INQUIRY			(GSID_BEGIN +    3)
#define		GSTR_INQUIRY			" QUESTION  "
#define		GSID_WARN			(GSID_BEGIN +    4)
#define		GSTR_WARN			"  WARNING  "
#define		GSID_ERROR			(GSID_BEGIN +    5)
#define		GSTR_ERROR			"   ERROR   "
#define		GSID_FATAL			(GSID_BEGIN +    6)
#define		GSTR_FATAL			"FATAL ERROR"
#define		GSID_STRING			(GSID_BEGIN +   99)
#define		GSTR_STRING			"%s"

/* 100 - 9899 : generic */
#define		GSID_MEW_LOAD_FAILED		(GSID_BEGIN +  100)
#define		GSTR_MEW_LOAD_FAILED		"Failed to load Mewthree module. Please try setup again."
#define		GSID_MEMALLOC_FAILED		(GSID_BEGIN +  101)
#define		GSTR_MEMALLOC_FAILED		"Memory allocation failed. Processing can't be continued on the computer you are using. (location: %s)"
#define		GSID_PROCESS_FAILED		(GSID_BEGIN +  102)
#define		GSTR_PROCESS_FAILED		"--- Failed... ;("
#define		GSID_PROCESS_SUCCEED		(GSID_BEGIN +  103)
#define		GSTR_PROCESS_SUCCEED		"--- Succeeded. ;)"
#define		GSID_PROGLOGIC_ERROR		(GSID_BEGIN +  104)
#define		GSTR_PROGLOGIC_ERROR		"Program internal error: dst is empty. (location: %s)"
#define		GSID_UNINSTALLING		(GSID_BEGIN +  105)
#define		GSTR_UNINSTALLING		"Uninstalling %s..."
#define		GSID_PROCESS_ABORTED		(GSID_BEGIN +  106)
#define		GSTR_PROCESS_ABORTED		"--- Aborted... :o"

#define		GSID_OPTION_PARSE_FAILED	(GSID_BEGIN +  110)
#define		GSTR_OPTION_PARSE_FAILED	"Failed to analyze the command line option info. Please input the valid information."
#define		GSID_UNKNOWN_OPTION		(GSID_BEGIN +  111)
#define		GSTR_UNKNOWN_OPTION		"Unknown option : \"%s\""
#define		GSID_OPTION_ARGMISSING		(GSID_BEGIN +  112)
#define		GSTR_OPTION_ARGMISSING		"Information necessary for \"%s\" option is missing. "
#define		GSID_OPTION_INVALID		(GSID_BEGIN +  113)
#define		GSTR_OPTION_INVALID		"You have entered an invalid value for \"%s\" option : \"%s\""
#define		GSID_OPTION_MISSPARSER		(GSID_BEGIN +  114)
#define		GSTR_OPTION_MISSPARSER		"Program error: Required number of option parsers are missing. (location: %s)"
#define		GSID_OPTION_USAGE		(GSID_BEGIN +  115)
#define		GSTR_OPTION_USAGE		"Usage: %s [options] <ROM>"
#define		GSID_OPTION_MOREINFO		(GSID_BEGIN +  116)
#define		GSTR_OPTION_MOREINFO		"Please try '-?' or '--help' option, and you can get more information."

#define		GSID_ROM_OPEN_FAILED		(GSID_BEGIN +  150)
#define		GSTR_ROM_OPEN_FAILED		"It couldn't open the ROM. Make sure that the ROM exists or is not opened by another program."
#define		GSID_ROM_IS_NOT_SMW		(GSID_BEGIN +  151)
#define		GSTR_ROM_IS_NOT_SMW		"This ROM file isn't SMW ROM."
#define		GSID_ROM_WRITE_FAILED		(GSID_BEGIN +  152)
#define		GSTR_ROM_WRITE_FAILED		"Failed to write to ROM. Check the status of the storage."

#define		GSID_ASM_OPEN_FAILED		(GSID_BEGIN +  160)
#define		GSTR_ASM_OPEN_FAILED		"It couldn't open the ASM file \"%s\"."

#define		GSID_LIST_NOT_FOUND		(GSID_BEGIN +  170)
#define		GSTR_LIST_NOT_FOUND		"List file \"%s\" couldn't be found."
#define		GSID_LIST_OPEN_FAILED		(GSID_BEGIN +  171)
#define		GSTR_LIST_OPEN_FAILED		"It couldn't open the list file \"%s\". Please check the file access permission."
#define		GSID_LIST_PARSE_START		(GSID_BEGIN +  172)
#define		GSTR_LIST_PARSE_START		"Loading the list file \"%s\"... "
#define		GSID_LIST_PARSE_FAILED		(GSID_BEGIN +  173)
#define		GSTR_LIST_PARSE_FAILED		"Failed to load the list file."
#define		GSID_LIST_SYNTAX_ERROR		(GSID_BEGIN +  174)
#define		GSTR_LIST_SYNTAX_ERROR		"\"%s\": line %d: Syntax error."
#define		GSID_LIST_INVALID_SECTION	(GSID_BEGIN +  175)
#define		GSTR_LIST_INVALID_SECTION	"Section \"%s\" isn't exist. This specification is ignored."
#define		GSID_LIST_SECTION_MATCH		(GSID_BEGIN +  176)
#define		GSTR_LIST_SECTION_MATCH		"Match the \"%s\" section"
#define		GSID_LIST_INSERT_NUM_LARGE	(GSID_BEGIN +  177)
#define		GSTR_LIST_INSERT_NUM_LARGE	"Insertion number is too large. (Max: $%02x)"
#define		GSID_LIST_INSERT_NUM_FEW	(GSID_BEGIN +  178)
#define		GSTR_LIST_INSERT_NUM_FEW	"Insertion number is too small. (Min: $%02x)"
#define		GSID_LIST_INSERT_CONFLICT	(GSID_BEGIN +  179)
#define		GSTR_LIST_INSERT_CONFLICT	"The insertion number is duplicated. (Num:%02x / Grp:%1x \"%s\")"

#define		GSID_UNINSTALL_NOT_INSTALLED	(GSID_BEGIN +  200)
#define		GSTR_UNINSTALL_NOT_INSTALLED	"It isn't installed in this ROM."
#define		GSID_UNINSTALL_RESADR_NULL	(GSID_BEGIN +  201)
#define		GSTR_UNINSTALL_RESADR_NULL	"There is no place to insert the restore data. The ROM data may be corrupted."
#define		GSID_UNINSTALL_DATA_BROKEN	(GSID_BEGIN +  202)
#define		GSTR_UNINSTALL_DATA_BROKEN	"The information necessary for uninstalling sprites is corrupted..."
#define		GSID_UNINSTALL_INVALID_RATS	(GSID_BEGIN +  203)
#define		GSTR_UNINSTALL_INVALID_RATS	"The RATS tag of the data is broken..."
#define		GSID_UNINSTALL_RATSCLEAN_NG	(GSID_BEGIN +  204)
#define		GSTR_UNINSTALL_RATSCLEAN_NG	"Failed to clear the routine data. The ROM data may be corrupted."
#define		GSID_UNINSTALL_PROCESS_GRP	(GSID_BEGIN +  205)
#define		GSTR_UNINSTALL_PROCESS_GRP	"Uninstalling sprites info stored in group %d..."
#define		GSID_UNINSTALL_EXTENDED		(GSID_BEGIN +  206)
#define		GSTR_UNINSTALL_EXTENDED		"Uninstalling extended sprites info..."
#define		GSID_UNINSTALL_CLUSTER		(GSID_BEGIN +  207)
#define		GSTR_UNINSTALL_CLUSTER		"Uninstalling cluster sprites info..."
#define		GSID_UNINSTALL_OVERWORLD	(GSID_BEGIN +  208)
#define		GSTR_UNINSTALL_OVERWORLD	"Uninstalling overworld sprites info..."
#define		GSID_UNINSTALL_SPRITES		(GSID_BEGIN +  209)
#define		GSTR_UNINSTALL_SPRITES		"Uninstalling all sprites..."
#define		GSID_UNINSTALL_LIBRARIES	(GSID_BEGIN +  210)
#define		GSTR_UNINSTALL_LIBRARIES	"Uninstalling libraries..."
#define		GSID_UNINSTALL_COREDATA		(GSID_BEGIN +  211)
#define		GSTR_UNINSTALL_COREDATA		"Uninstalling the core data..."
#define		GSID_UNINSTALL_RESTORE		(GSID_BEGIN +  212)
#define		GSTR_UNINSTALL_RESTORE		"Restoring hijack data..."
#define		GSID_UNINSTALL_CLEANING		(GSID_BEGIN +  213)
#define		GSTR_UNINSTALL_CLEANING		"RatsClean @ $%06x, 0x%x bytes."
#define		GSID_UNINSTALL_TWEAKING		(GSID_BEGIN +  214)
#define		GSTR_UNINSTALL_TWEAKING		"Restore @ $%06x, %d bytes"
#define		GSID_UNINSTALL_TWEAKS		(GSID_BEGIN +  215)
#define		GSTR_UNINSTALL_TWEAKS		"Uninstalling sprite's tweaks info..."
#define		GSID_UNINSTALL_LEAKDATA		(GSID_BEGIN +  216)
#define		GSTR_UNINSTALL_LEAKDATA		"Coding error: Uninstall before clean data (Please report it to developer)"
#define		GSID_UNINSTALL_NOTIMP_RES	(GSID_BEGIN +  217)
#define		GSTR_UNINSTALL_NOTIMP_RES	"Coding error: Restore function missing (Please report it to developer)"

#define		GSID_INSERT_PARSE_LIST		(GSID_BEGIN +  220)
#define		GSTR_INSERT_PARSE_LIST		"Loading the list file..."
#define		GSID_INSERT_CREATE_WORKSP	(GSID_BEGIN +  221)
#define		GSTR_INSERT_CREATE_WORKSP	"Creating work files..."
#define		GSID_INSERT_COLLECT_LIBS	(GSID_BEGIN +  222)
#define		GSTR_INSERT_COLLECT_LIBS	"Collecting library information..."
#define		GSID_INSERT_INITIALIZE_ROM	(GSID_BEGIN +  223)
#define		GSTR_INSERT_INITIALIZE_ROM	"Initializing the ROM..."
#define		GSID_INSERT_SPRITES		(GSID_BEGIN +  224)
#define		GSTR_INSERT_SPRITES		"Inserting sprites info..."
#define		GSID_INSERT_EXTENDED		(GSID_BEGIN +  225)
#define		GSTR_INSERT_EXTENDED		"Inserting extended sprites info..."
#define		GSID_INSERT_CLUSTER		(GSID_BEGIN +  226)
#define		GSTR_INSERT_CLUSTER		"Inserting cluster sprites info..."
#define		GSID_INSERT_OVERWORLD		(GSID_BEGIN +  227)
#define		GSTR_INSERT_OVERWORLD		"Inserting overworld sprites info..."
#define		GSID_INSERT_CLEAN_WORKSP	(GSID_BEGIN +  228)
#define		GSTR_INSERT_CLEAN_WORKSP	"Cleaning work files..."
#define		GSID_INSERT_ASSEMBLE_ALL	(GSID_BEGIN +  229)
#define		GSTR_INSERT_ASSEMBLE_ALL	"Assembling all ASM of sprites..."
#define		GSID_INSERT_CHK_DATA_LEAK	(GSID_BEGIN +  230)
#define		GSTR_INSERT_CHK_DATA_LEAK	"Checking the remaining data..."
#define		GSID_INSERT_DATA_LEAK_DETECT	(GSID_BEGIN +  231)
#define		GSTR_INSERT_DATA_LEAK_DETECT	"There are remaining data. Discard the data and continue."

#define		GSID_LIBRARY_FOUND		(GSID_BEGIN +  240)
#define		GSTR_LIBRARY_FOUND		"Detected library \"%s\"..."
#define		GSID_LIBRARY_CANT_OPEN		(GSID_BEGIN +  241)
#define		GSTR_LIBRARY_CANT_OPEN		"It couldn't open the library file \"%s\"."
#define		GSID_LIBRARY_NOTHING		(GSID_BEGIN +  242)
#define		GSTR_LIBRARY_NOTHING		"There is no library."
#define		GSID_LIBRARY_FILE_NUMBERS	(GSID_BEGIN +  243)
#define		GSTR_LIBRARY_FILE_NUMBERS	"Library file numbers : %d"
#define		GSID_LIBRARY_LABEL_NUMBERS	(GSID_BEGIN +  244)
#define		GSTR_LIBRARY_LABEL_NUMBERS	"Library label numbers: %d"

#define		GSID_ASSEMBLE_FILE		(GSID_BEGIN +  250)
#define		GSTR_ASSEMBLE_FILE		"Assembling \"%s\"..."
#define		GSID_ASSEMBLE_FAILED		(GSID_BEGIN +  251)
#define		GSTR_ASSEMBLE_FAILED		"Assembling of \"%s\" failed..."
#define		GSID_ASSEMBLE_STR_WITH_ASMNAME	(GSID_BEGIN +  252)
#define		GSTR_ASSEMBLE_STR_WITH_ASMNAME	"%s: %s"
#define		GSID_ASSEMBLE_CFG_NOT_FOUND	(GSID_BEGIN +  253)
#define		GSTR_ASSEMBLE_CFG_NOT_FOUND	"CFG file \"%s\" was not found in the search path."
#define		GSID_ASSEMBLE_EMPTY_ASM		(GSID_BEGIN +  254)
#define		GSTR_ASSEMBLE_EMPTY_ASM		"There is no data after assembling. It's speculated that it is an empty ASM."
#define		GSID_ASSEMBLE_USING_ASAR	(GSID_BEGIN +  255)
#define		GSTR_ASSEMBLE_USING_ASAR	"Assembling \"%s\" using ASAR."
#define		GSID_ASSEMBLE_USING_TRASM	(GSID_BEGIN +  256)
#define		GSTR_ASSEMBLE_USING_TRASM	"Assembling \"%s\" using TRASM"
#define		GSID_ASSEMBLE_ASM_NOT_FOUND	(GSID_BEGIN +  257)
#define		GSTR_ASSEMBLE_ASM_NOT_FOUND	"ASM file \"%s\" was not found under CFG file dir."
#define		GSID_ASSEMBLE_CFG_READ_ERROR	(GSID_BEGIN +  258)
#define		GSTR_ASSEMBLE_CFG_READ_ERROR	"Failed to load the CFG file \"%s\"..."
/* 259 : missing number */
#define		GSID_ASSEMBLE_CFG_PROC_FAILED	(GSID_BEGIN +  260)
#define		GSTR_ASSEMBLE_CFG_PROC_FAILED	"CFG file \"%s\" processing failed..."
#define		GSID_ASSEMBLE_BIN_OPEN_ERROR	(GSID_BEGIN +  261)
#define		GSTR_ASSEMBLE_BIN_OPEN_ERROR	"I'm Morikubo, you know?"
#define		GSID_ASSEMBLE_PIPE_OPEN_ERROR	(GSID_BEGIN +  262)
#define		GSTR_ASSEMBLE_PIPE_OPEN_ERROR	"Pipe open error: Please report it to developer. (location: %s )"
#define		GSID_ASSEMBLE_TRASM_ERR_COL	(GSID_BEGIN +  263)
#define		GSTR_ASSEMBLE_TRASM_ERR_COL	"It couldn't read error informations of TRASM. Please try setup the system again."
#define		GSID_ASSEMBLE_TRASM_LO_ONLY	(GSID_BEGIN +  264)
#define		GSTR_ASSEMBLE_TRASM_LO_ONLY	"TRASM doesn't support anything other than LoROM. This ASM may not work properly."
#define		GSID_ASSEMBLE_ENTRY_NOT_FOUND	(GSID_BEGIN +  265)
#define		GSTR_ASSEMBLE_ENTRY_NOT_FOUND	"Neither INIT nor MAIN can be found. Specify the entry point at the beginning of ASM."
#define		GSID_ASSEMBLE_FREE_NOT_FOUND	(GSID_BEGIN +  266)
#define		GSTR_ASSEMBLE_FREE_NOT_FOUND	"ROM has no free spaces..."

#define		GSID_PARSECFG_FIELDS_TOO_FEW	(GSID_BEGIN +  270)
#define		GSTR_PARSECFG_FIELDS_TOO_FEW	"\"%s\": Information is insufficient."
#define		GSID_PARSECFG_READING_FILE	(GSID_BEGIN +  271)
#define		GSTR_PARSECFG_READING_FILE	"Loading the CFG file \"%s\"..."
#define		GSID_PARSECFG_SYNTAX_ERROR	(GSID_BEGIN +  272)
#define		GSTR_PARSECFG_SYNTAX_ERROR	GSTR_LIST_SYNTAX_ERROR
#define		GSID_PARSECFG_OLD_FORMAT_SP	(GSID_BEGIN +  273)
#define		GSTR_PARSECFG_OLD_FORMAT_SP	"\"%s\": It couldn't identify the type of sprite(it is an old format cfg). It's assumed that it is a normal sprite."
#define		GSID_PARSECFG_OLD_FORMAT_SH	(GSID_BEGIN +  274)
#define		GSTR_PARSECFG_OLD_FORMAT_SH	"\"%s\": It couldn't identify the type of sprite(it is an old format cfg). It's assumed that it is a shooter."
#define		GSID_PARSECFG_OLD_FORMAT_GEN	(GSID_BEGIN +  275)
#define		GSTR_PARSECFG_OLD_FORMAT_GEN	"\"%s\": It couldn't identify the type of sprite(it is an old format cfg). It's assumed that it is a generator."
#define		GSID_PARSECFG_TAG_INVALID	(GSID_BEGIN +  276)
#define		GSTR_PARSECFG_TAG_INVALID	GSTR_LIST_INVALID_SECTION

#define		GSID_EXTCLS_BROKEN_RELOC_ADR	(GSID_BEGIN +  280)
#define		GSTR_EXTCLS_BROKEN_RELOC_ADR	"Location information in ROM is broken. Please restore the ROM from the previous data."
#define		GSID_EXTCLS_EXTENDED_LOCATION	(GSID_BEGIN +  281)
#define		GSTR_EXTCLS_EXTENDED_LOCATION	"Table for extended : $%06x"
#define		GSID_EXTCLS_CLUSTER_LOCATION	(GSID_BEGIN +  282)
#define		GSTR_EXTCLS_CLUSTER_LOCATION	"Table for cluster : $%06x"

#define		GSID_SPRITES_TWEAKS_LOC		(GSID_BEGIN +  290)
#define		GSTR_SPRITES_TWEAKS_LOC		"Table for Tweaks : $%06x"
#define		GSID_SPRITES_TABLE_GRP_LOC	(GSID_BEGIN +  291)
#define		GSTR_SPRITES_TABLE_GRP_LOC	"Table for sprite group %d : $%06x"

#define		GSID_INITIALIZE_OLD_TOOL	(GSID_BEGIN +  300)
#define		GSTR_INITIALIZE_OLD_TOOL	"The version of the tool is older than the system version of ROM. Please update the tool to latest version."
#define		GSID_INITIALIZE_FORCE		(GSID_BEGIN +  301)
#define		GSTR_INITIALIZE_FORCE		"Forcibly reinstall the system."
#define		GSID_INITIALIZE_EXBYTE_QUES	(GSID_BEGIN +  302)
#define		GSTR_INITIALIZE_EXBYTE_QUES	"Disable the extra bytes feature. Extra bytes information will be lost, are you sure you want to continue processing?"
#define		GSID_INITIALIZE_TERMED		(GSID_BEGIN +  303)
#define		GSTR_INITIALIZE_TERMED		"System Initialization was interrupted."
#define		GSID_INITIALIZE_UPDATING	(GSID_BEGIN +  304)
#define		GSTR_INITIALIZE_UPDATING	"Updating the system..."
#define		GSID_INITIALIZE_UPDATE_NG	(GSID_BEGIN +  305)
#define		GSTR_INITIALIZE_UPDATE_NG	"System update failed..."
#define		GSID_INITIALIZE_INSTALLING	(GSID_BEGIN +  306)
#define		GSTR_INITIALIZE_INSTALLING	"Installing the system..."
#define		GSID_INITIALIZE_VER_NOT_MATCH	(GSID_BEGIN +  307)
#define		GSTR_INITIALIZE_VER_NOT_MATCH	"ASM version and tool version don't match. Please report it to developer."
#define		GSID_INITIALIZE_EXB_LMVER_SMALL	(GSID_BEGIN +  308)
#define		GSTR_INITIALIZE_EXB_LMVER_SMALL	"Because the version of Lunar Magic is less than 1.80, extra bytes feature can't be enabled."

#define		GSID_COLLECT_LM_NOT_INSTALLED	(GSID_BEGIN +  310)
#define		GSTR_COLLECT_LM_NOT_INSTALLED	"ROM hasn't been edited using Lunar Magic. Please edit with Lunar Magic."
#define		GSID_COLLECT_UNKNOWN_CHUNK	(GSID_BEGIN +  311)
#define		GSTR_COLLECT_UNKNOWN_CHUNK	"Unknown chunk found. (value: %02x %02x %02x %02x)"
#define		GSID_COLLECT_LM_DOWNGRADED	(GSID_BEGIN +  312)
#define		GSTR_COLLECT_LM_DOWNGRADED	"Lunar Magic has been downgraded to less than version 1.80. Cancel processing."
#define		GSID_COLLECT_BROKEN_ROM		(GSID_BEGIN +  313)
#define		GSTR_COLLECT_BROKEN_ROM		"ROM data is broken..."

#define		GSID_CHKASM_JUDGE_TRASM		(GSID_BEGIN +  320)
#define		GSTR_CHKASM_JUDGE_TRASM		"\"%s\" line %d: It's judged as ASM for TRASM."
#define		GSID_CHKASM_JUDGE_ASAR		(GSID_BEGIN +  321)
#define		GSTR_CHKASM_JUDGE_ASAR		"\"%s\" line %d: It's judged as ASM for ASAR(or XKAS)."
#define		GSID_CHKASM_NON_COLON_TRASM	(GSID_BEGIN +  322)
#define		GSTR_CHKASM_NON_COLON_TRASM	"\"%s\" colon is missing. It's judged as ASM for TRASM."
#define		GSID_CHKASM_COLON_ASAR		(GSID_BEGIN +  323)
#define		GSTR_CHKASM_COLON_ASAR		"\"%s\" colon found. It's judged as ASM for ASAR(or XKAS)."

#define		GSID_EXBYTES_SPR_DATA_REMAP	(GSID_BEGIN +  325)
#define		GSTR_EXBYTES_SPR_DATA_REMAP	"Sprites data for Level %03X was remapped. ($%06x => $%06x)"
#define		GSID_EXBYTES_INVALID_ADDR	(GSID_BEGIN +  326)
#define		GSTR_EXBYTES_INVALID_ADDR	"Invalid level data address.(Level %03X: $%06x) If you haven't edited level data with Lunar Magic, please try again after editing."
#define		GSID_EXBYTES_SPRTERM_NOT_FOUND	(GSID_BEGIN +  327)
#define		GSTR_EXBYTES_SPRTERM_NOT_FOUND	"Level %03X: The end of sprite data was not found. Probably this data is broken..."

#define		GSID_RAMEN_END			(GSID_BEGIN +  400)
#define		GSTR_RAMEN_END			"A ramen timer is finished. ;)"
#define		GSID_RAMEN_END_TITLE		(GSID_BEGIN +  401)
#define		GSTR_RAMEN_END_TITLE		"('tt')"

/* Tooltip warnings */
#define		GSID_TOOLTIP_OPEN_FAILED	(GSID_BEGIN +  410)
#define		GSTR_TOOLTIP_OPEN_FAILED	"Failed to open \"%s\". Please check write permission of file."

#if defined(GUI) && isWindows

/*--- win32 menu ---*/
/* File */
#define		GSID_MENU_FILE			(GSID_BEGIN + 2000)
#define		GSTR_MENU_FILE			"&File"
#define		GSID_MENU_FILE_INSERT		(GSID_BEGIN + 2001)
#define		GSTR_MENU_FILE_INSERT		"&Insert"
#define		GSID_MENU_FILE_UNINSTALL	(GSID_BEGIN + 2002)
#define		GSTR_MENU_FILE_UNINSTALL	"&Uninstall"
#define		GSID_MENU_FILE_ABORT		(GSID_BEGIN + 2003)
#define		GSTR_MENU_FILE_ABORT		"&Abort"
#define		GSID_MENU_FILE_EXIT		(GSID_BEGIN + 2004)
#define		GSTR_MENU_FILE_EXIT		"E&xit"


/* Option */
#define		GSID_MENU_OPTION		(GSID_BEGIN + 2010)
#define		GSTR_MENU_OPTION		"&Option"
#define		GSID_MENU_OPTION_PIXI		(GSID_BEGIN + 2011)
#define		GSTR_MENU_OPTION_PIXI		"Use &PIXI compatible code"
#define		GSID_MENU_OPTION_EBEN		(GSID_BEGIN + 2012)
#define		GSTR_MENU_OPTION_EBEN		"Enable Extra &Bytes"
#define		GSID_MENU_OPTION_FORCE		(GSID_BEGIN + 2013)
#define		GSTR_MENU_OPTION_FORCE		"&Force re-install"
#define		GSID_MENU_OPTION_DEBUG		(GSID_BEGIN + 2014)
#define		GSTR_MENU_OPTION_DEBUG		"&Show Debug information"
#define		GSID_MENU_OPTION_WARN		(GSID_BEGIN + 2015)
#define		GSTR_MENU_OPTION_WARN		"Don't care &Warnings"
#define		GSID_MENU_OPTION_DIR		(GSID_BEGIN + 2016)
#define		GSTR_MENU_OPTION_DIR		"&Directory option"
#define		GSID_MENU_OPTION_SSC		(GSID_BEGIN + 2017)
#define		GSTR_MENU_OPTION_SSC		"Disable to generate Custom &Tooltips"

/* Help */
#define		GSID_MENU_HELP			(GSID_BEGIN + 2020)
#define		GSTR_MENU_HELP			"&Help"
#define		GSID_MENU_HELP_VERSION		(GSID_BEGIN + 2021)
#define		GSTR_MENU_HELP_VERSION		"&Version info"

/*--- Main window ---*/
#define		GSID_MAINWND_ROM_FILE		(GSID_BEGIN + 2100)
#define		GSTR_MAINWND_ROM_FILE		"ROM file"
#define		GSID_MAINWND_LIST_FILE		(GSID_BEGIN + 2101)
#define		GSTR_MAINWND_LIST_FILE		"List file"
#define		GSID_MAINWND_DEFINES		(GSID_BEGIN + 2102)
#define		GSTR_MAINWND_DEFINES		"User defines"
#define		GSID_MAINWND_INSERT		(GSID_BEGIN + 2103)
#define		GSTR_MAINWND_INSERT		"Insert"

#define		GSID_OFNFILTER_ROM_FILE		(GSID_BEGIN + 2110)
#define		GSTR_OFNFILTER_ROM_FILE		"SNES ROM file(*.smc,*.sfc,*.bin)\0*.smc;*.sfc;*.bin\0All file(*.*)\0*.*\0\0"
#define		GSID_OFNFILTER_LIST_FILE	(GSID_BEGIN + 2111)
#define		GSTR_OFNFILTER_LIST_FILE	"Text file(*.txt)\0*.txt\0All file(*.*)\0*.*\0\0"
#define		GSID_OFNFILTER_DEFAULT_ROM	(GSID_BEGIN + 2112)
#define		GSTR_OFNFILTER_DEFAULT_ROM	"rom.smc"
#define		GSID_OFNFILTER_DEFAULT_LIST	(GSID_BEGIN + 2113)
#define		GSTR_OFNFILTER_DEFAULT_LIST	"list.txt"

#define		GSID_LISTVIEW_COL_NAME		(GSID_BEGIN + 2120)
#define		GSTR_LISTVIEW_COL_NAME		"Name"
#define		GSID_LISTVIEW_COL_VALUE		(GSID_BEGIN + 2121)
#define		GSTR_LISTVIEW_COL_VALUE		"Value"

/* Common button */
#define		GSID_BUTTON_OK			(GSID_BEGIN + 2200)
#define		GSTR_BUTTON_OK			"OK"
#define		GSID_BUTTON_CANCEL		(GSID_BEGIN + 2201)
#define		GSTR_BUTTON_CANCEL		"Cancel"
#define		GSID_BUTTON_LOAD		(GSID_BEGIN + 2202)
#define		GSTR_BUTTON_LOAD		"Load"
#define		GSID_BUTTON_SAVE		(GSID_BEGIN + 2203)
#define		GSTR_BUTTON_SAVE		"Save"

/* Directory Option Dialog */
#define		GSID_DIROPTDLG			(GSID_BEGIN + 2210)
#define		GSTR_DIROPTDLG			"Directory option"
#define		GSID_DIROPTDLG_LIBRARIES	(GSID_BEGIN + 2211)
#define		GSTR_DIROPTDLG_LIBRARIES	"Libraries directory"
#define		GSID_DIROPTDLG_SPRITES		(GSID_BEGIN + 2212)
#define		GSTR_DIROPTDLG_SPRITES		"Sprites directory"
#define		GSID_DIROPTDLG_EXTENDEDES	(GSID_BEGIN + 2213)
#define		GSTR_DIROPTDLG_EXTENDEDES	"Extendedes directory"
#define		GSID_DIROPTDLG_CLUSTERS		(GSID_BEGIN + 2214)
#define		GSTR_DIROPTDLG_CLUSTERS		"Clusters directory"
#define		GSID_DIROPTDLG_ERREMPTY		(GSID_BEGIN + 2215)
#define		GSTR_DIROPTDLG_ERREMPTY		"\"%s\" is empty."

/* Information Dialog */
#define		GSID_INFDLG			(GSID_BEGIN + 2230)
#define		GSTR_INFDLG			"Information"
#define		GSID_INFDLG_VERSION		(GSID_BEGIN + 2231)
#define		GSTR_INFDLG_VERSION		"Version information"
#define		GSID_INFDLG_VER_DESC		(GSID_BEGIN + 2232)
#define		GSTR_INFDLG_VER_DESC		"%s version %0.2lf [Code: v0x%04x]\n(C)2018 boldowa"
#define		GSID_INFDLG_DEBUG		(GSID_BEGIN + 2233)
#define		GSTR_INFDLG_DEBUG		"Debug information"
#define		GSID_INFDLG_WARNING		(GSID_BEGIN + 2234)
#define		GSTR_INFDLG_WARNING		"Warning information"
#define		GSID_INFDLG_WARNING_DESC	(GSID_BEGIN + 2235)
#define		GSTR_INFDLG_WARNING_DESC	"There are warnings.\nPlease remove these as much as possible."
#define		GSID_INFDLG_ERROR		(GSID_BEGIN + 2236)
#define		GSTR_INFDLG_ERROR		"Error information"
#define		GSID_INFDLG_ERROR_DESC		(GSID_BEGIN + 2237)
#define		GSTR_INFDLG_ERROR_DESC		"There are errors.\nPlease remove these."

/* Information Dialog Tabs */
#define		GSID_INFDLG_TAB_LIBS		(GSID_BEGIN + 2250)
#define		GSTR_INFDLG_TAB_LIBS		"Libraries"
#define		GSID_INFDLG_TAB_DEBUG		(GSID_BEGIN + 2251)
#define		GSTR_INFDLG_TAB_DEBUG		"Debug"
#define		GSID_INFDLG_TAB_WARNING		(GSID_BEGIN + 2252)
#define		GSTR_INFDLG_TAB_WARNING		"Warning"
#define		GSID_INFDLG_TAB_ERROR		(GSID_BEGIN + 2253)
#define		GSTR_INFDLG_TAB_ERROR		"Error"
#define		GSID_INFDLG_TAB_FATAL		(GSID_BEGIN + 2254)
#define		GSTR_INFDLG_TAB_FATAL		"Fatal"

/* Status bar */
#define		GSID_STSBAR_READY		(GSID_BEGIN + 2260)
#define		GSTR_STSBAR_READY		"Ready"
#define		GSID_STSBAR_PROGRESS		(GSID_BEGIN + 2261)
#define		GSTR_STSBAR_PROGRESS		"Assembling ... %d / %d"

/* input error */
#define		GSID_INPUTERR_EMPTY		(GSID_BEGIN + 2270)
#define		GSTR_INPUTERR_EMPTY		"\"%s\" is empty."
#define		GSID_INPUTERR_DUPKEY		(GSID_BEGIN + 2271)
#define		GSTR_INPUTERR_DUPKEY		"Define name is duplicated."

/* Ini file error */
#define		GSID_INI_LOAD_FAILED		(GSID_BEGIN + 2280)
#define		GSTR_INI_LOAD_FAILED		"Failed to load ini file. Would you like to start with the default settings?"
#define		GSID_INI_SAVE_FAILED		(GSID_BEGIN + 2281)
#define		GSTR_INI_SAVE_FAILED		"Failed to save ini file. Please check write permission of ini file."


#endif
