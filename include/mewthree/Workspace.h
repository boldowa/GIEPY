#pragma once
/**
 * @file Workspace.h
 */
#ifdef __cplusplus
extern "C" {
#endif

void InitializeWorkspace(MewEnvStruct* env, MewInsInfo* inf, List* defs);
void CleanWorkspace(MewEnvStruct* env);

#ifdef __cplusplus
} /*extern "C" */
#endif
