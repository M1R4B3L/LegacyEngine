#include "Importer.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "Importer.h"
#include "examples/imgui_impl_sdl.h"
#include <string>

#define MAX_KEYS 300

ModuleInput::ModuleInput(bool startEnable) : Module(startEnable)
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouseButtons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouseX, &mouseY);

	mouseX /= SCREEN_SIZE;
	mouseY /= SCREEN_SIZE;
	mouseZ = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(mouseButtons[i] == KEY_IDLE)
				mouseButtons[i] = KEY_DOWN;
			else
				mouseButtons[i] = KEY_REPEAT;
		}
		else
		{
			if(mouseButtons[i] == KEY_REPEAT || mouseButtons[i] == KEY_DOWN)
				mouseButtons[i] = KEY_UP;
			else
				mouseButtons[i] = KEY_IDLE;
		}
	}

	mouseXMotion = mouseYMotion = 0;

	bool quit = false;
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		ImGui_ImplSDL2_ProcessEvent(&e);
		//TODO: BLOCK PRESSING KEYS ON MOUSE AND KEYBOARD
		ImGuiUseKeyboard = ImGui::GetIO().WantCaptureKeyboard;
		ImGuiUseMouse = ImGui::GetIO().WantCaptureMouse;
		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
			if(!ImGuiUseMouse)
				mouseZ = e.wheel.y;
			break;

			case SDL_MOUSEMOTION:
			if (!ImGuiUseMouse) {
				mouseX = e.motion.x / SCREEN_SIZE;
				mouseY = e.motion.y / SCREEN_SIZE;

				mouseXMotion = e.motion.xrel / SCREEN_SIZE;
				mouseYMotion = e.motion.yrel / SCREEN_SIZE;
			}
			break;

			case SDL_QUIT:
			quit = true;
			break;

			case SDL_DROPFILE: {
				char* file = e.drop.file;
				LOG("DROPED file: %s",e.drop.file);
				Importer::ImportDroped(e.drop.file);
				SDL_free(file);
				break; 
			}

			case SDL_WINDOWEVENT:
			{
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					App->renderer3D->OnResize(e.window.data1, e.window.data2);
			}
		}
	}

	if(quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}