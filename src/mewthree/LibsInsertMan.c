/**
 * @file LibsInsertMan.c
 */
#include "common/types.h"
#include <assert.h>
#include <setjmp.h>
#include <string.h>
#if isWindows
#  include <windows.h>
#else
#  include <dirent.h>
#endif
#include "common/strres.h"
#include "common/puts.h"
#include "common/Str.h"
#include "common/List.h"
#include "common/Funex.h"
#include "common/Observer.h"
#include "file/FilePath.h"
#include "file/File.h"
#include "file/TextFile.h"
#include "file/libfile.h"
#include "mewthree/MewEnv.h"
#include "mewthree/SearchPath.h"
#include "mewthree/LibsInsertMan.h"

/* this header isn't read from anything other */
/* than inherited object.                     */ 
#include "LibsInsertMan.protected.h"

/* prototypes */
static void DeleteLibraryFileItem(void* data);
static void DeleteLabelSearcherItem(void* data);

static size_t filesCount(LibsInsertMan*);
static size_t labelsCount(LibsInsertMan*);
static void buildData(LibsInsertMan* self, const char* libdir, MewEnvStruct* env, Observers* obs);
static LibraryFileItem * const searchLibrary(LibsInsertMan*, const char* labelname);
static LibraryFileItem * const searchFilename(LibsInsertMan* self, const char* filename);


/*--------------- Constructor / Destructor ---------------*/

/**
 * @brief Create LibsInsertMan object
 *
 * @return the pointer of object
 */
LibsInsertMan* new_LibsInsertMan(void)
{
	LibsInsertMan* self;
	LibsInsertMan_protected* pro;
	List* libraryFileList;
	List* detectedLabelList;

	/* make objects */
	self = malloc(sizeof(LibsInsertMan));
	pro = malloc(sizeof(LibsInsertMan_protected));
	libraryFileList = new_List(NULL, DeleteLibraryFileItem);
	detectedLabelList = new_List(NULL, DeleteLabelSearcherItem);

	/* check whether object creatin succeeded */
	assert(pro);
	assert(self);
	assert(libraryFileList);
	assert(detectedLabelList);

	/*--- set protected member ---*/
	pro->libraryFileList = libraryFileList;
	pro->detectedLabelList = detectedLabelList;

	/*--- set public member ---*/
	self->filesCount = filesCount;
	self->labelsCount = labelsCount;
	self->buildData = buildData;
	self->searchLibrary = searchLibrary;
	self->searchFilename = searchFilename;

	/* init LibsInsertMan object */
	self->pro = pro;
	return self;
}

/**
 * @brief delete own member variables
 *
 * @param the pointer of object
 */
void delete_LibsInsertMan_members(LibsInsertMan* self)
{
	/* delete protected members */
	delete_List(&self->pro->libraryFileList);
	delete_List(&self->pro->detectedLabelList);
	free(self->pro);
}

/**
 * @brief Delete LibsInsertMan object
 *
 * @param the pointer of object
 */
void delete_LibsInsertMan(LibsInsertMan** self)
{
	/* This is the template that default destractor. */
	assert(self);
	if(NULL == (*self)) return;
	delete_LibsInsertMan_members(*self);
	free(*self);
	(*self) = NULL;
}


/*--------------- internal methods ---------------*/

static bool SearchLabel(const void *search, const void *listv)
{
	const char* labelname = (const char*)search;
	LabelSearcherItem* labelitem = (LabelSearcherItem*)listv;

	if(0 == strcmp(labelname, labelitem->labelname))
	{
		return true;
	}
	return false;
}

static bool SearchFile(const void *search, const void *listv)
{
	const char* filename = (const char*)search;
	LibraryFileItem* fitem = (LibraryFileItem*)listv;
	FilePath* fp;
	bool result;

	fp = new_FilePath(fitem->libpath);
	result = (0 == strcmp(filename, fp->name_get(fp)));
	delete_FilePath(&fp);

	return result;
}

