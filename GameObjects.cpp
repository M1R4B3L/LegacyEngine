#include "GameObjects.h"



GameObjects::GameObjects()
{
	active = true;
}

GameObjects::~GameObjects()
{
}

Component* GameObjects::CreateComponent(Component::ComponentType type)
{
	return nullptr;
}

void Component::Enable()
{
}

void Component::Update()
{
}

void Component::Disable()
{
}
