#include "Globals.h"
#include "GL/glew.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "imgui.h"
#include "examples\imgui_impl_sdl.h"
#include "examples\imgui_impl_opengl3.h"
#include "Importer.h"
#include "GameObjects.h"
#include "Component.h"
#include "ComponentMesh.h"



ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	Importer::Textures::Init();
	//LOG("Loading Intro assets");
	bool ret = true;
	root = new GameObject(nullptr, "root");

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	Importer::ImportDroped("Assets/Fish/fish.fbx");

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	//LOG("Unloading Intro scene");
	
	return true;
}

void ModuleSceneIntro::UpdateAllGameObjects(float dt)
{
	root->Update(dt);
}

void ModuleSceneIntro::DrawAllGameObjects()
{
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{

	plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	UpdateAllGameObjects(dt);

	return UPDATE_CONTINUE;
}

GameObject* ModuleSceneIntro::CreateGameObject(const char* name , GameObject* parent)
{
	//Todo: this ?!?!
	GameObject* go;
	if (parent)
		go = new GameObject(parent, name);
	else 
		go = new GameObject(root, name);

	return go;
}

const GameObject* ModuleSceneIntro::GetRootObject() const
{
	return root;
}
