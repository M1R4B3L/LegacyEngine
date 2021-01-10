#pragma once
#include "Resource.h"
#include <Windows.h>

class GameObject;
struct ScriptFunctions {
	void(*Start)(GameObject*) = nullptr;
	void(*Update)(GameObject*) = nullptr;
	const char*(*GetScriptName)() = nullptr;
};

class ResourceScript : public Resource {
public:
	ResourceScript(unsigned int id);
	~ResourceScript();

	bool LoadInMemory() override;
	void HotReload(const char* sourceFile);

	ScriptFunctions functions;
	bool dllAvailable = false;
	//NeedReload()
private:
	//LastTimeModification
	//bool SuccesfulReload;
	HMODULE dllHandle = NULL;
};