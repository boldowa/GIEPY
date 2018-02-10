#pragma once
/**
 * @file ParseCfg.h
 */

#include "common/CfgData.h"

CfgData* ParseCfg(const char* cfgName, int number, Observers* obs);
void VerifyCfgData(CfgData* data);
void DiscardCfgData(CfgData** cfgp, Observers* obs);


