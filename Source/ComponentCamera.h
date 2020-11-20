#pragma once

#include "Component.h"

#include "Dependencies/MathGeoLib/include/Geometry/Frustum.h"

class ComponentCamera : public Component
{
public:

	Frustum frustrum_camera;

};