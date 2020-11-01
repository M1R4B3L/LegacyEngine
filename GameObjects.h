#pragma once
#ifndef __GameObjects_H__
#define __GameObjects_H__

#include <vector>

class GameObjects;

struct Component
{
	enum ComponentType
	{
		component_transform,
		component_mesh,
		component_material,
	};

	GameObjects* parent = nullptr;

	virtual void Enable();
	virtual void Update();
	virtual void Disable();
};

class GameObjects
{
public:

	GameObjects();
	~GameObjects();

	bool active;
	std::vector<Component*> components;

	Component* CreateComponent(Component::ComponentType type);

};
#endif //__GameObjects_H__