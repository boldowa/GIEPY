#pragma once
/**
 * @file Json.h
 */
#ifdef __cplusplus
extern "C" {
#endif

CfgData* ParseJson(const char* cfgPath, int number, Observers* obs);

#ifdef __cplusplus
} /*extern "C" */
#endif
