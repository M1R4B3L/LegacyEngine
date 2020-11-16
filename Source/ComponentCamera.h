#ifndef __ComponentCamera_H__
#define __ComponentCamera_H__

#include "Components.h"

#include "Dependencies/MathGeoLib/include/Geometry/Frustum.h"

class ComponentCamera : public Component
{
public:

	Frustum frustrum_camera;

};

#endif //__ComponentCamera_H__