#ifndef __ModuleFileSystem_H__
#define __ModuleFileSystem_H__

#include "Module.h"
#include "Globals.h"

class ModuleFileSystem: public Module
{
public:
	ModuleFileSystem(bool startEnable = true);
	~ModuleFileSystem();

	//bool Init();

	bool AddPath(const char * path);
	bool CreateDir(const char* dir);
	bool IsDirectory(const char* file);
	void CreateLibraryDirectories();

	// Open for Read/Write
	unsigned int Load(const char* path, const char* file, char** buffer) const;
	unsigned int Load(const char* file, char** buffer) const;
	unsigned int Save(const char* file, const void* buffer, unsigned int size, bool append = false) const;
	bool Remove(const char* file);
};


#endif //__ModuleFileSystem_H__