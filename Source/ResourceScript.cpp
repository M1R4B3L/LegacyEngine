#include "ResourceScript.h"
#include "Globals.h"
#include "GameObjects.h"

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
		LOG("Could not open script dll file");
	}

	return dllAvailable;
}

ResourceScript::~ResourceScript()
{
	if(dllHandle != NULL)
		FreeLibrary(dllHandle);
}
