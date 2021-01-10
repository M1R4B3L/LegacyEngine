#pragma once
#include "..\Source\Dependencies\SDL2\include\SDL_scancode.h"

class __declspec(dllexport) Input {
public:

	enum KEY_STATE {
		KEY_IDLE = 0,
		KEY_DOWN,
		KEY_REPEAT,
		KEY_UP
	};

	enum MOUSE_BUTTONS {
		MOUSE_LEFT_BUTTON = 1,
		MOUSE_MIDDLE_BUTTON = 2,
		MOUSE_RIGHT_BUTTON = 3,
	};

	bool GetKeyDown(SDL_Scancode& key);
	bool GetKeyUp(SDL_Scancode& key);
	bool GetKeyRepeat(SDL_Scancode& key);

	int GetMouseX();
	int GetMouseY();
	int GetMouseWheel();

	bool GetMouseButtonDown(MOUSE_BUTTONS& button);
	bool GetMouseButtonUp(MOUSE_BUTTONS& button);
	bool GetMouseButtonRepeat(MOUSE_BUTTONS& button);

};