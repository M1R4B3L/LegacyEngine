#include "ModuleResources.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "Importer.h"
#include "ResourceTexture.h"
#include "ResourceMesh.h"
#include "ModuleScene.h"
#include "ResourceModel.h"
#include "ResourceScene.h"
#include "ResourceScript.h"
#include "Compiler.h"

bool ModuleResources::Start()
{
	std::vector<std::string> files;
	std::vector<std::string> directories;
	App->fileSystem->DiscoverFiles(MESHES_PATH, files, directories);
	App->fileSystem->DiscoverFiles(SCENES_PATH, files, directories);
	App->fileSystem->DiscoverFiles(TEXTURES_PATH, files, directories);
	App->fileSystem->DiscoverFiles(MODELS_PATH, files, directories);
	App->fileSystem->DiscoverFiles(SCRIPTS_PATH, files, directories);
	
	std::vector<std::string>::const_iterator cit = files.cbegin();
	for (cit; cit != files.cend(); ++cit) {
		unsigned int uid = std::stoi((*cit));
		resources[uid] = nullptr;
	}
	return true;
}

update_status ModuleResources::Update(float dt)
{
	return update_status::UPDATE_CONTINUE;
}

bool ModuleResources::CleanUp()
{
	return true;
}

ModuleResources::ModuleResources(bool startEnabled) : Module(startEnabled)
{
}

Resource* ModuleResources::CreateNewResource(const char* assetsPath, Resource::Type type, std::string* libPath, std::string* metaPath)
{
	Resource* ret = nullptr;
	std::string fileName;
	unsigned int uid = GenerateNewUID();
	switch (type) {
	case Resource::Type::TEXTURE:
		ret = (Resource*) new ResourceTexture(uid);
		if (ret == nullptr)
			break;
		App->fileSystem->SplitFilePath(assetsPath, metaPath, &fileName);
		*metaPath = ASSETS_TEXTURES + fileName + ".meta";
		break;
		//case Resource::Type::MESH: ret = (Resource*) new ResourceMesh(uid); break;
		case Resource::Type::SCENE: 
			ret = (Resource*) new ResourceScene(uid);
			if (ret == nullptr)
				break;
			App->fileSystem->SplitFilePath(assetsPath, metaPath, &fileName);
			*metaPath = ASSETS_SCENES + fileName + ".scene";
			break;
	case Resource::Type::MODEL:
		ret = (Resource*) new ResourceModel(uid);
		if (ret == nullptr)
			break;
		App->fileSystem->SplitFilePath(assetsPath, metaPath, &fileName);
		*metaPath = ASSETS_MODELS + fileName + ".meta";
		break;
	}
	if (ret != nullptr)
	{
		resources[uid] = ret;
		*libPath = GetLibFilePath(ret);//->fer el save en el library file o nomes crear el file amb el nom del uid?
	}
	return ret;
}

unsigned int ModuleResources::ImportFile(const char* assetsFile, Resource::Type type)//pot ser que el asset file sigui el meta file?
{
	std::string libPath; std::string metaPath;
	Resource* resource = CreateNewResource(assetsFile, type, &libPath, &metaPath);
	unsigned int ret = 0;
	char* metaBuffer = nullptr;
	char* libBuffer = nullptr;
	unsigned int libSize = 0;
	unsigned int metaSize = 0;

	switch (resource->GetType()) {
	case Resource::Type::TEXTURE: Importer::Textures::Import(assetsFile, &libBuffer, libSize, &metaBuffer, metaSize, (ResourceTexture*)resource); break;
	case Resource::Type::SCENE: Importer::Scenes::Import(assetsFile, &libBuffer, libSize, &metaBuffer, metaSize, (ResourceScene*)resource); break;
	//case Resource::Type::MESH: Importer::Meshes::Import(fileBuffer, resource); break;
	case Resource::Type::MODEL: Importer::Models::ImportFbx(assetsFile, &libBuffer, libSize, &metaBuffer, metaSize, (ResourceModel*)resource); break;
	}

	SaveResource(resource, libPath.c_str(), libBuffer, libSize, metaPath.c_str(), metaBuffer, metaSize);
	ret = resource->GetUID();
	if (metaBuffer != nullptr) {
		delete[] metaBuffer;
		metaBuffer = nullptr;
	}
	if (libBuffer != nullptr) {
		delete[] libBuffer;
		libBuffer = nullptr;
	}
	UnloadResource(resource->GetUID());
	return ret;
}

//Save the library and the meta files of a resource
void ModuleResources::SaveResource(Resource* resource, const char* libPath ,char* libBuffer, unsigned int libSize, const char* metaPath, char* metaBuffer, unsigned int metaSize) 
{
	if(libSize != 0)
		App->fileSystem->Save(libPath, libBuffer, libSize);
	if(metaSize != 0)
		App->fileSystem->Save(metaPath, metaBuffer, metaSize);
}

std::string ModuleResources::GetLibFilePath(Resource* resource)
{
	std::string path;
	switch (resource->GetType()) 
	{
	case Resource::Type::TEXTURE: path = TEXTURES_PATH; break;
	case Resource::Type::MESH: path = MESHES_PATH; break;
	case Resource::Type::MODEL: path = MODELS_PATH; break;
	case Resource::Type::SCENE: path = SCENES_PATH; break;
	}
	path += std::to_string(resource->GetUID());
	return path;
}

