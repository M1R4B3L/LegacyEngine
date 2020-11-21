#include "Component.h"
#include "GameObjects.h"

Component::Component(ComponentType iType) : gameObject(nullptr), type(iType),active(true) {}

Component::Component(GameObject* go, ComponentType iType = ComponentType::Unknown, bool iActive = true) : gameObject(go), type(iType), active(iActive) 
{ 
	//TODO: Attach directly components on the constructor ?
	go->AddComponent(this); 
}

const GameObject& Component::GetGameObject() const
{
	return	*gameObject;
}

const ComponentType Component::GetType() const
{
	return type;
}

GameObject& Component::GetGameObject()
{
	return	*gameObject;
}

void Component::SetGameObject(GameObject& go)
{
	gameObject = &go;
}
