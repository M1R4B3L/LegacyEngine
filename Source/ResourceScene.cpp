#include "Globals.h"
#include "ResourceScene.h"
#include "parson.h"
#include "GameObjects.h"
#include "Application.h"
#include "parson.h"
#include "ModuleFileSystem.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"

ResourceScene::ResourceScene(unsigned int id) : Resource(id, Resource::Type::SCENE)
{

}

ResourceScene::~ResourceScene()
{

}

bool ResourceScene::LoadInMemory()
{
	//TODO: No estic considerant cap import option amb el meta file

	char* buffer = nullptr;
	App->fileSystem->Load(SCENES_PATH, std::to_string(uid).c_str(), &buffer);
	JSON_Value* rootValue = json_parse_string(buffer);
	if (!rootValue)
	{
		LOG("Could not parse json buffer from file: %s", std::to_string(uid).c_str());
		delete[] buffer;
		return false;
	}
	JSON_Object* node = json_value_get_object(rootValue);
	if (!json_object_has_value_of_type(node, "GameObjects", JSONArray))
	{
		LOG("json Array GameObjects not found");
		json_value_free(rootValue);
		delete[] buffer;
		return false;
	}

	JSON_Array* goArray = json_object_get_array(node, "GameObjects");
	JSON_Object* jsonRootGO = json_array_get_object(goArray, 0);
	unsigned int rootID = json_object_get_number(jsonRootGO, "ParentUID");
	if (rootID != 0)
	{
		LOG("Error loading the json scene: The first Game Object of the array is not the root");
		json_value_free(rootValue);
		delete[] buffer;
		return false;
	}

	sceneName = json_object_get_string(jsonRootGO, "SceneName");
	root = new GameObject(nullptr, sceneName.c_str());

	GameObject* rootImport = new GameObject(root, json_object_get_number(jsonRootGO, "UID"), json_object_get_string(jsonRootGO, "Name"));
	JSON_Array* rootComponents = json_object_get_array(jsonRootGO, "Components");
	JSON_Object* rootTransform = json_array_get_object(rootComponents, 0);
	if (json_object_get_number(rootTransform, "Type") != (int)ComponentType::Transform)
	{
		LOG("The root object does'nt have a transform component");
		json_value_free(rootValue);
		delete[] buffer;
		return false;
	}
	ComponentTransform* newComponentTransform = new ComponentTransform(rootImport);
	newComponentTransform->Load(rootTransform);

	GameObject* currGO;

	for (int i = 1; i < json_array_get_count(goArray); ++i)
	{
		JSON_Object* jsonGO = json_array_get_object(goArray, i);
		currGO = new GameObject(rootImport, json_object_get_number(jsonGO, "UID"), json_object_get_string(jsonGO, "Name"));
		JSON_Array* compArray = json_object_get_array(jsonGO, "Components");
		for (int j = 0; j < json_array_get_count(compArray); ++j)
		{
			JSON_Object* compObj = json_array_get_object(compArray, j);
			ComponentType type = (ComponentType)json_object_get_number(compObj, "Type");
			switch (type)
			{
			case ComponentType::Transform:
			{
				ComponentTransform* newComponentTransform = new ComponentTransform(currGO);
				newComponentTransform->Load(compObj);
				break;
			}
			case ComponentType::Mesh:
			{
				ComponentMesh* newComponentMesh = new ComponentMesh();
				newComponentMesh->Load(compObj);
				currGO->AddComponent((Component*)newComponentMesh);
				break;
			}
			case ComponentType::Material:
			{
				ComponentMaterial* newComponentMaterial = new ComponentMaterial();
				newComponentMaterial->Load(compObj);
				currGO->AddComponent((Component*)newComponentMaterial);
				break;
			}
			}
		}
	}
	json_value_free(rootValue);
	delete[] buffer;
	return true;
}

bool ResourceScene::SaveResource()
{
	JSON_Value* rootValue = json_value_init_object();
	JSON_Object* node = json_value_get_object(rootValue);
	json_object_set_string(node, "SceneName", sceneName.c_str());
	json_object_set_value(node, "GameObjects", json_value_init_array());
	JSON_Array* arry = json_object_get_array(node, "GameObjects");

	root->Save(arry);
	size_t size = json_serialization_size_pretty(rootValue);
	char* buffer = new char[size];
	json_serialize_to_buffer_pretty(rootValue, buffer, size);
	//TODO: Save the scene last modification time on the metafile
	std::string libPath = std::to_string(uid).c_str();
	libPath.append(SCENES_PATH);
	App->fileSystem->Save(libPath.c_str(), buffer, size);
	json_value_free(rootValue);
	delete[] buffer;

	return true;
}
