#pragma once
/**
 * @file ParseCfg.h
 */

#include "common/CfgData.h"

#ifdef __cplusplus
extern "C" {
#endif

void* CloneTiles(const void* vsrc);
CfgData* ParseCfg(const char* cfgName, int number, Observers* obs);
void VerifyCfgData(CfgData* data);
void DiscardCfgData(CfgData** cfgp, Observers* obs);

#ifdef __cplusplus
} /*extern "C" */
#endif