static LibraryFileItem* CreateLibraryFileItem(const char* fpath)
{
	LibraryFileItem * item;
	char *path;

	assert(fpath);
	path = Str_copy(fpath);
	item = malloc(sizeof(LibraryFileItem));
	assert(path);
	assert(item);

	item->isInserted = false;
	item->libpath = path;

	return item;
}

static void DeleteLibraryFileItem(void* data)
{
	LibraryFileItem* item = (LibraryFileItem*)data;
	assert(item);

	free(item->libpath);
	free(item);
}

static LabelSearcherItem* CreateLabelSearcherItem(LibraryFileItem* fileItem, const char* labelname)
{
	LabelSearcherItem * item;
	char *label;

	assert(fileItem);
	assert(labelname);
	label = Str_copy(labelname);
	item = malloc(sizeof(LabelSearcherItem));
	assert(label);
	assert(item);

	item->labelname = label;
	item->fileItem = fileItem;

	return item;
}

static void DeleteLabelSearcherItem(void* data)
{
	LabelSearcherItem* item = (LabelSearcherItem*)data;
	assert(item);

	free(item->labelname);
	free(item);
}

static size_t filesCount(LibsInsertMan* self)
{
	List* lst;

	assert(self);
	lst = self->pro->libraryFileList;
	return lst->length(lst);

}

static size_t labelsCount(LibsInsertMan* self)
{
	List* lst;

	assert(self);
	lst = self->pro->detectedLabelList;
	return lst->length(lst);
}

static bool CreateSearchPath(char** dirs, const char* libdir, MewEnvStruct* env, Observers* obs)
{
	size_t len;
	char* path;
	len = strlen(libdir);
	path = calloc(len+2, sizeof(char));
	assert(path);
	strcpy_s(path, len+2, libdir);
#if isWindows
	if(('\\' != path[len-1]) || ('/' != path[len-1]))
	{
		path[len] = '\\';
		path[len+1] = '\0';
	}
#else
	if('/' != path[len-1])
	{
		path[len] = '/';
		path[len+1] = '\0';
	}
#endif
	if(false == ConstructSearchPath(dirs, path, env, obs))
	{
		return false;
	}
	free(path);

	return true;
}

static char* GetLabel(const char* line)
{
	char* work;
	char* label;
	char* s;
	size_t len;
	size_t i;

	work = Str_copy(line);

	/* remove comment */
	s = strstr(work, ";");
	if(NULL != s)
	{
		*s = '\0';
	}
	len = strlen(work);
	for(i = len-1; i>=0; i--)
	{
		if(false == IsSpace(work[i]))
		{
			break;
		}
		work[i] = '\0';
	}

	/* label match */
	s = strstr(work, ":");
	if(NULL == s)
	{
		free(work);
		return NULL;
	}

	*s = '\0';
	i = 0;
	len = strlen(work);
	SkipSpaces(work, &i, len);
	if(i == len)
	{
		free(work);
		return NULL;
	}

	if('_' == work[i])
	{
		free(work);
		return NULL;
	}

	label = Str_copy(&work[i]);
	free(work);

	return label;
}

static void ReadAsmFile(LibraryFileItem* fileItem, List* labelList, const char* path, Observers* obs)
{
	jmp_buf e;
	TextFile* asmFile;
	const char* linebuf;
	char* label;
	LabelSearcherItem* item;

	assert(fileItem);
	assert(labelList);
	assert(path);

	obs->debug(0, GSID_LIBRARY_FOUND, path);
	if(0 == setjmp(e))
	{
		asmFile = new_TextFile(path);
		if(NULL == asmFile)
		{
			longjmp(e, 1);
		}

		if(FileOpen_NoError != asmFile->Open(asmFile))
		{
			obs->warn(0, GSID_LIBRARY_CANT_OPEN, asmFile->super.path_get(&asmFile->super));
			longjmp(e, 1);
		}

		linebuf = asmFile->GetLine(asmFile);
		while(NULL != linebuf)
		{
			/* match func */
			label = GetLabel(linebuf);
			if(NULL != label)
			{
				if(NULL == labelList->search(labelList, label, SearchLabel))
				{
					item = CreateLabelSearcherItem(fileItem, label);
					labelList->push(labelList, item);
				}
				free(label);
			}
			linebuf = asmFile->GetLine(asmFile);
		}

		asmFile->super.Close(&asmFile->super);
	}
	else
	{
		/* nothing to do */
	}
	delete_TextFile(&asmFile);
}

