#include "Application.h"

#include <list>


Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	//audio = new ModuleAudio(this, true);
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
	//AddModule(audio);
	
	// Scenes
	AddModule(scene_intro);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	std::list <Module*>::reverse_iterator it = list_modules.rbegin();

	for (it; it != list_modules.rend(); it++)
	{
		if (*it != NULL)
		{
			delete (*it);
			*it = NULL;
		}
	}

	list_modules.clear();
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	std::list<Module*>::iterator it = list_modules.begin();

	for(it; it != list_modules.end() && ret == true; it++)
	{
		ret = (*it)->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	it = list_modules.begin();

	for(it; it != list_modules.end() && ret == true; it++)
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
		fps_log[fps_log.size() - 1] = lastSecFrameCount;
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
	
	std::list<Module*>::iterator it = list_modules.begin();	
	for (it; it != list_modules.end() && ret == UPDATE_CONTINUE; it++)
	{
		ret = (*it)->PreUpdate(dt);
	}

	it = list_modules.begin();
	for (it; it != list_modules.end() && ret == UPDATE_CONTINUE; it++)
	{
		ret = (*it)->Update(dt);
	}

	it = list_modules.begin();
	for (it; it != list_modules.end() && ret == UPDATE_CONTINUE; it++)
	{
		ret = (*it)->PostUpdate(dt);
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	std::list <Module*>::reverse_iterator it = list_modules.rbegin();
	for (it; it != list_modules.rend() && ret == true; it++)
	{
		ret = (*it)->CleanUp();
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}