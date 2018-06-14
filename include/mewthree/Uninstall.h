#pragma once
/**
 * @file Uninstall.h
 */
#ifdef __cplusplus
extern "C" {
#endif

bool UninstallMainData(RomFile*, MewInsInfo*, Observers*);
bool CleanData(RomFile*, MewInsInfo*, Observers*);
bool UninstallAllData(RomFile*, MewInsInfo*, Observers*);

#ifdef __cplusplus
} /*extern "C" */
#endif
