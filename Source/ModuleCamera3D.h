#ifndef __ModuleCamera3D_H__
#define __ModuleCamera3D_H__

#include "Module.h"
#include "Globals.h"
#include "Dependencies/MathGeolib/Math/float3.h"
#include "Dependencies/MathGeolib/Geometry/LineSegment.h"

class GameObject;
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

	void LookAt(const float3& pos);
	void MoveTo(const float3& pos);
	void Look(const float3& pos, float distance);

	void WASDMovement(float dt);
	void DragCamera(float dt);

	void WorldRotation(float dt);
	void ReferenceRotation(float dt);

	void Zoom(float dt);
	void Focus(GameObject* gameObject);
	
	void LeftClick();

	void DrawRay();

public:

	float moveSpeed;
	float rotateSpeed;
	float zoomSpeed;

private:

	float3 reference;
	ComponentCamera* cameraMain = nullptr;
	LineSegment rayPicking;
};

#endif //__ModuleCamera3D_H__