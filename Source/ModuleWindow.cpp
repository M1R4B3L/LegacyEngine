#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(bool start_enabled) : Module(start_enabled)
{
	window = NULL;
	screen_surface = NULL;

	w_config.fullscreen = WIN_FULLSCREEN;
	w_config.resizable = WIN_RESIZABLE;
	w_config.border = WIN_BORDERLESS;
	w_config.full_desktop = WIN_FULLSCREEN_DESKTOP;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		w_config.screen_width = SCREEN_WIDTH * SCREEN_SIZE;
		w_config.screen_height = SCREEN_HEIGHT * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3.3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(App->GetEngineTitle(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w_config.screen_width, w_config.screen_height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

int ModuleWindow::GetWidth()
{
	return w_config.screen_width;
}

int ModuleWindow::GetHeight()
{
	return w_config.screen_height;
}

void ModuleWindow::SetSize(int width, int height)
{
	w_config.screen_width = width;
	w_config.screen_height = height;

	SDL_SetWindowSize(window, w_config.screen_width, w_config.screen_height);
}

const int ModuleWindow::GetRefreshRate() const
{
	SDL_DisplayMode current;

	if (SDL_GetCurrentDisplayMode(0, &current) != 0)
	{
		LOG("[WINDOWS] Could not get display mode for video display %s", SDL_GetError());
	}
	else
	{
		return current.refresh_rate;
	}
}

bool ModuleWindow::GetFullscreen()
{
	return w_config.fullscreen;
}

void ModuleWindow::SetFullscreen(bool activate)
{
	if (activate != w_config.fullscreen)
	{
		if (activate == true)
		{
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
			LOG("[WINDOWS] Set Fullscreen Mode");
			w_config.fullscreen = activate;
		}
		else
		{
			SDL_SetWindowFullscreen(window, 0);
			LOG("[WINDOWS] Set Windows Mode");
			w_config.fullscreen = activate;
			w_config.full_desktop = activate;
		}
	}
}

bool ModuleWindow::GetResizable()
{
	return w_config.resizable;
}

void ModuleWindow::SetResizable(bool activate)
{

	if (activate != w_config.resizable)
	{
		if (activate == true)
		{
			SDL_SetWindowResizable(window, SDL_TRUE);
			LOG("[WINDOWS] Resizable");
			w_config.resizable = activate;
		}
		else
		{
			SDL_SetWindowResizable(window, SDL_FALSE);
			LOG("[WINDOWS] Not Resizable");
			w_config.resizable = activate;
		}
	}
}

bool ModuleWindow::GetBorderless()
{
	return w_config.border;
}

void ModuleWindow::SetBorderless(bool activate)
{

	if (activate != w_config.border && w_config.fullscreen == false && w_config.full_desktop == false)
	{
		if (activate == true)
		{
			SDL_SetWindowBordered(window, SDL_FALSE);
			LOG("[WINDOWS] Borderless");
			w_config.border = activate;
		}
		else
		{
			SDL_SetWindowBordered(window, SDL_TRUE);
			LOG("[WINDOWS] Bordered");
			w_config.border = activate;
		}
	}
}

bool ModuleWindow::GetFullscreenDesktop()
{
	return w_config.full_desktop;
}

void ModuleWindow::SetFullscreenDesktop(bool activate)
{
	if (activate != w_config.full_desktop)
	{
		if (activate == true)
		{
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			LOG("[WINDOWS] Set Fullscreen Desktop Mode");
			w_config.full_desktop = activate;
		}
		else
		{
			SDL_SetWindowFullscreen(window, 0);
			LOG("[WINDOWS] Set Windows Mode");
			w_config.fullscreen = activate;
			w_config.full_desktop = activate;
		}
	}
}

bool ModuleWindow::GetBrightness()
{
	return SDL_GetWindowBrightness(window);
}

void ModuleWindow::SetBrightness(float brightness)
{
	SDL_SetWindowBrightness(window, brightness);	
}