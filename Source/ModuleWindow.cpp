#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(bool startEnable) : Module(startEnable)
{
	window = NULL;
	screenSurface = NULL;

	windowConfig.fullscreen = WIN_FULLSCREEN;
	windowConfig.resizable = WIN_RESIZABLE;
	windowConfig.border = WIN_BORDERLESS;
	windowConfig.fullDesktop = WIN_FULLSCREEN_DESKTOP;
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
		windowConfig.screenWidth = SCREEN_WIDTH * SCREEN_SIZE;
		windowConfig.screenHeight = SCREEN_HEIGHT * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		/*
		//Use OpenGL 4.6
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
		*/
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 1);

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

		window = SDL_CreateWindow(App->GetEngineTitle(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowConfig.screenWidth, windowConfig.screenHeight, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screenSurface = SDL_GetWindowSurface(window);
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
	return windowConfig.screenWidth;
}

int ModuleWindow::GetHeight()
{
	return windowConfig.screenHeight;
}

void ModuleWindow::SetSize(int width, int height)
{
	windowConfig.screenWidth = width;
	windowConfig.screenHeight = height;

	SDL_SetWindowSize(window, windowConfig.screenWidth, windowConfig.screenHeight);
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
	return windowConfig.fullscreen;
}

void ModuleWindow::SetFullscreen(bool activate)
{
	if (activate != windowConfig.fullscreen)
	{
		if (activate == true)
		{
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
			LOG("[WINDOWS] Set Fullscreen Mode");
			windowConfig.fullscreen = activate;
		}
		else
		{
			SDL_SetWindowFullscreen(window, 0);
			LOG("[WINDOWS] Set Windows Mode");
			windowConfig.fullscreen = activate;
			windowConfig.fullDesktop = activate;
		}
	}
}

bool ModuleWindow::GetResizable()
{
	return windowConfig.resizable;
}

void ModuleWindow::SetResizable(bool activate)
{

	if (activate != windowConfig.resizable)
	{
		if (activate == true)
		{
			SDL_SetWindowResizable(window, SDL_TRUE);
			LOG("[WINDOWS] Resizable");
			windowConfig.resizable = activate;
		}
		else
		{
			SDL_SetWindowResizable(window, SDL_FALSE);
			LOG("[WINDOWS] Not Resizable");
			windowConfig.resizable = activate;
		}
	}
}

bool ModuleWindow::GetBorderless()
{
	return windowConfig.border;
}

void ModuleWindow::SetBorderless(bool activate)
{

	if (activate != windowConfig.border && windowConfig.fullscreen == false && windowConfig.fullDesktop == false)
	{
		if (activate == true)
		{
			SDL_SetWindowBordered(window, SDL_FALSE);
			LOG("[WINDOWS] Borderless");
			windowConfig.border = activate;
		}
		else
		{
			SDL_SetWindowBordered(window, SDL_TRUE);
			LOG("[WINDOWS] Bordered");
			windowConfig.border = activate;
		}
	}
}

bool ModuleWindow::GetFullscreenDesktop()
{
	return windowConfig.fullDesktop;
}

void ModuleWindow::SetFullscreenDesktop(bool activate)
{
	if (activate != windowConfig.fullDesktop)
	{
		if (activate == true)
		{
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			LOG("[WINDOWS] Set Fullscreen Desktop Mode");
			windowConfig.fullDesktop = activate;
		}
		else
		{
			SDL_SetWindowFullscreen(window, 0);
			LOG("[WINDOWS] Set Windows Mode");
			windowConfig.fullscreen = activate;
			windowConfig.fullDesktop = activate;
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