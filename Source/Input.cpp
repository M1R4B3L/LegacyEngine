#include "Input.h"
#include "Application.h"
#include "ModuleInput.h"

bool Input::GetKeyDown(const SDL_Scancode& key)
{
	return (App->input->GetKey(key) == KEY_STATE::KEY_DOWN);
}

bool Input::GetKeyUp(const SDL_Scancode& key)
{
	return (App->input->GetKey(key) == KEY_STATE::KEY_UP);
}

bool Input::GetKeyRepeat(const SDL_Scancode& key)
{
	return (App->input->GetKey(key) == KEY_STATE::KEY_REPEAT);
}

int Input::GetMouseX()
{
	return App->input->GetMouseX();
}

int Input::GetMouseY()
{
	return App->input->GetMouseY();
}
int Input::GetMouseWheel()
{
	return App->input->GetMouseZ();
}

bool Input::GetMouseButtonDown(MOUSE_BUTTONS& button)
{
	return (App->input->GetMouseButton(button) == KEY_DOWN);
}

bool Input::GetMouseButtonUp(MOUSE_BUTTONS& button)
{
	return (App->input->GetMouseButton(button) == KEY_UP);
}

bool Input::GetMouseButtonRepeat(MOUSE_BUTTONS& button)
{
	return (App->input->GetMouseButton(button) == KEY_REPEAT);
}
