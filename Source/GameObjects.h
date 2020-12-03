#ifndef __GameObjects_H__
#define __GameObjects_H__

#include <vector>
#include <string>
#include "MathGeoLib.h"

class Component;
enum class ComponentType;

class GameObject
{
public:
	GameObject();
	GameObject(GameObject* parent, const char* iName = "noName",float3 transf = float3::zero,float3 scale = float3::one, Quat rot = Quat::identity, bool active = true);
	~GameObject();

	//	add	a	new	component	to	this	game	object
	void AddComponent(Component* c);

	//void Start();
	void Update(float dt);
	void Draw();

	GameObject* GetParent() const;
	void SetParent(GameObject* newParent);
	const Component* GetComponent(ComponentType type) const;
	bool HasComponent(ComponentType type) const;
	std::vector<GameObject*> GetChildren() const;

	const char* GetName() const;
	void SetName(char* newName);

private:
	GameObject* parent = nullptr;
	std::string name;

	bool active;
	
	//TODO: Change children to private
public:
	std::vector<Component*> components;
	std::vector<GameObject*> children;

};


#endif //__GameObjects_H__