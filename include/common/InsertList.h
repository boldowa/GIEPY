#pragma once
/**
 * @file InsertList.h
 */
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	SpriteType_Undefined = 0,
	SpriteType_Sprite,
	SpriteType_Extended,
	SpriteType_Cluster,
	SpriteType_Overworld,
}SpriteType;

typedef struct _InsertInf {
	char*	path;
	uint32	insloc;
	uint32	initloc;
	uint32	mainloc;
}InsertInf;

typedef struct _InsertItem {
	SpriteType		type;
	int			grp;
	int			number;
	char*			fname;
	InsertInf		insinf;	/* for asm specify sprites(e.g. extended) */
	struct _CfgData*	cfg;	/* use insint in this one if use cfg sprites */
}InsertItem;

#ifdef __cplusplus
} /*extern "C" */
#endif
