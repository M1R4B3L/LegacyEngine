#pragma once
#include "Utils.h"
#include "Application.h"
#include "Globals.h"

float GetGameTime()
{
	return App->GetGameDt();
}

void ConsoleLog(const char* message)
{
	LOG("%s", message);
}
