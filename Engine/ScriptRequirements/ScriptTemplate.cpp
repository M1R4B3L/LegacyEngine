#include "Engine.h"

Input input;


float speed = 50.0f;
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

		float4x4 inverse = transform->GetGlobalTransform();

		float3 colum = inverse.Col3(2);
		float3 forward = colum.Normalized();

		if (input.GetKeyRepeat(SDL_SCANCODE_UP)) {
		
			velocity += speed *GetGameTime();	

			ConsoleLog(std::to_string(GetGameTime()).c_str());
			ConsoleLog("Moving Up!");
		}
		if (input.GetKeyRepeat(SDL_SCANCODE_DOWN)) {
		
			velocity -= speed * GetGameTime();
			
			ConsoleLog("Moving Down!");
		}
		if (input.GetKeyRepeat(SDL_SCANCODE_RIGHT)) {
			
			angle -= speed * GetGameTime();

			rotation = rotation.RotateY(angle * 0.01745329252);

			ConsoleLog("Moving Right!");
		}
		if (input.GetKeyRepeat(SDL_SCANCODE_LEFT)) {
		
			angle += speed * GetGameTime();

			rotation = rotation.RotateY(angle * 0.01745329252);
	
			ConsoleLog("Moving Left!");	
		}

		transform->SetLocalTransform(forward*velocity, scale, rotation);
	}
}

extern "C" __declspec(dllexport) const char* GetName() {
	return "Move";
}
