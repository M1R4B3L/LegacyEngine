#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL.h"

class ModuleWindow : public Module
{
public:

	ModuleWindow(bool startEnable = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screenSurface;


private:

	struct WindowConfig {
		int screenWidth;
		int screenHeight;
		bool fullscreen;
		bool resizable;
		bool border;
		bool fullDesktop;
	};

	WindowConfig windowConfig;

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