static void ReadLibraryFiles(LibsInsertMan* self, char* const * dirs, Observers* obs)
{
#if isWindows
	int i;
	HANDLE hFind;
	WIN32_FIND_DATA win32fd;
	char* path = NULL;
	char* s;
	List* fileList;
	LibraryFileItem* item;

	assert(self);
	fileList = self->pro->libraryFileList;
	for(i=0; NULL != dirs[i]; i++)
	{
		path = Str_concat(dirs[i], "*.asm");

		hFind = FindFirstFile(path, &win32fd);
		if(INVALID_HANDLE_VALUE == hFind)
		{
			free(path);
			continue;
		}

		do{
			s = Str_concat(dirs[i], win32fd.cFileName);
			item = CreateLibraryFileItem(s);
			ReadAsmFile(item, self->pro->detectedLabelList, s, obs);
			fileList->push(fileList, item);
			free(s);
		} while(FindNextFile(hFind, &win32fd));
		free(path);
	}
#else
	int i;
	FilePath* fp;
	char* ext;
	DIR *dir;
	struct dirent *dp;
	List* fileList;
	LibraryFileItem* item;

	assert(self);
	fileList = self->pro->libraryFileList;
	for(i=0; NULL != dirs[i]; i++)
	{
		dir = opendir(dirs[i]);
		if(NULL == dir)
		{
			continue;
		}

		for(dp = readdir(dir); NULL != dp; dp = readdir(dir))
		{
			{
				char* p;
				p = Str_concat(dirs[i], dp->d_name);
				fp = new_FilePath(p);
				free(p);
			}
			ext = Str_copy(fp->ext_get(fp));
			Str_tolower(ext);

			if(0 != strcmp(".asm", ext))
			{
				free(ext);
				delete_FilePath(&fp);
				continue;
			}
			free(ext);
			item = CreateLibraryFileItem(fp->path_get(fp));
			ReadAsmFile(item, self->pro->detectedLabelList, fp->path_get(fp), obs);
			fileList->push(fileList, item);
			delete_FilePath(&fp);
		}
	}
#endif
}

static void buildData(LibsInsertMan* self, const char* libdir, MewEnvStruct* env, Observers* obs)
{
	char* dirs[SPMAX];

	assert(self);
	if(false == CreateSearchPath(dirs, libdir, env, obs))
	{
		return;
	}

	ReadLibraryFiles(self, dirs, obs);
	DestroySearchPath(dirs);

	if(0 == self->filesCount(self))
	{
		obs->debug(0, GSID_LIBRARY_NOTHING);
		return;
	}

	obs->debug(0, GSID_LIBRARY_FILE_NUMBERS, self->filesCount(self));
	obs->debug(0, GSID_LIBRARY_LABEL_NUMBERS, self->labelsCount(self));
}

static LibraryFileItem * const searchLibrary(LibsInsertMan* self, const char* labelname)
{
	List* labelList;
	LabelSearcherItem* labelItem;
	Iterator* it;

	assert(self);
	labelList = self->pro->detectedLabelList;
	it = labelList->search(labelList, labelname, SearchLabel);
	if(NULL == it)
	{
		return NULL;
	}

	labelItem = (LabelSearcherItem*)it->data(it);
	return labelItem->fileItem;
}


static LibraryFileItem * const searchFilename(LibsInsertMan* self, const char* filename)
{
	List* fileList;
	Iterator* it;

	assert(self);
	fileList = self->pro->libraryFileList;
	it = fileList->search(fileList, filename, SearchFile);
	if(NULL == it)
	{
		return NULL;
	}

	return (LibraryFileItem*)it->data(it);
}

