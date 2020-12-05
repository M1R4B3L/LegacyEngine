#pragma once

#include "Component.h"
#include "Dependencies/MathGeolib/Geometry/Frustum.h"

class ComponentCamera : public Component
{
public:
	ComponentCamera(GameObject* gameObject);
	~ComponentCamera();

public:
	Frustum frustum;
	bool culling;
	bool active;

};