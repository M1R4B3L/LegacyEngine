#include "Engine.h"

extern "C" __declspec(dllexport) void Start(GameObject* object) {
	ConsoleLog("Hi Scripter");
}

extern "C" __declspec(dllexport) void Update(GameObject* object) {

}

extern "C" __declspec(dllexport) const char* GetName() {
	return "ScriptName";
}