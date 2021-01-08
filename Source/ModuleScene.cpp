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
#include "ModuleResources.h"
#include "ResourceScene.h"

//NO CAL
#include "ModuleInput.h"
#include "Compiler.h"


ModuleScene::ModuleScene(bool startEnable) : Module(startEnable){}

ModuleScene::~ModuleScene(){}

// Load assets
bool ModuleScene::Start()
{
	Importer::Textures::Init();
	bool ret = true;

	//TODO: Scene inizialitzation
	std::string path = ASSETS_SCENES;
	path += "DefaultScene.scene";
	if (!App->fileSystem->FileExists(path.c_str()))
		GenerateScene("DefaultScene");
	else {
		char* buffer = nullptr;
		App->fileSystem->Load(path.c_str(), &buffer);
		JSON_Value* rootValue = json_parse_string(buffer);
		JSON_Object* node = json_value_get_object(rootValue);
		unsigned int uid = json_object_get_number(node, "LIBUID");
		json_value_free(rootValue);
		delete[] buffer;

		if (!App->scene->LoadScene(uid))
		{
			LOG("Could not load the default scene \nGenerating default scene");
			GenerateScene("DefaultScene");
		}
	}
	return ret;
}

bool ModuleScene::CleanUp()
{

	App->resources->UnrequestResource(resourceID);
	root = nullptr;
	resource = nullptr;

	return true;
}

void ModuleScene::GenerateScene(const char* name)
{
	if (resource != nullptr) 
		App->resources->UnrequestResource(resourceID);
	
	root = new GameObject(nullptr, name);
	sceneName = name;
	resourceID = GetRandomInt();
	resource = new ResourceScene(resourceID, root);
	App->resources->AddSceneResource(resource);

	JSON_Value* rootValue = json_value_init_object();
	JSON_Object* node = json_value_get_object(rootValue);
	json_object_set_string(node, "SceneName", sceneName.c_str());
	json_object_set_value(node, "GameObjects", json_value_init_array());
	JSON_Array* arry = json_object_get_array(node, "GameObjects");

	root->Save(arry);
	size_t size = json_serialization_size_pretty(rootValue);
	char* buffer = new char[size];
	json_serialize_to_buffer_pretty(rootValue, buffer, size);

	std::string libPath = SCENES_PATH;
	libPath += std::to_string(resourceID).c_str();

	App->fileSystem->Save(libPath.c_str(), buffer, size);
	json_value_free(rootValue);
	delete[] buffer;
	buffer = nullptr;

	//Generating metafile
	rootValue = json_value_init_object();
	node = json_value_get_object(rootValue);
	json_object_set_number(node, "LIBUID", resource->GetUID());
	size = json_serialization_size_pretty(rootValue);
	buffer = new char[size];
	json_serialize_to_buffer_pretty(rootValue, buffer, size);
	json_value_free(rootValue);
	std::string path = ASSETS_SCENES;
	path += name;
	path += ".scene";
	App->fileSystem->Save(path.c_str(), buffer, size);

	delete[] buffer;
	buffer = nullptr;
}

void ModuleScene::StartGame()
{
	//TODO: kuan s'acabi el game time returnar a la primera scena
	root->Start();
}

void ModuleScene::UpdateAllGameObjects(float dt)
{
	root->Update(dt);
}

void ModuleScene::DrawAllGameObjects()
{
	//TODO: The draw of all gameobjects at once
}

// Update
update_status ModuleScene::Update(float dt)
{
	PPlane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	UpdateAllGameObjects(dt);

	if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_UP) 
	{
		std::vector<std::string>compileFiles;
		std::vector<std::string>linkLibraries;
		CompilerOptions options;
		//std::string Hello = "hello.h";
		std::string hellocpp = "hello.cpp";
		//compileFiles.push_back(Hello);
		compileFiles.push_back(hellocpp);
		App->compiler->RunCompile(compileFiles, options, linkLibraries, "hi");
	}

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

GameObject* ModuleScene::CreateTransformGameObject(const char* name, GameObject* parent, ComponentTransform* transform)
{
	GameObject* go;
	if (parent)
		go = new GameObject(parent, name);
	else
		go = new GameObject(root, name);

	if (transform)
		go->AddComponent((Component*)transform);
	else
		Component* transformComp = new ComponentTransform(go);
	
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

	ComponentTransform* goTransform = (ComponentTransform*)gameObject->GetComponent(ComponentType::Transform);
	newParent->children.push_back(gameObject);
	gameObject->SetParent(newParent);
	goTransform->SetFlag();
	goTransform->NewParentLocal(newParent);

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
		GameObject* parent = gameObject->GetParent();
		if (parent) {
			for (std::vector<GameObject*>::iterator it = parent->children.begin(); it != parent->children.end(); ++it)
			{
				if ((*it) == gameObject) {
					parent->children.erase(it);
					break;
				}
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

void ModuleScene::SaveSceneAt(const char* path)
{
	selectedObject = nullptr;
	resource->SaveResourceAt(path);
}

void ModuleScene::LoadSceneFrom(const char* path)
{
	selectedObject = nullptr;
	resource->LoadResourceFrom(path);
}

void ModuleScene::SaveScene()
{
	resource->SaveResource();
}

bool ModuleScene::LoadScene(unsigned int ID)
{
	if (resourceID == ID)
		App->resources->UnrequestResource(resourceID);
	
	ResourceScene* newResource = (ResourceScene*)App->resources->RequestResource(ID, Resource::Type::SCENE);
	if (newResource == nullptr) 
	{
		LOG("Could not load the scene");
		return false;
	}
	//TODO: we just have 1 scene at a time (just one root on scene)

	if(resourceID != newResource->GetUID())
		App->resources->UnrequestResource(resourceID);
	resource = newResource;
	resourceID = resource->GetUID();
	root = resource->root;
	return true;
}

unsigned int ModuleScene::GetRandomInt()
{
	return random.Int();
}

GameObject* ModuleScene::FindGOFromUID(GameObject* currGO, unsigned int UID)
{
	if (currGO->GetID() == UID)
		return currGO;

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

unsigned int ModuleScene::GetResourceId() const
{
	return resourceID;
}

const ResourceScene* ModuleScene::GetResource() const
{
	return resource;
}

const char* ModuleScene::GetSceneName() const
{
	return sceneName.c_str();
}

void ModuleScene::SetSceneName(const char* newName)
{
	sceneName = newName;
}
