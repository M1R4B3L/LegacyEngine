#include "Globals.h"
#include "GL/glew.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "ModuleScene.h"
#include "Primitive.h"
#include "imgui.h"
#include "examples\imgui_impl_sdl.h"
#include "examples\imgui_impl_opengl3.h"
#include "Importer.h"
#include "GameObjects.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"
#include "parson.h"
#include "ModuleFileSystem.h"


ModuleScene::ModuleScene(bool startEnable) : Module(startEnable)
{
}

ModuleScene::~ModuleScene()
{}

// Load assets
bool ModuleScene::Start()
{
	Importer::Textures::Init();
	//LOG("Loading Intro assets");
	bool ret = true;
	root = new GameObject(nullptr, "Scene");

	Importer::ImportDroped("Assets/Baker_house/BakerHouse.fbx");

	Importer::ImportDroped("Assets/street/Street environment_V01.FBX");

	return ret;
}

// Load assets
bool ModuleScene::CleanUp()
{
	//LOG("Unloading Intro scene");
	
	return true;
}

void ModuleScene::UpdateAllGameObjects(float dt)
{
	root->Update(dt);
}

void ModuleScene::DrawAllGameObjects()
{
}

// Update
update_status ModuleScene::Update(float dt)
{
	/*PPlane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();*/

	UpdateAllGameObjects(dt);

	return UPDATE_CONTINUE;
}

GameObject* ModuleScene::CreateGameObject(const char* name , GameObject* parent)
{
	//Todo: this ?!?!
	GameObject* go;
	if (parent)
		go = new GameObject(parent, name);
	else 
		go = new GameObject(root, name);

	return go;
}

void ModuleScene::SetParent(GameObject* gameObject, GameObject* newParent)
{
	GameObject* parent = gameObject->GetParent();
	
	for (std::vector<GameObject*>::iterator it = parent->children.begin(); it != parent->children.end(); ++it)
	{
		if ((*it) == gameObject) {
			parent->children.erase(it);
			break;
		}
	}

	newParent->children.push_back(gameObject);
	gameObject->SetParent(newParent);
	((ComponentTransform*)gameObject->GetComponent(ComponentType::Transform))->SetFlag();
	((ComponentTransform*)gameObject->GetComponent(ComponentType::Transform))->NewParentLocal(newParent);

}

GameObject* ModuleScene::GetRootObject() const
{
	return root;
}

GameObject* ModuleScene::GetSelectedObject() const
{
	return selectedObject;
}

void ModuleScene::SetGameObjectSelected(GameObject* gameObject)
{
	selectedObject = gameObject;
}

void ModuleScene::SetGameObjectUnselected()
{
	selectedObject = nullptr;
}

void ModuleScene::DeleteGameObject(GameObject* gameObject, bool root)
{
	if (selectedObject == gameObject)
	{
		selectedObject = nullptr;
	}

	std::vector<GameObject*>::iterator it = gameObject->children.begin();

	for (it; it != gameObject->children.end(); ++it)
	{
		DeleteGameObject(*it, false);
	}

	gameObject->children.clear();

	if (root) {
		for (std::vector<GameObject*>::iterator it = gameObject->GetParent()->children.begin(); it != gameObject->GetParent()->children.end(); ++it)
		{
			if ((*it) == gameObject) {
				gameObject->GetParent()->children.erase(it);
				break;
			}
		}
	}

	/*for (std::vector<Component*>::iterator it = gameObject->components.begin(); it != gameObject->components.end(); ++it)
	{
		delete (*it);
	}*/
	delete gameObject;
	gameObject = nullptr;
}

void ModuleScene::SaveScene()
{
	JSON_Value* rootValue = json_value_init_object();
	JSON_Object* node = json_value_get_object(rootValue);
	json_object_set_value(node, "GameObjects", json_value_init_array());
	JSON_Array* arry = json_object_get_array(node, "GameObjects");

	root->Save(arry);
	size_t size = json_serialization_size_pretty(rootValue);
	char* buffer = new char[size];
	json_serialize_to_buffer_pretty(rootValue, buffer, size);
	//TODO:Scene name!!!
	App->fileSystem->Save("Assets/scene.json",buffer,size);
	json_value_free(rootValue);
	delete[] buffer;
}

bool ModuleScene::LoadScene(const char* fileName)
{
	char* buffer = nullptr;
	App->fileSystem->Load("Assetes/", fileName, &buffer);
	JSON_Value* rootValue = json_parse_string(buffer);
	if (!rootValue)
	{
		LOG("Could not parse json buffer from file: %s", fileName);
		return false;
	}
	JSON_Object* node = json_value_get_object(rootValue);
	if (!json_object_has_value_of_type(node,"GameObjects", JSONArray)) 
	{
		LOG("json Array GameObjects not found");
		json_value_free(rootValue);
		return false;
	}
	JSON_Array* goArray = json_object_get_array(node, "GameObjects");
	JSON_Object* jsonRootGO = json_array_get_object(goArray, 0);
	unsigned int rootID = json_object_get_number(jsonRootGO, "ParentUID");
	if (rootID != 0)
	{
		LOG("Error loading the json scene: The first Game Object of the array is not the root");
		json_value_free(rootValue);
		return false;
	}
	//El root no li llegeixo el transform (ns ni si en te)
	root = new GameObject(nullptr, json_object_get_number(jsonRootGO, "UID"), json_object_get_string(jsonRootGO, "Name"));
	GameObject* currGO;
	for (int i = 1; i < json_array_get_count(goArray); ++i) 
	{
		JSON_Object* jsonGO = json_array_get_object(goArray, i);
		currGO = new GameObject(FindGOFromUID(root, json_object_get_number(jsonGO, "ParentUID")), json_object_get_number(jsonGO, "UID"), json_object_get_string(jsonGO, "Name"));
		JSON_Array* compArray = json_object_get_array(jsonGO, "Components");
		for (int j = 0; j < json_array_get_count(compArray); ++j) 
		{
			JSON_Object* compObj = json_array_get_object(compArray, j);
			ComponentType type= (ComponentType)json_object_get_number(compObj, "Type");
			switch (type) 
			{
			case ComponentType::Transform: 
			{
				ComponentTransform* newComponentTransform = new ComponentTransform(currGO);
				newComponentTransform->Load(compObj);
				currGO->AddComponent((Component*)newComponentTransform);
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
}

unsigned int ModuleScene::GetRandomInt()
{
	return random.Int();
}

GameObject* ModuleScene::FindGOFromUID(GameObject* currGO, unsigned int UID)
{
	for (std::vector<GameObject*>::const_iterator cit = currGO->children.cbegin(); cit != currGO->children.cend(); ++cit) 
	{
		if ((*cit)->GetID() == UID)
			return (*cit);
		FindGOFromUID((*cit),UID);
	}
	return nullptr;
}

void ModuleScene::CreateCamera(const char* name)
{

	GameObject* camera = new GameObject(root, name);
	ComponentTransform* componentTransform = new ComponentTransform(camera);
	ComponentCamera* componentCamera = new ComponentCamera(camera);

}
