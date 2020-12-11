/*#pragma once
#include "Module.h"
#include <map>
class Resource;
class ModuleResources : public Module
{
public:
	UID Find(const char* file_in_assets) const;
	UID ImportFile(const char* new_file_in_assets);
	UID GenerateNewUID();
	const Resource* RequestResource(UID uid) const;
	Resource* RequestResource(UID uid);
	void ReleaseResource(UID uid);
private:
	Resource* CreateNewResource(const char* assetsPath, Resource::Type type);
private:
	std::map<unsigned int, Resource*> resources;
};
*/