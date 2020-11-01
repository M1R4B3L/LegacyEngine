#ifndef __GameObjects_H__
#define __GameObjects_H__

#include <vector>
#include <string>

class Component;
enum class ComponentType;

class GameObject
{
public:
	GameObject();
	GameObject(const std::string&& name);
	~GameObject();

	//	add	a	new	component	to	this	game	object
	void AddComponent(Component* c);
	
	Component* CreateComponent(ComponentType type);

	void Start();
	void Update(float dt);



private:
	GameObject* parent;
	std::string m_Name;
	std::vector<Component*> m_Components;
	std::vector<GameObject*> children;
};


#endif //__GameObjects_H__