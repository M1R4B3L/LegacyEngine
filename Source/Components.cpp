#include "Components.h"

Component::Component() : m_GameObject(nullptr) {}

const GameObject& Component::GetGameObject() const
{
	return	*m_GameObject;
}

GameObject& Component::GetGameObject()
{
	return	*m_GameObject;
}

void Component::SetGameObject(GameObject& go)
{
	m_GameObject = &go;
}
