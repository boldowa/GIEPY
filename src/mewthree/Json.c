/**
 * @file Json.c
 *   In this source, json is decoded using the following library.
 *   - Jansson : https://github.com/akheron/jansson
 */
#include "common/types.h"
#include "jansson.h"
#include <setjmp.h>
#include <assert.h>
#include "common/Str.h"
#include "common/strres.h"
#include "common/Observer.h"
#include "common/InsertList.h"
#include "mewthree/ParseCfg.h"

#include "mewthree/Json.h"

/* $1656 */
#define Key1656_b0	"Object Clipping"
#define Key1656_b4	"Can be jumped on"
#define Key1656_b5	"Dies when jumped on"
#define Key1656_b6	"Hop in /kick shell"
#define Key1656_b7	"Disappears in cloud of smoke"

/* $1662 */
#define Key1662_b0	"Sprite Clipping"
#define Key1662_b6	"Use shell as death frame"
#define Key1662_b7	"Fall straight down when killed"

/* $166E */
#define Key166E_b0	"Use second graphics page"
#define Key166E_b1	"Palette"
#define Key166E_b4	"Disable fireball killing"
#define Key166E_b5	"Disable cape killing"
#define Key166E_b6	"Disable water splash"
#define Key166E_b7	"Don't interact with Layer 2"

/* $167A */
#define Key167A_b0	"Don't disable cliping when starkilled"
#define Key167A_b1	"Invincible to star/cape/fire/bounce blk"
#define Key167A_b2	"Process when off screen"
#define Key167A_b3	"Don't change into shell when stunned"
#define Key167A_b4	"Can't be kicked like shell"
#define Key167A_b5	"Process interaction with Mario every frame"
#define Key167A_b6	"Gives power-up when eaten by Yoshi"
#define Key167A_b7	"Don't use default interaction with Mario"

/* $1686 */
#define Key1686_b0	"Inedible"
#define Key1686_b1	"Stay in Yoshi's mouth"
#define Key1686_b2	"Weird ground behaviour"
#define Key1686_b3	"Don't interact with other sprites"
#define Key1686_b4	"Don't change direction if touched"
#define Key1686_b5	"Don't turn into coin when goal passed"
#define Key1686_b6	"Spawn a new sprite"
#define Key1686_b7	"Don't interact with objects"

/* $190F */
#define Key190F_b0	"Make platform passable from below"
#define Key190F_b1	"Don't erase when goal passed"
#define Key190F_b2	"Can't be killed by sliding"
#define Key190F_b3	"Take 5 fireballs to kill"
#define Key190F_b4	"Can't be jumped on with upwards Y speed"
#define Key190F_b5	"Death frame two tiles high"
#define Key190F_b6	"Don't turn into a coin with silver POW"
#define Key190F_b7	"Don't get stuck in walls (carryable sprites)"

static char* JsonGetString(json_t* json, const char* key)
{
	json_t* jitem;
	const char* str;
	char* result;

	result = NULL;
	jitem = json_object_get(json, key);
	if(NULL != jitem)
	{
		if(json_is_string(jitem))
		{
			str = json_string_value(jitem);
			if(NULL != str)
			{
				if(0 != strlen(str))
				{
					result = Str_copy(str);
				}
			}
		}
		/*json_decref(jitem);*/
	}

	return result;
}

static uint8 JsonGetBool(json_t* json, const char* key)
{
	json_t* jitem;
	uint8 result;

	result = 0;
	jitem = json_object_get(json, key);
	if(NULL != jitem)
	{
		if(json_is_boolean(jitem))
		{
			result = (uint8)json_boolean_value(jitem);
		}
		/*json_decref(jitem);*/
	}

	return result;
}

static int JsonGetInt(json_t* json, const char* key)
{
	json_t* jitem;
	int result;

	result = 0;
	jitem = json_object_get(json, key);
	if(NULL != jitem)
	{
		if(json_is_integer(jitem))
		{
			result = (int)json_integer_value(jitem);
		}
		/*json_decref(jitem);*/
	}

	return result;
}

static uint8 Get1656(json_t* json)
{
	uint8 result;

	result  = (uint8)JsonGetInt(json, Key1656_b0);
	result |= (uint8)(JsonGetBool(json, Key1656_b4) << 4);
	result |= (uint8)(JsonGetBool(json, Key1656_b5) << 5);
	result |= (uint8)(JsonGetBool(json, Key1656_b6) << 6);
	result |= (uint8)(JsonGetBool(json, Key1656_b7) << 7);

	return result;
}

static uint8 Get1662(json_t* json)
{
	uint8 result;

	result  = (uint8)JsonGetInt(json, Key1662_b0);
	result |= (uint8)(JsonGetBool(json, Key1662_b6) << 6);
	result |= (uint8)(JsonGetBool(json, Key1662_b7) << 7);

	return result;
}

