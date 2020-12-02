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

	App->camera->Move(vec3(5.0f, 5.0f, 5.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	Importer::ImportDroped("Assets/Baker_house/BakerHouse.fbx");

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

	plane p(0, 1, 0, 0);
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