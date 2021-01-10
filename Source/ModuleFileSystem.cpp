#include "Globals.h"
#include "ModuleFileSystem.h"
#include "physfs.h"
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include "parson.h"
#include "Application.h"
#include "ModuleResources.h"


ModuleFileSystem::ModuleFileSystem(bool startEnable) : Module(startEnable)
{
	PHYSFS_init(nullptr);

	//Setting the working directory as the writing directory
	if (PHYSFS_setWriteDir(".") == 0)
		LOG("File System error while creating write dir: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));

	//Add the current working directory to the path
	AddPath(".");
	//TODO: Add the assets path ??
	CreateAssetsDirectories();
	AddPath(ASSETS_PATH);
	AddPath(ASSETS_MESHES);
	AddPath(ASSETS_MODELS);
	AddPath(ASSETS_TEXTURES);
	AddPath(ASSETS_SHADERS);
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
		LOG("Error while setting path (%s): %s\n", path, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}
	else
		ret = true;

	return ret;
}

bool ModuleFileSystem::Exists(const char* file) const
{
	return PHYSFS_exists(file) != 0;
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

bool ModuleFileSystem::IsDirectory(const char * file) const {
	PHYSFS_Stat stat;
	if (!PHYSFS_stat(file, &stat))
		LOG("Error obtaining file/dir stat: %s", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	return (stat.filetype == PHYSFS_FileType::PHYSFS_FILETYPE_DIRECTORY);
}

int ModuleFileSystem::LastModificationTime(const char* file) const
{
	PHYSFS_Stat stat;
	if (!PHYSFS_stat(file, &stat))
		LOG("Error obtaining file/dir stat: %s", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	return stat.modtime;
}

void ModuleFileSystem::CreateLibraryDirectories()
{
	CreateDir(LIBRARY_PATH);
	CreateDir(MODELS_PATH);
	CreateDir(MESHES_PATH);
	CreateDir(TEXTURES_PATH);
	CreateDir(SHADERS_PATH);
	CreateDir(SCENES_PATH);
	CreateDir(TEMP_PATH);
	CreateDir(SCRIPTS_PATH);
}

void ModuleFileSystem::CreateAssetsDirectories()
{
	CreateDir(ASSETS_PATH);
	CreateDir(ASSETS_MESHES);
	CreateDir(ASSETS_MODELS);
	CreateDir(ASSETS_TEXTURES);
	CreateDir(ASSETS_SHADERS);
	CreateDir(ASSETS_SCENES);
	CreateDir(ASSETS_SCRIPTS);
}

bool ModuleFileSystem::FileExists(const char* file)
{
	return PHYSFS_exists(file) != 0;
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
				LOG("Error while reading from vertex file %s: %s\n", file, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
				delete[] (*buffer);
				*buffer = nullptr;
			}
			else {
				//TODO: Adding end of file at the end of the buffer. Loading a shader file does not add this for some reason
				(*buffer)[fileSize] = '\0';
			}
		}
		if (PHYSFS_close(fsFile) == 0)
			LOG("Error while closing vertex file %s: %s\n", file, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}
	else {
		LOG("Error opening  file %s: %s\n",file, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
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
		uint written = (uint)PHYSFS_writeBytes(fs_file, (const void*)buffer,size);
		if (written != size)
		{
			LOG("[error] File System error while writing to file %s: %s", file, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
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
			LOG("[error] File System error while closing file %s: %s", file, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}
	else
		LOG("[error] File System error while opening file %s: %s", file, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));

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
		if (Exists(file)) {
			if (PHYSFS_delete(file) != 0)
			{
				LOG("File deleted: [%s]", file);
				ret = true;
			}
			else
				LOG("File System error while trying to delete [%s]: %s", file, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		}
	}

	return ret;
}

bool ModuleFileSystem::DeleteDir(const char* dirName)
{
	std::vector<std::string>files;
	std::vector<std::string>dirs;
	DiscoverFiles(dirName, files, dirs);
	for (int i = 0; i < files.size(); ++i){
		files[i] = dirName + files[i];
		Remove(files[i].c_str());
	}
	for (int i = 0; i < dirs.size(); ++i) {
		DeleteDir(dirs[i].c_str());
	}
	Remove(dirName);
	return true;
}

bool ModuleFileSystem::Start()
{
	return true;
}

update_status ModuleFileSystem::Update(float dt)
{
	currentWatchTime -= dt;
	if (currentWatchTime <= 0) {
		bool reloaded = false;
		for (std::vector<std::string>::iterator it = watchFiles.begin(); it != watchFiles.end(); ++it) 
		{
			std::string path;
			std::string name;
			std::string extension;
			SplitFilePath((*it).c_str(), &path, &name, &extension);
			char* buffer = nullptr;
			std::string metaPath = path + name + ".meta";
			Load(metaPath.c_str(), &buffer);
			JSON_Value* rootValue = json_parse_string(buffer);
			JSON_Object* node = json_value_get_object(rootValue);
			unsigned int metaModification = json_object_get_number(node, "LastModification");
			int lastModification = LastModificationTime((*it).c_str());
			if (metaModification != lastModification && lastModification != -1) 
			{
				int resourceID = json_object_get_number(node, "LIBUID");
				if (App->resources->GetResourceCount(resourceID) > 0) 
				{
					App->resources->HotReloadDll(resourceID,(*it).c_str());
					reloaded = true;
				}
				
				json_object_set_number(node, "LastModification", LastModificationTime((*it).c_str()));
				size_t size = json_serialization_size_pretty(rootValue);
				char* secondBuffer = new char[size];
				json_serialize_to_buffer_pretty(rootValue, secondBuffer, size);
				App->fileSystem->Save(metaPath.c_str(), secondBuffer, size);
				delete[] secondBuffer; secondBuffer = nullptr;
			}
			json_value_free(rootValue);
			delete[] buffer;
			buffer = nullptr;
			if (reloaded)
				break;
		}
		currentWatchTime = watchTime;
	}
	return update_status::UPDATE_CONTINUE;
}

bool ModuleFileSystem::CleanUp()
{
	DeleteDir(TEMP_PATH);
	return true;
}

void ModuleFileSystem::SplitFilePath(const char* full_path, std::string* path, std::string* file, std::string* extension) const
{
	if (full_path != nullptr)
	{
		std::string full(full_path);
		size_t pos_separator = full.find_last_of("\\/");
		size_t pos_dot = full.find_last_of(".");

		if (path != nullptr)
		{
			if (pos_separator < full.length())
				*path = full.substr(0, pos_separator + 1);
			else
				path->clear();
		}

		if (file != nullptr)
		{
			if (pos_separator < full.length())
				*file = full.substr(pos_separator + 1, pos_dot - pos_separator - 1);
			else
				*file = full.substr(0, pos_dot);
		}

		if (extension != nullptr)
		{
			if (pos_dot < full.length())
				*extension = full.substr(pos_dot + 1);
			else
				extension->clear();
		}
	}
}

std::string ModuleFileSystem::NormalizePath(const char* path)
{
	std::string filePath = path;
	std::replace(filePath.begin(), filePath.end(), '\\', '/');
	return filePath;
}

bool ModuleFileSystem::DuplicateFile(const char* file, const char* dstFolder, std::string& relativePath)
{
	std::string fileStr, extensionStr;
	SplitFilePath(file, nullptr, &fileStr, &extensionStr);

	relativePath = relativePath.append(dstFolder).append("/") + fileStr.append(".") + extensionStr;
	std::string finalPath = std::string(*PHYSFS_getSearchPath()).append("/") + relativePath;

	return DuplicateFile(file, finalPath.c_str());
}

bool ModuleFileSystem::DuplicateFile(const char* srcFile, const char* dstFile)
{
	//TODO: Redo this function (maybe do it just using physfs?)
	std::ifstream src;
	src.open(srcFile, std::ios::binary);
	bool srcOpen = src.is_open();
	std::ofstream  dst(dstFile, std::ios::binary);
	bool dstOpen = dst.is_open();

	dst << src.rdbuf();

	src.close();
	dst.close();

	if (srcOpen && dstOpen)
	{
		LOG("[success] File Duplicated Correctly");
		return true;
	}
	else
	{
		LOG("[error] File could not be duplicated");
		return false;
	}
}

void ModuleFileSystem::AddScriptWatch(const char* path)
{
	watchFiles.push_back(path);
}

void ModuleFileSystem::DiscoverFiles(const char* directory, std::vector<std::string>& file_list, std::vector<std::string>& dir_list) const
{
	char **rc = PHYSFS_enumerateFiles(directory);
	char **i;

	for (i = rc; *i != nullptr; i++)
	{
		std::string str = std::string(directory) + std::string("/") + std::string(*i);
		if (IsDirectory(str.c_str()))
			dir_list.push_back(*i);
		else
			file_list.push_back(*i);
	}

	PHYSFS_freeList(rc);
}
