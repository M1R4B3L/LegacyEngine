#pragma once
#include "Globals.h"
#include "Timer.h"
#include "Module.h"

#include <vector>
#include <array>
#include <string>

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
	std::array<float,50>ms_log;

	const char* GetEngineTitle() const;
	void SetEngineTitle(const char* title);

	bool close_app;
private:

	Timer	ms_timer;
	Timer   lastSecFrameTime;
	unsigned __int32 lastSecFrameCount = 0;
	float	dt;
	int capped_ms = 1000/60;

	//Windows
	std::string title;

	std::vector<Module*> modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	void CapFramerate(int fps);
	uint GetFramerate();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

extern Application* App;