#ifndef __ModuleFileSystem_H__
#define __ModuleFileSystem_H__

#include "Module.h"
#include "Globals.h"
#include <string>
#include <vector>

class ModuleFileSystem: public Module
{
public:
	ModuleFileSystem(bool startEnable = true);
	~ModuleFileSystem();

	//bool Init();

	void CreateLibraryDirectories();
	void CreateAssetsDirectories();

	bool AddPath(const char * path);
	bool Exists(const char* file) const;
	bool CreateDir(const char* dir);
	bool IsDirectory(const char* file) const;
	void DiscoverFiles(const char* directory, std::vector<std::string>& file_list, std::vector<std::string>& dir_list) const;
	bool FileExists(const char* file);
	void SplitFilePath(const char* full_path, std::string* path, std::string* file = nullptr, std::string* extension = nullptr) const;
	std::string NormalizePath(const char* path);
	bool DuplicateFile(const char* file, const char* dstFolder, std::string& relativePath);
	bool DuplicateFile(const char* srcFile, const char* dstFile);
	

	// Open for Read/Write
	unsigned int Load(const char* path, const char* file, char** buffer) const;
	unsigned int Load(const char* file, char** buffer) const;
	unsigned int Save(const char* file, const void* buffer, unsigned int size, bool append = false) const;
	bool Remove(const char* file);

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
};


#endif //__ModuleFileSystem_H__