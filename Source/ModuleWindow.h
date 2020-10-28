#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL.h"

class ModuleWindow : public Module
{
public:

	ModuleWindow(bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;


private:

	struct WindowConfig {
		int screen_width;
		int screen_height;
		bool fullscreen;
		bool resizable;
		bool border;
		bool full_desktop;
	};

	WindowConfig w_config;

public:

	//Windows Parameters
	int  GetWidth();
	int  GetHeight();
	const int GetRefreshRate() const;
	bool GetFullscreen();
	bool GetResizable();
	bool GetBorderless();
	bool GetFullscreenDesktop();
	bool GetBrightness();

	void SetTitle(const char* title);
	void SetSize(int width, int height);
	void SetFullscreen(bool activate);
	void SetResizable(bool activate);
	void SetBorderless(bool activate);
	void SetFullscreenDesktop(bool activate);
	void SetBrightness(float brightness);
};

#endif // __ModuleWindow_H__