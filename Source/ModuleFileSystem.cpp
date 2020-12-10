#include "Globals.h"
#include "ModuleFileSystem.h"
#include "physfs.h"
#include <string>
#include <vector>


ModuleFileSystem::ModuleFileSystem(bool startEnable) : Module(startEnable)
{
	PHYSFS_init(nullptr);

	//Setting the working directory as the writing directory
	if (PHYSFS_setWriteDir(".") == 0)
		LOG("File System error while creating write dir: %s\n", PHYSFS_getLastError());

	//Add the current working directory to the path
	AddPath(".");
	AddPath("Assets");
	AddPath("Assets/Shaders/");
	CreateLibraryDirectories();
}

ModuleFileSystem::~ModuleFileSystem()
{
	PHYSFS_deinit();
}

/*bool ModuleFileSystem::Init()
{
	
	return true;
}*/

bool ModuleFileSystem::AddPath(const char * path)
{
	bool ret = false;
	if (!PHYSFS_mount(path, nullptr, 1)) 
	{
		LOG("Error while setting path (%s): %s\n", path, PHYSFS_getLastError());
	}
	else
		ret = true;

	return ret;
}

bool ModuleFileSystem::CreateDir(const char* dir) 
{
	if (IsDirectory(dir) == false)
	{
		PHYSFS_mkdir(dir);
		return true;
	}
	return false;
}

bool ModuleFileSystem::IsDirectory(const char * file) {
	return PHYSFS_isDirectory(file) != 0;
}

void ModuleFileSystem::CreateLibraryDirectories()
{
	CreateDir(LIBRARY_PATH);
	CreateDir(MESHES_PATH);
	CreateDir(TEXTURES_PATH);
	CreateDir(SHADERS_PATH);
}

unsigned int ModuleFileSystem::Load(const char* path, const char* file, char** buffer) const
{
	std::string full_path(path);
	full_path += file;
	return Load(full_path.c_str(), buffer);
}

unsigned int ModuleFileSystem::Load(const char* file, char** buffer) const
{
	unsigned int readedSize = 0;
	PHYSFS_File* fsFile = PHYSFS_openRead(file);

	if (fsFile != nullptr) {

		PHYSFS_sint32 fileSize = (PHYSFS_sint32)PHYSFS_fileLength(fsFile);
		if (fileSize > 0) {
			*buffer = new char[(fileSize+1)];
			readedSize = (unsigned int)PHYSFS_readBytes(fsFile, *buffer, fileSize);
			if (readedSize != fileSize)
			{
				LOG("Error while reading from vertex file %s: %s\n", file, PHYSFS_getLastError());
				delete[] (*buffer);
				*buffer = nullptr;
			}
			else {
				//TODO: Adding end of file at the end of the buffer. Loading a shader file does not add this for some reason
				(*buffer)[fileSize] = '\0';
			}
		}
		if (PHYSFS_close(fsFile) == 0)
			LOG("Error while closing vertex file %s: %s\n", file, PHYSFS_getLastError());
	}
	else {
		LOG("Error opening  file %s: %s\n",file, PHYSFS_getLastError());
	}
	
	return readedSize;
}

unsigned int ModuleFileSystem::Save(const char* file, const void* buffer, unsigned int size, bool append) const
{
	unsigned int ret = 0;

	bool overwrite = PHYSFS_exists(file) != 0;
	PHYSFS_File* fs_file = (append) ? PHYSFS_openAppend(file) : PHYSFS_openWrite(file);

	if (fs_file != nullptr)
	{
		uint written = (uint)PHYSFS_write(fs_file, (const void*)buffer, 1, size);
		if (written != size)
		{
			LOG("[error] File System error while writing to file %s: %s", file, PHYSFS_getLastError());
		}
		else
		{
			if (append == true) 
			{
				LOG("Added %u data to [%s%s]", size, PHYSFS_getWriteDir(), file);
			}
			else if (overwrite == true) 
			{
				LOG("File [%s%s] overwritten with %u bytes", PHYSFS_getWriteDir(), file, size);
			}
			else
				LOG("New file created [%s%s] of %u bytes", PHYSFS_getWriteDir(), file, size);

			ret = written;
		}

		if (PHYSFS_close(fs_file) == 0)
			LOG("[error] File System error while closing file %s: %s", file, PHYSFS_getLastError());
	}
	else
		LOG("[error] File System error while opening file %s: %s", file, PHYSFS_getLastError());

	return ret;
}

bool ModuleFileSystem::Remove(const char* file)
{
	bool ret = false;

	if (file != nullptr)
	{
		/*//TODO: Path node still not done
		//If it is a directory, we need to recursively remove all the files inside
		if (IsDirectory(file))
		{
			std::vector<std::string> containedFiles, containedDirs;
			PathNode rootDirectory = GetAllFiles(file);

			for (uint i = 0; i < rootDirectory.children.size(); ++i)
				Remove(rootDirectory.children[i].path.c_str());
		}*/

		if (PHYSFS_delete(file) != 0)
		{
			LOG("File deleted: [%s]", file);
			ret = true;
		}
		else
			LOG("File System error while trying to delete [%s]: %s", file, PHYSFS_getLastError());
	}

	return ret;
}
