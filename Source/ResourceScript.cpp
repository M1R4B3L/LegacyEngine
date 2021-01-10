#include "Application.h"
#include "ResourceScript.h"
#include "Globals.h"
#include "GameObjects.h"
#include "Compiler.h"
#include "ModuleFileSystem.h"

ResourceScript::ResourceScript(unsigned int id): Resource(id, Resource::Type::SCRIPT) {}

bool ResourceScript::LoadInMemory()
{
	std::string path = SCRIPTS_PATH + std::to_string(uid) + ".dll";
	dllHandle = LoadLibrary(path.c_str());
	if (dllHandle != NULL)
	{
		functions.Start = (void(*)(GameObject*))GetProcAddress(dllHandle, "Start");
		functions.Update = (void(*)(GameObject*))GetProcAddress(dllHandle, "Update");
		functions.GetScriptName = (const char*(*)())GetProcAddress(dllHandle, "GetName");
		dllAvailable = true;
	}
	else 
	{
		dllAvailable = false;
		functions.Start = nullptr;
		functions.Update = nullptr;
		functions.GetScriptName = nullptr;
		LOG("Could not open script dll file");
	}

	return dllAvailable;
}

void ResourceScript::HotReload(const char* sourceFile)
{
	//UNLOAD THE CURRENT DLL
	if (dllHandle != NULL)
		FreeLibrary(dllHandle);

	std::vector<std::string>toCompile;
	toCompile.push_back(sourceFile);
	std::string engineLib = "Engine.lib"; std::vector<std::string>linkLibs;
	linkLibs.push_back(engineLib);
	//linkLibs.push_back(std::string("MathGeoLib.lib"));
	std::string libPath = SCRIPTS_PATH;
	libPath += std::to_string(uid) + ".dll";
	/*std::string libPath = SCRIPTS_PATH;
	libPath += std::to_string(uid) + ".dll";*/
	App->fileSystem->Remove(libPath.c_str());
	std::string pdbFile = SCRIPTS_PATH + std::to_string(uid) + ".pdb";
	App->fileSystem->Remove(pdbFile.c_str());
	CompilerOptions options;
	options.includeDirList.push_back(SCRIPT_HELPER_PATH);
	options.libraryDirList.push_back(SCRIPT_HELPER_PATH);
	options.intermediatePath = TEMP_PATH;
	App->compiler->RunCompile(toCompile, options, linkLibs, libPath);

	LoadInMemory();
}

ResourceScript::~ResourceScript()
{
	if(dllHandle != NULL)
		FreeLibrary(dllHandle);
}
