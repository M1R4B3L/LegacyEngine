#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

#include "Module.h"
#include "Globals.h"


struct Hardware {

	SDL_version H_SDLVersion;
	int H_CPU;
	int H_CPUCache;
	float H_SystemRAM;
	bool H_rdtsc;
	bool H_3dnow;
	bool H_altivec;
	bool H_avx;
	bool H_avx2;
	bool H_mmx;
	bool H_sse;
	bool H_sse2;
	bool H_sse3;
	bool H_sse41;
	bool H_sse42;

};

class ModuleEditor : public Module
{
public:
	ModuleEditor(bool start_enable = true);
	~ModuleEditor();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();

	//IMGUI!!!!
	bool docking_window;
	bool about_window;
	bool config_window;
	bool console_window;
	bool inspector_window;
	bool hierarchy_window;
	bool demo_window;

	char org[128];

	//Config Window

	void WindowDocking();
	void MenuBar();
	void WindowAbout();
	void WindowConfig();
	void WindowConsole();
	void WindowInspector();
	void WindowHierarchy();
	void WindowDemo();

	void AddLog(const char*);
	void ClearLog();

private:

	std::vector<char*> string_log;
	bool scroll;

};

#endif //__ModuleEditor_H__