/*#include "ModuleResources.h"

UID ModuleResources::ImportFile(const char* new_file_in_assets)
{
	Resource* resource = CreateNewResource(assetsFile, type); //Save ID, assetsFile path, libraryFile path
	UID ret = 0;
	char* fileBuffer = Engine->fileSystem->Load(assetsFile); //<-- pseudocode, load from File System
	switch (resource->Type) {
	case Resource::texture: App->tex->Import(fileBuffer, resource); break;
	case Resource::scene: App->scene->Import(fileBuffer, resource); break;
	case Resource::mesh: App->meshes > Import(fileBuffer, resource); break;
	}
	SaveResource(resource);
	ret = resource->GetID();
	RELEASE_ARRAY(buffer);
	UnloadResource(resource); //<-- unload the resource after importing, we should only use the ID
	return ret;
}

Resource* ModuleResources::RequestResource(UID uid)
{
	std::map<UID, Resource*>::iterator it = resources.find(uid);
	if (it != resources.end())
	{
		it->second->referenceCount++;
		return it->second;
	}
	return nullptr;
}

Resource* ModuleResources::CreateNewResource(const char* assetsPath, Resource::Type type)
{
	Resource* ret = nullptr;
	UID uid = GenerateNewUID();
	switch (type) {
	case Resource::texture: ret = (Resource*) new ResourceTexture(uid); break;
	case Resource::mesh: ret = (Resource*) new ResourceMesh(uid); break;
	case Resource::scene: ret = (Resource*) new ResourceScene(uid); break;
	case Resource::bone: ret = (Resource*) new ResourceBone(uid); break;
	case Resource::animation: ret = (Resource*) new ResourceAnimation(uid); break;
	}
	if (ret != nullptr)
	{
		resources[uid] = ret;
		resource->assetsFile = assetsPath;
		resource->libraryFile = GenLibraryFile(resource);
	}
	return ret;
}
*/