unsigned int ModuleResources::GenerateNewUID()
{
	//TODO: MD5 ID generation
	return App->scene->GetRandomInt();
}

Resource* ModuleResources::RequestResource(unsigned int uid, Resource::Type type)
{
	std::map<unsigned int, Resource*>::iterator it = resources.find(uid);
	if (it != resources.end())
	{
		
		if (it->second == nullptr) 
		{
			switch (type) 
			{
			case Resource::Type::MESH: it->second = new ResourceMesh(it->first); break;
			case Resource::Type::MODEL: it->second = new ResourceModel(it->first); break;
			case Resource::Type::TEXTURE: it->second = new ResourceTexture(it->first); break;
			case Resource::Type::SCENE: it->second = new ResourceScene(it->first); break;
			case Resource::Type::SCRIPT: it->second = new ResourceScript(it->first); break;
			}

			it->second->LoadInMemory();
		}

		it->second->IncreaseReferenceCount();
		return it->second;
	}
	return nullptr;
}

int ModuleResources::GetResourceCount(unsigned int uid)
{
	std::map<unsigned int, Resource*>::iterator it = resources.find(uid);
	if (it != resources.end())
	{
		if (it->second == nullptr)
		{
			return 0;
		}
		return it->second->GetReferenceCount();
	}
	return -1;
}

void ModuleResources::UnrequestResource(unsigned int uid)
{
	std::map<unsigned int, Resource*>::iterator it = resources.find(uid);
	if (it != resources.end())
	{
		if (it->second != nullptr)
		{
			it->second->DecreaseReferenceCount();
			if (it->second->GetReferenceCount() == 0)
				UnloadResource(uid);
		}
	}
}

bool ModuleResources::ResourceExists(unsigned int uid)
{
	std::map<unsigned int, Resource*>::iterator it = resources.find(uid);
	if (it != resources.end())
		return true;

	return false;
}

void ModuleResources::UnloadResource(unsigned int uid)
{
	std::map<unsigned int, Resource*>::iterator it = resources.find(uid);
	if (it != resources.end()) 
	{
		delete it->second;
		it->second = nullptr;
	}
}

void ModuleResources::RemoveResource(unsigned int uid)
{
	std::map<unsigned int, Resource*>::iterator it = resources.find(uid);
	if (it != resources.end()) 
	{
		delete it->second;
		resources.erase(it);
	}

	//TODO: Delete the asset, library and meta files releted to the resource
}

void ModuleResources::AddMeshResource(unsigned int uid, Resource* resource)
{
	resources[uid] = resource;
	UnloadResource(resource->GetUID());
}

void ModuleResources::AddSceneResource(Resource* resource)
{
	resources[resource->GetUID()] = resource;
}

void ModuleResources::CreateScriptResource(const char* scriptName)
{
	int uid = GenerateNewUID();
	ResourceScript* resource = new ResourceScript(uid);
	resources[uid] = resource;
	UnrequestResource(uid);

	std::string metaPath = ASSETS_SCRIPTS;
	metaPath += scriptName;
	metaPath += ".meta";
	char* metaBuffer;
	JSON_Value* rootValue = json_value_init_object();
	JSON_Object* node = json_value_get_object(rootValue);
	json_object_set_number(node, "LIBUID", resource->GetUID());
	json_object_set_string(node, "SourceFile", scriptName);
	int metaSize = json_serialization_size_pretty(rootValue);
	metaBuffer = new char[metaSize];
	json_serialize_to_buffer_pretty(rootValue, metaBuffer, metaSize);
	json_value_free(rootValue);
	App->fileSystem->Save(metaPath.c_str(), metaBuffer, metaSize);
	delete[] metaBuffer;

	std::string templatePath = SCRIPT_HELPER_PATH;
	templatePath += "ScriptTemplate.cpp";
	std::string assetsPath = ASSETS_SCRIPTS;
	assetsPath += scriptName;
	assetsPath += ".cpp";
	App->fileSystem->DuplicateFile(templatePath.c_str(), assetsPath.c_str());
	std::vector<std::string>toCompile;
	toCompile.push_back(assetsPath);
	std::string engineLib = "Engine.lib"; std::vector<std::string>linkLibs;
	linkLibs.push_back(engineLib);
	//linkLibs.push_back(std::string("MathGeoLib.lib"));
	/*std::string tempPath = TEMP_PATH;
	tempPath += std::to_string(uid) + ".dll";*/
	std::string libPath = SCRIPTS_PATH;
	libPath += std::to_string(uid) + ".dll";
	CompilerOptions options;
	options.includeDirList.push_back(SCRIPT_HELPER_PATH);
	options.libraryDirList.push_back(SCRIPT_HELPER_PATH);
	options.intermediatePath = TEMP_PATH;
	App->compiler->RunCompile(toCompile, options, linkLibs, libPath);

	/*std::string libPath = SCRIPTS_PATH;
	libPath += std::to_string(uid) + ".dll";
	App->fileSystem->DuplicateFile(tempPath.c_str(), libPath.c_str());*/
}
