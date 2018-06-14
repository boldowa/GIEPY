#pragma once
/**
 * @file Option.h
 */
#ifdef __cplusplus
extern "C" {
#endif

#define OPT_Hyde 0x80000000

typedef enum OptionType {
	OptionType_Unknown = 0,
	OptionType_Bool,
	OptionType_Int,
	OptionType_Float,
	OptionType_String,
	OptionType_FunctionString,
	/*================*/
	OptionType_Term,
} OptionType;

typedef struct OptionStruct {
	const char*		name;
	const char		sname;
	const char*		desc;
	const OptionType	type;
	void*			val;
} OptionStruct;

typedef bool (*SetOptFunc_t)(void*, const char*);

typedef struct SetOptStruct {
	SetOptFunc_t func;
	void* dest;
} SetOptStruct;

bool Option_Parse(int*, TCHAR***, OptionStruct*);
void Option_Usage(const OptionStruct*);

#ifdef __cplusplus
} /*extern "C" */
#endif
