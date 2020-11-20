#include "Component.h"

//Component::Component() : gameObject(nullptr),type(ComponentType::Unknown),active(true) {}

Component::Component(GameObject* go, ComponentType iType = ComponentType::Unknown, bool iActive = true): gameObject(go), type(iType), active(iActive) {}

const GameObject& Component::GetGameObject() const
{
	return	*gameObject;
}

GameObject& Component::GetGameObject()
{
	return	*gameObject;
}

void Component::SetGameObject(GameObject& go)
{
	gameObject = &go;
}
