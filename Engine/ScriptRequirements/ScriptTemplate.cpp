#include "Engine.h"

Input input;

float speed = 3.0f;

extern "C" __declspec(dllexport) void Start(GameObject* object) {
	ConsoleLog("Hi Scripter");
	
}

extern "C" __declspec(dllexport) void Update(GameObject* object) {

	if (input.GetKeyRepeat(SDL_SCANCODE_UP)) {
		ComponentTransform* transform = (ComponentTransform*)object->GetComponent(ComponentType::Transform);
		if (transform != nullptr) {
			float3 translation; Quat rotation; float3 scale;
			transform->GetLocalTransform(translation, scale, rotation);
			translation.z += speed * GetGameTime();
			transform->SetLocalTransform(translation, scale, rotation);
			ConsoleLog("%f", translation.z);
			ConsoleLog("Moving Up!!!!!!!");
		}
	}
	/*else if (input.GetKeyRepeat(SDL_SCANCODE_DOWN)) {

	}
	else if (input.GetKeyRepeat(SDL_SCANCODE_RIGHT)) {

	}
	else if (input.GetKeyRepeat(SDL_SCANCODE_LEFT)) {

	}*/
}

extern "C" __declspec(dllexport) const char* GetName() {
	return "Move";
}