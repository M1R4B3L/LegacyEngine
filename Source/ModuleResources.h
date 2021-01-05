#pragma once
#include "Globals.h"
#include "Module.h"
#include <map>
#include "Resource.h"

class ModuleResources : public Module
{
public:
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	ModuleResources(bool startEnabled = true);
	//unsigned int Find(const char* file_in_assets) const;
	unsigned int ImportFile(const char* new_file_in_assets, Resource::Type type = Resource::Type::unknown);
	std::string GetLibFilePath(Resource* resoure);
	unsigned int GenerateNewUID();
	void SaveResource(Resource* resource, const char* libPath, char* libBuffer, unsigned int libSize, const char* metaPath, char* metaBuffer, unsigned int metaSize);
	bool ResourceExists(unsigned int uid);
	Resource* RequestResource(unsigned int uid, Resource::Type type);
	void UnrequestResource(unsigned int uid);
	int GetResourceCount(unsigned int uid);
	void RemoveResource(unsigned int uid);
	void AddMeshResource(unsigned int uid,Resource* resource);
	void AddSceneResource(Resource* resource);
private:
	Resource* CreateNewResource(const char* assetsPath, Resource::Type type,std::string* libPath = nullptr, std::string* metaPath = nullptr);
	void UnloadResource(unsigned int uid);

private:
	std::map<unsigned int, Resource*> resources;
};