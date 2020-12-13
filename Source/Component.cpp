#include "Component.h"
#include "GameObjects.h"

Component::Component(ComponentType iType) : gameObject(nullptr), type(iType) {}

Component::Component(GameObject* go, ComponentType iType = ComponentType::Unknown) : gameObject(go), type(iType)
{ 
	//TODO: Attach directly components on the constructor ?
	go->AddComponent(this); 
}

bool Component::IsActive() const
{
	return activeComponent;
}

void Component::SetActive(bool active)
{
	activeComponent = active;
}

const GameObject& Component::GetGameObject() const
{
	return *gameObject;
}

const ComponentType Component::GetType() const
{
	return type;
}

GameObject& Component::GetGameObject()
{
	return *gameObject;
}

void Component::SetGameObject(GameObject& go)
{
	gameObject = &go;
}

void Component::RemoveGameObjectComponent(GameObject& go, ComponentType type)
{
	std::vector<Component*>::iterator it = go.components.begin();
	for (it; it != go.components.end(); ++it)
	{
		if ((*it)->type == type)
		{
			go.components.erase(it);
			break;
		}
	}
}
