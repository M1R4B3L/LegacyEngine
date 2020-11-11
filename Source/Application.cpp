#include "Application.h"

#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleSceneIntro.h"
#include "ModuleEditor.h"

#include <list>

Application::Application() : title("Legacy Engine"),
close_app(false)
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	editor = new ModuleEditor(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	
	// Scenes
	AddModule(scene_intro);
	AddModule(editor);

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
		capped_ms = (1.f / ((float)fps / 1000.f));
	}
	else
		capped_ms = 0;
}

uint Application::GetFramerate()
{
	if (capped_ms > 0)
	{
		return (uint)((1.0f / capped_ms) * 1000.0f);
	}
	else
	{
		return 0;
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
	ms_log[ms_log.size() - 1] = last_frame_ms;
	//LOG("Last_frame_ms: %d", last_frame_ms);
	
	for (int i = ms_log.size() - 2; i >= 0; --i) {
		ms_log[i] = ms_log[i + 1];
	}
	
	if (capped_ms > 0 && last_frame_ms < capped_ms)
	{
		SDL_Delay((uint)(capped_ms - last_frame_ms));
	}
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	if (close_app)
	{
		return UPDATE_STOP;
	}

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


void Application::AddConsoleLog(const char* string)
{
	if (App->close_app == false)
	{
		if (App->scene_intro != nullptr)
		{
			std::string tmp_log = string;
			//Ty Angel
			uint log_start_position = tmp_log.find_last_of("\\") + 1;
			uint log_end_position = tmp_log.size();

			std::string short_log = tmp_log.substr(log_start_position, log_end_position);

			editor->AddLog(short_log.c_str());

			tmp_log.clear();
			short_log.clear();
		}
	}
}
