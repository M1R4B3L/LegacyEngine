#include "Application.h"

#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleScene.h"
#include "ModuleEditor.h"

#include <list>

Application::Application() : title("Legacy Engine"),
closeApp(false)
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	scene = new ModuleScene(this);
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
	AddModule(scene);
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

	msTimer.Start();
	return ret;
}

void Application::CapFramerate(int fps) {
	if (fps > 0)
	{
		cappedMs = (1.f / ((float)fps / 1000.f));
	}
	else
		cappedMs = 0;
}

uint Application::GetFramerate()
{
	if (cappedMs > 0)
	{
		return (uint)((1.0f / cappedMs) * 1000.0f);
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
	dt = (float)msTimer.Read() / 1000.0f;
	msTimer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	if (lastSecFrameTime.Read() > 1000)
	{
		lastSecFrameTime.Start();
		fpsLog[fpsLog.size() - 1] = lastSecFrameCount;
		lastSecFrameCount = 0;

		for (int i = fpsLog.size() - 2; i >= 0; --i) {
			fpsLog[i] = fpsLog[i + 1];
		}
	}

	unsigned __int32 lastFrameMs = msTimer.Read();
	msLog[msLog.size() - 1] = lastFrameMs;
	//LOG("Last_frame_ms: %d", last_frame_ms);
	
	for (int i = msLog.size() - 2; i >= 0; --i) {
		msLog[i] = msLog[i + 1];
	}
	
	if (cappedMs > 0 && lastFrameMs < cappedMs)
	{
		SDL_Delay((uint)(cappedMs - lastFrameMs));
	}
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	if (closeApp)
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
	if (App->closeApp == false)
	{
		if (App->scene != nullptr)
		{
			std::string tmpLog = string;
			//Ty Angel
			uint StartPositionLog = tmpLog.find_last_of("\\") + 1;
			uint EndPositionLog = tmpLog.size();

			std::string ShortLog = tmpLog.substr(StartPositionLog, EndPositionLog);

			editor->AddLog(ShortLog.c_str());

			tmpLog.clear();
			ShortLog.clear();
		}
	}
}
