#include "ComponentCamera.h"

ComponentCamera::ComponentCamera(GameObject* gameObject) : Component(gameObject, ComponentType::Camera), active(true), culling(false)
{
	frustum;
}

ComponentCamera::~ComponentCamera()
{
}
