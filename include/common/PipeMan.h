#pragma once
/**
 * @file PipeMan.h
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * public accessor
 */
typedef struct _PipeMan PipeMan;
typedef struct _PipeMan_protected PipeMan_protected;
struct _PipeMan {
	/* member method */
	const char* (*readline)(PipeMan*);
	/* protected members */
	PipeMan_protected* pro;
};

/**
 * Constructor
 */
PipeMan* new_PipeMan(const char* command);

/**
 * Destractor
 */
void delete_PipeMan(PipeMan**);

#ifdef __cplusplus
} /*extern "C" */
#endif
