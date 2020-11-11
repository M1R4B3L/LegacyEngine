#include "Globals.h"
#include "GL/glew.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "imgui.h"
#include "examples\imgui_impl_sdl.h"
#include "examples\imgui_impl_opengl3.h"
#include "Importer.h"
#include "GameObjects.h"
#include "Components.h"
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

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	ImportGameObject("Assets/Baker_house/BakerHouse.fbx");

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	//LOG("Unloading Intro scene");
	
	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{

	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	std::vector<GameObject*>::iterator it;
	for (it = game_objects.begin(); it != game_objects.end(); it++) {
		(*it)->Update(dt);
	}

	//PROBA RENDER 1X1 CUBE OPENGL
	//static DefaultCube c;
	//c.RenderDirect();
	//c.RenderArrayBuffer();
	//c.RenderIndexBuffer();

	//warrior->Draw();
	//walk->Draw();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

GameObject* ModuleSceneIntro::ImportGameObject(char* filepath)
{
	
	ComponentMesh* CMesh = new ComponentMesh(Importer::Meshes::Import(filepath));

	GameObject* Go = new GameObject();
	Component* component = CMesh;
	Go->AddComponent(component);	
	game_objects.push_back(Go);
	return Go;
}
