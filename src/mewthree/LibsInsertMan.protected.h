#pragma once
/**
 * @file LibsInsertMan.protected.h
 */

/**
 * LibsInsertMan main instance
 */
struct _LibsInsertMan_protected {
	List* libraryFileList;
	List* detectedLabelList;
};

/**
 * detete own members method prototype
 * I recommend it that you move this method in "LibsInsertMan.c"
 * if you plan to make the final class.
 */
void delete_LibsInsertMan_members(LibsInsertMan*);

