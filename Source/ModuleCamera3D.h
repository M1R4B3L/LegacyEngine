#ifndef __ModuleCamera3D_H__
#define __ModuleCamera3D_H__

#include "Module.h"
#include "Globals.h"
#include "Dependencies/MathGeolib/Math/float3.h"

class ComponentCamera;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool startEnable = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	ComponentCamera* GetCamera() const;

	void LookAt(const float3& Spot);

	/*void WASDMovement(float dt);
	void WorldTranslation(float dt);
	void WorldRotation(float dt);
	void ReferenceRotation(float dt);
	void Zoom(float dt);

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	
	void Move(const vec3 &Movement);
	float* GetViewMatrix();

private:

	void CalculateViewMatrix();*/

public:

	float moveSpeed;
	float rotateSpeed;
	float zoomSpeed;

private:

	float3 reference;
	ComponentCamera* cameraMain = nullptr;
};

#endif //__ModuleCamera3D_H__