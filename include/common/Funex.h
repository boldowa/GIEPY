#pragma once
/**
 * @file Funex.h
 */

#include "common/Observer.h"

typedef bool (*FunexFunc_t)(const char*, void*);
typedef bool (*FunexFuncObs_t)(const char*, void*, Observers*);

typedef struct FunexStruct {
	FunexFunc_t	func;
	void*		match;
} FunexStruct;

typedef struct FunexStructObs {
	FunexFuncObs_t	func;
	void*		match;
} FunexObsStruct;

/* Main */
int FunexMatch(const char*, FunexStruct*);
int FunexMatchObs(const char*, FunexObsStruct*, Observers*);

/* Funex libs */
bool ishex(const char);
int atoh(const char);
bool IsSpace(const char);
void CutOffTailSpaces(char*);
void SkipSpaces(const char*, size_t*, const size_t);
void SkipUntilSpaces(const char*, size_t*, const size_t);
void SkipSpacesRev(const char*, size_t*);
void SkipUntilChar(const char*, size_t*, const char, const size_t);
