#pragma once

#include "Component.h"
#include "Dependencies/MathGeolib/Geometry/Frustum.h"
#include "Dependencies/MathGeolib/Geometry/Plane.h"

class ComponentCamera : public Component
{
public:
	ComponentCamera();
	ComponentCamera(GameObject* gameObject);
	~ComponentCamera();

	float GetNearPlaneDistance() const;
	float GetFarPlaneDistance() const;
	float GetVerticalFOV() const;
	float GetHoritzontalFOV() const;
	float GetAspectRatio() const;

	void SetNearPlaneDistance(float distance);
	void SetFarPlaneDistance(float distance);
	void SetVerticalFOV(float fov);
	void SetHoritzontalFOV(float fov);
	void SetVerticalAspectRatio(float ratio);		//Aspect ratio set with the vertical fov modification
	void SetHoritzontalAspectRatio(float ratio);	//Aspect ratio set with the horitzontal fov modification

	void Look(const float3& position);

	float4x4 GetGLViewMatrix() const;
	float4x4 GetGLProjectionMatrix() const;


public:

	Frustum frustum;
	bool culling;
	bool active;
	bool updatePMatrix;

	Plane frustumPlanes[6];
};