#pragma once
#ifndef __Application_H__
#define __Application_H__

#include "Globals.h"
#include "Timer.h"
#include "Module.h"

#include <vector>
#include <array>
#include <string>

class ModuleWindow;
class ModuleInput;
class ModuleScene;
class ModuleRenderer3D;
class ModuleCamera3D;
class ModuleEditor;
class ModuleFileSystem;
class ModuleResources;

class Application
{
public:

	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleScene* scene = nullptr;
	ModuleRenderer3D* renderer3D = nullptr;
	ModuleCamera3D* camera = nullptr;
	ModuleEditor* editor = nullptr;
	ModuleFileSystem* fileSystem = nullptr;
	ModuleResources* resources = nullptr;

	std::array<float,50>fpsLog;
	std::array<float,50>msLog;

	const char* GetEngineTitle() const;
	void SetEngineTitle(const char* title);

	bool closeApp;
private:

	Timer	msTimer;
	Timer   lastSecFrameTime;
	unsigned __int32 lastSecFrameCount = 0;
	float	dt;
	float cappedMs = 0;
	//Game mode
	float gameDt;

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


	void AddConsoleLog(const char* string);

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

};

extern Application* App;

#endif // __Application_H__