static uint8 Get166E(json_t* json)
{
	uint8 result;

	result  = (uint8)JsonGetBool(json, Key166E_b0);
	result |= (uint8)(JsonGetInt(json, Key166E_b1) << 1);
	result |= (uint8)(JsonGetBool(json, Key166E_b4) << 4);
	result |= (uint8)(JsonGetBool(json, Key166E_b5) << 5);
	result |= (uint8)(JsonGetBool(json, Key166E_b6) << 6);
	result |= (uint8)(JsonGetBool(json, Key166E_b7) << 7);

	return result;
}

static uint8 Get167A(json_t* json)
{
	uint8 result;

	result  = (uint8)JsonGetBool(json, Key167A_b0);
	result |= (uint8)(JsonGetBool(json, Key167A_b1) << 1);
	result |= (uint8)(JsonGetBool(json, Key167A_b2) << 2);
	result |= (uint8)(JsonGetBool(json, Key167A_b3) << 3);
	result |= (uint8)(JsonGetBool(json, Key167A_b4) << 4);
	result |= (uint8)(JsonGetBool(json, Key167A_b5) << 5);
	result |= (uint8)(JsonGetBool(json, Key167A_b6) << 6);
	result |= (uint8)(JsonGetBool(json, Key167A_b7) << 7);

	return result;
}

static uint8 Get1686(json_t* json)
{
	uint8 result;

	result  = (uint8)JsonGetBool(json, Key1686_b0);
	result |= (uint8)(JsonGetBool(json, Key1686_b1) << 1);
	result |= (uint8)(JsonGetBool(json, Key1686_b2) << 2);
	result |= (uint8)(JsonGetBool(json, Key1686_b3) << 3);
	result |= (uint8)(JsonGetBool(json, Key1686_b4) << 4);
	result |= (uint8)(JsonGetBool(json, Key1686_b5) << 5);
	result |= (uint8)(JsonGetBool(json, Key1686_b6) << 6);
	result |= (uint8)(JsonGetBool(json, Key1686_b7) << 7);

	return result;
}

static uint8 Get190F(json_t* json)
{
	uint8 result;

	result  = (uint8)JsonGetBool(json, Key190F_b0);
	result |= (uint8)(JsonGetBool(json, Key190F_b1) << 1);
	result |= (uint8)(JsonGetBool(json, Key190F_b2) << 2);
	result |= (uint8)(JsonGetBool(json, Key190F_b3) << 3);
	result |= (uint8)(JsonGetBool(json, Key190F_b4) << 4);
	result |= (uint8)(JsonGetBool(json, Key190F_b5) << 5);
	result |= (uint8)(JsonGetBool(json, Key190F_b6) << 6);
	result |= (uint8)(JsonGetBool(json, Key190F_b7) << 7);

	return result;
}

static uint8 GetTweaks(json_t* json, const char* key, uint8 (*gets)(json_t*))
{
	json_t* jitem;
	uint8 result;

	result = 0;
	jitem = json_object_get(json, key);
	if(NULL != jitem)
	{
		result = gets(jitem);
		/*json_decref(jitem);*/
	}

	return result;
}

static char* con(const char* s1, const char* s2)
{
	size_t s1len, s2len, len;
	char* newStr;

	s1len = strlen(s1);
	s2len = strlen(s2);
	len = s1len + s2len;

	newStr = calloc(len+3, sizeof(len));
	assert(newStr);

	strcpy_s(&newStr[0], s1len+3, s1);
	strcpy_s(&newStr[s1len], s2len+3, s2);

	newStr[len+0] = '\\';
	newStr[len+1] = 'n';
	newStr[len+2] = '\0';
	return newStr;
}

static char* GetStringArray(json_t* json, const char* key)
{
	jmp_buf e;
	char* desc;
	char* tmp;
	const char* str;
	size_t len;
	size_t i;
	json_t* jarray;
	json_t* jstr;

	jarray = json_object_get(json, key);
	if(NULL == jarray || !json_is_array(jarray)) return NULL;

	len = json_array_size(jarray);
	if(0 == len) return NULL;

	desc = calloc(1,sizeof(char));
	assert(desc);

	if(0 == setjmp(e))
	{
		for(i=0;i<len;i++)
		{
			jstr = json_array_get(jarray, i);
			if(!json_is_string(jstr)) longjmp(e,1);

			str = json_string_value(jstr);
			if(NULL == str) longjmp(e,1);

			tmp = con(desc, str);
			if(NULL == tmp) longjmp(e,1);
			free(desc);
			desc = tmp; tmp=NULL;
		}
	}
	else
	{
		free(desc);
		return NULL;
	}

	len = strlen(desc);
	if(1<len)
	{
		desc[len-2] = '\0';
		desc[len-1] = '\0';
	}
	return desc;
}

