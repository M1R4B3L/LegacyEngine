#include "Application.h"

#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleSceneIntro.h"

#include <list>

Application::Application() : title("Legacy Engine")
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	
	// Scenes
	AddModule(scene_intro);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	std::vector<Module*>::reverse_iterator it = modules.rbegin();

	for (it; it != modules.rend(); it++)
	{
		if (*it != NULL)
		{
			delete (*it);
			*it = NULL;
		}
	}

	modules.clear();
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	std::vector<Module*>::iterator it = modules.begin();

	for(it; it != modules.end() && ret == true; it++)
	{
		ret = (*it)->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	it = modules.begin();

	for(it; it != modules.end() && ret == true; it++)
	{
		ret = (*it)->Start();
		
	}

	ms_timer.Start();
	return ret;
}

void Application::CapFramerate(int fps) {
	if (fps > 0)
	{
		capped_ms = 1000 / fps;
	}
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	lastSecFrameCount++;
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	if (lastSecFrameTime.Read() > 1000)
	{
		lastSecFrameTime.Start();
		fps_log[fps_log.size() - 1] = lastSecFrameCount - 1;
		lastSecFrameCount = 0;

		for (int i = fps_log.size() - 2; i >= 0; --i) {
			fps_log[i] = fps_log[i + 1];
		}
	}

	unsigned __int32 last_frame_ms = ms_timer.Read();

	if (capped_ms > 0 && last_frame_ms < capped_ms)
	{
		SDL_Delay(capped_ms - last_frame_ms);
	}
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	std::vector<Module*>::iterator it = modules.begin();
	for (it; it != modules.end() && ret == UPDATE_CONTINUE; it++)
	{
		ret = (*it)->PreUpdate(dt);
	}

	it = modules.begin();
	for (it; it != modules.end() && ret == UPDATE_CONTINUE; it++)
	{
		ret = (*it)->Update(dt);
	}

	it = modules.begin();
	for (it; it != modules.end() && ret == UPDATE_CONTINUE; it++)
	{
		ret = (*it)->PostUpdate(dt);
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	std::vector <Module*>::reverse_iterator it = modules.rbegin();
	for (it; it != modules.rend() && ret == true; it++)
	{
		ret = (*it)->CleanUp();
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	modules.push_back(mod);
}

const char* Application::GetEngineTitle() const
{
	return title.c_str();
}

void Application::SetEngineTitle(const char* title)
{
	this->title = title;
	App->window->SetTitle(title);
}