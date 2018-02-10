#pragma once
/**
 * @file Mewthree.protected.h
 */

/**
 * Mewthree main instance
 */
struct _Mewthree_protected {
	RomFile* rom;
	RomMap map;
	Observers obs;
	MewInsInfo inf;
	MewEnvStruct env;
	List* inslist;
	bool isSeparateGroup;
	LibsInsertMan* libsMan;
};

/**
 * detete own members method prototype
 * I recommend it that you move this method in "Mewthree.c"
 * if you plan to make the final class.
 */
void delete_Mewthree_members(Mewthree*);

