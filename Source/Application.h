#pragma once
#include "Globals.h"
#include "Timer.h"
#include "Module.h"

#include <vector>
#include <array>

class ModuleWindow;
class ModuleInput;
class ModuleSceneIntro;
class ModuleRenderer3D;
class ModuleCamera3D;

class Application
{
public:

	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleSceneIntro* scene_intro = nullptr;
	ModuleRenderer3D* renderer3D = nullptr;
	ModuleCamera3D* camera = nullptr;
	std::array<float,50>fps_log;

private:

	Timer	ms_timer;
	Timer   lastSecFrameTime;
	unsigned __int32 lastSecFrameCount = 0;
	float	dt;
	int capped_ms = -1;

	std::vector<Module*> modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	void CapFramerate(int fps);

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

extern Application* App;