void GetTiles(json_t* json, const char* key, List* tiles)
{
	size_t len;
	size_t i;
	json_t* jarray;
	json_t* jtiles;
	int* v;

	jarray = json_object_get(json, key);
	if(NULL == jarray || !json_is_array(jarray)) return;

	len = json_array_size(jarray);
	if(0 == len) return;

	for(i=0;i<len;i++)
	{
		jtiles = json_array_get(jarray, i);
		if(NULL==jtiles) return;

		v = malloc(3*sizeof(int));
		if(NULL==v) return;

		v[0] = JsonGetInt(jtiles, "X");
		v[1] = JsonGetInt(jtiles, "Y");
		v[2] = JsonGetInt(jtiles, "Tile");

		tiles->push(tiles,v);
	}
}

/**
 * @brief Decode the json file
 *
 * @param jsonPath json file path
 * @param number sprite insert number
 * @param obs Observer
 *
 * @return If it succeeds, return CfgData object pointer.
 */
CfgData* ParseJson(const char* jsonPath, int number, Observers* obs)
{
	CfgData* data;
	jmp_buf e;
	json_error_t jerror;
	json_t* jroot = NULL;
	json_t* j;
	bool isDetectType = false;

	data = calloc(1, sizeof(CfgData));
	assert(data);
	data->tag.x = 7;
	data->tag.y = 7;
	data->tag.tiles = new_List(CloneTiles, free);
	assert(data->tag.tiles);

	if(0 == setjmp(e)) /* try */
	{
		/* load json file */
		jroot = json_load_file(jsonPath, 0, &jerror);
		if(NULL == jroot)
		{
			/* parse failure */
			longjmp(e,1);
		}

		/*--- get json parameter ---*/
		/* new_code_flag */
		data->new_code_flag = (uint8)JsonGetInt(jroot, "Type");

		/* origin sprite number */
		data->orig_sprite_number = (uint8)JsonGetInt(jroot, "ActLike");

		/* get tweaks */
		data->tweaks[0] = GetTweaks(jroot, "$1656", Get1656);
		data->tweaks[1] = GetTweaks(jroot, "$1662", Get1662);
		data->tweaks[2] = GetTweaks(jroot, "$166E", Get166E);
		data->tweaks[3] = GetTweaks(jroot, "$167A", Get167A);
		data->tweaks[4] = GetTweaks(jroot, "$1686", Get1686);
		data->tweaks[5] = GetTweaks(jroot, "$190F", Get190F);

		/* get extra props */
		data->extra_props[0] = (uint8)JsonGetInt(jroot, "Extra Property Byte 1");
		data->extra_props[1] = (uint8)JsonGetInt(jroot, "Extra Property Byte 2");

		/* get asm name */
		data->asm_name1 = JsonGetString(jroot, "AsmFile");

		/* get sprite subtype (GIEPY INFO) */
		j = json_object_get(jroot, "SubType");
		if(NULL != j)	/* param exists check */
		{
			isDetectType = true;
			/*json_decref(j);*/
		}
		data->sprite_type = (uint8)JsonGetInt(jroot, "SubType");

		/* get unique info (GIEPY INFO) */
		data->unique_info = (uint8)JsonGetInt(jroot, "Unique Info");

		/* get extra bytes length (GIEPY INFO) */
		data->extra_byte_nums = (uint8)JsonGetInt(jroot, "Extra Bytes Length");

		/* get asm name2 (GIEPY INFO) */
		data->asm_name2 = JsonGetString(jroot, "AsmFile2");

		/* get X,Y */
		data->tag.x = JsonGetInt(jroot, "X");
		data->tag.y = JsonGetInt(jroot, "Y");

		/* get sprite name for ssc */
		data->tag.name = JsonGetString(jroot, "Name");

		/* get label text for ssc */
		data->tag.label = GetStringArray(jroot, "Label");
		if(NULL == data->tag.label)
		{
			GetTiles(jroot, "Tiles", data->tag.tiles);
		}

		/* get description for ssc */
		data->tag.description = GetStringArray(jroot, "Description");

		/* release objects */
		json_decref(jroot);

		if(false == isDetectType)
		{
			/* old type(PIXI) json file. */
			if(data->new_code_flag)
			{
				if(0xc0>number || 0xdf<number)
				{
					obs->warn(0, GSID_PARSECFG_OLD_FORMAT_SP, jsonPath);
					data->sprite_type = 1;
				}
				else if(0xd0>number)
				{
					obs->warn(0, GSID_PARSECFG_OLD_FORMAT_SH, jsonPath);
					data->sprite_type = 2;
				}
				else
				{
					obs->warn(0, GSID_PARSECFG_OLD_FORMAT_GEN, jsonPath);
					data->sprite_type = 3;
				}
			}
		}
	}
	else /* catch */
	{
		if(NULL != jroot) json_decref(jroot);
		DiscardCfgData(&data, obs);
		return NULL;
	}

	/* verify */
	VerifyCfgData(data);

	/* read ok, succeeded. */
	return data;
}

