#pragma once
/**
 * @file LibsInsertMan.h
 */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	char* libpath;
	bool isInserted;
} LibraryFileItem;

typedef struct {
	char* labelname;
	LibraryFileItem* fileItem;
} LabelSearcherItem;

/**
 * public accessor
 */
typedef struct _LibsInsertMan LibsInsertMan;
typedef struct _LibsInsertMan_protected LibsInsertMan_protected;
struct _LibsInsertMan {
	/* member methods */
	size_t (*filesCount)(LibsInsertMan*);
	size_t (*labelsCount)(LibsInsertMan*);
	void (*buildData)(LibsInsertMan*, const char*, MewEnvStruct*, Observers*);
	LibraryFileItem * const (*searchLibrary)(LibsInsertMan*, const char* labelname);
	LibraryFileItem * const (*searchFilename)(LibsInsertMan*, const char* filename);

	/* protected members */
	LibsInsertMan_protected* pro;
};

/**
 * Constructor
 */
LibsInsertMan* new_LibsInsertMan(void);

/**
 * Destractor
 */
void delete_LibsInsertMan(LibsInsertMan**);

#ifdef __cplusplus
} /*extern "C" */
#endif
