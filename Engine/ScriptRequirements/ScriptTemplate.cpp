#include "Engine.h"

Input input;

float speed = 3.0f;
float angle = 0.0f;
float velocity = 0.0f;

extern "C" __declspec(dllexport) void Start(GameObject* object) {
	ConsoleLog("Hi Scripter");
	
}

extern "C" __declspec(dllexport) void Update(GameObject * object) {

	ComponentTransform* transform = (ComponentTransform*)object->GetComponent(ComponentType::Transform);
	float3 translation; Quat rotation; float3 scale;
	if (transform != nullptr) {
		transform->GetLocalTransform(translation, scale, rotation);

		float3 forward = float3((rotation.x * rotation.z + rotation.w * rotation.y),(rotation.y * rotation.z - rotation.w * rotation.x), (rotation.x * rotation.x + rotation.y * rotation.y));

		if (input.GetKeyRepeat(SDL_SCANCODE_UP)) {
		
			velocity += speed*GetGameTime();	
			//ConsoleLog("%f", translation.z);
			transform->SetLocalTransform(forward * velocity, scale, rotation);
			ConsoleLog("Moving Up!");
		}
		if (input.GetKeyRepeat(SDL_SCANCODE_DOWN)) {
		
			velocity -= speed * GetGameTime();
			transform->SetLocalTransform(forward * velocity, scale, rotation);
			ConsoleLog("Moving Down!");
		}
		if (input.GetKeyRepeat(SDL_SCANCODE_RIGHT)) {
			
			angle -= speed * GetGameTime();
			transform->SetLocalTransform(translation, scale, Quat::FromEulerXYX(0, angle * 0.01745329252, 0));
			ConsoleLog("Moving Right!");
		}
		if (input.GetKeyRepeat(SDL_SCANCODE_LEFT)) {
		
			angle += speed * GetGameTime();
			transform->SetLocalTransform(translation, scale, Quat::FromEulerXYX(0, angle * 0.01745329252, 0));
			ConsoleLog("Moving Left!");	
		}

	}
}

extern "C" __declspec(dllexport) const char* GetName() {
	return "Move";
}
