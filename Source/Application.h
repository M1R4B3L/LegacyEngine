#pragma once

#include <list>
#include <array>
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
//#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	//ModuleAudio* audio;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	std::array<float,25>fps_log;

private:

	Timer	ms_timer;
	Timer   lastSecFrameTime;
	unsigned __int32 lastSecFrameCount = 0;
	float	dt;
	int capped_ms = -1;
	std::list<Module*> list_modules;

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