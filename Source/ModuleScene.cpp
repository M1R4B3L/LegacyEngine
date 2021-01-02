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


ModuleScene::ModuleScene(bool startEnable) : Module(startEnable){}

ModuleScene::~ModuleScene(){}

// Load assets
bool ModuleScene::Start()
{
	Importer::Textures::Init();
	
	bool ret = true;
	root = new GameObject();

	return ret;
}

bool ModuleScene::CleanUp()
{

	App->resources->UnrequestResource(resourceID);
	root = nullptr;
	resource = nullptr;

	return true;
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
	resource->SaveResource();
}

bool ModuleScene::LoadScene(unsigned int ID)
{
	resource = (ResourceScene*)App->resources->RequestResource(ID, Resource::Type::SCENE);
	if (resource == nullptr) 
	{
		LOG("Could not load the scene");
		return false;
	}
	//TODO: we just have 1 scene at a time (just one root on scene)

	App->resources->UnrequestResource(resourceID);
	resourceID = resource->GetUID();
	root = resource->root;
	sceneName = resource->sceneName;
	return true;
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

bool ModuleScene::ChangeResource(unsigned int id)
{
	ResourceScene* newResource = (ResourceScene*)App->resources->RequestResource(id, Resource::Type::SCENE);
	if (newResource == nullptr)
		return false;

	App->resources->UnrequestResource(resourceID);
	resource = newResource;
	resourceID = id;
	selectedObject = nullptr;

	return true;
}
