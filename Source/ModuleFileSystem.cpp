#include "Globals.h"
#include "ModuleFileSystem.h"


ModuleFileSystem::ModuleFileSystem(bool startEnable) : Module(startEnable)
{
}

ModuleFileSystem::~ModuleFileSystem()
{
}

bool ModuleFileSystem::Init()
{
	return true;
}

update_status ModuleFileSystem::Update(float dt)
{
	return UPDATE_CONTINUE;
}
