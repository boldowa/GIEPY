/**
 * @file ParseCmdDefs.h
 */
#pragma once

#include "common/defines.h"

bool ParseCmdDefs(void* dst, const char* cmdline);

/**
 * For list create/destroy
 */
void* CloneDefine(const void* srcv);
void DelDefine(void* tgt);

