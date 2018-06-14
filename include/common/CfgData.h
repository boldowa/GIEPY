#pragma once
/**
 * @file CfgData.h
 */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _TagData {
	char*		name;
	char*		description;
	char*		label;
	int		x;
	int		y;
	List*		tiles;
} TagData;
typedef struct _CfgData {
	char*		basename;
	char*		path;
	uint8		new_code_flag;		/* line  1 */
	uint8		orig_sprite_number;	/* line  2 */
	uint8		tweaks[6];		/* line  3 */
	uint8		extra_props[2];		/* line  4 */
	char*		asm_name1;		/* line  5 */
	InsertInf	insinf1;
	/*uint8		asm_type;*/		/* line  6 */
	uint8		sprite_type;		/* line  7 */
	uint8		unique_info;		/* line  8 */
	uint8		extra_byte_nums;	/* line  9 */
	char*		asm_name2;		/* line 10 */
	InsertInf	insinf2;
	TagData		tag;
}CfgData;

#ifdef __cplusplus
} /*extern "C" */
#endif
