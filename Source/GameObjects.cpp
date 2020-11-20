#include "GameObjects.h"
#include "Component.h"

GameObject::GameObject() : parent(nullptr), name("No name")
{
}

GameObject::GameObject(GameObject * iParent, const char* iName, float3 transf, float3 scale, Quat rot) : parent(iParent), name(iName) 
{
	if (!parent) 
	{
		iParent->children.push_back(this);
	}
	

}

GameObject::~GameObject()
{
	std::vector<Component*>::iterator component = components.begin();
	for (component;component!= components.end();component++)	
		delete (*component);
	components.clear();
}

void GameObject::AddComponent(Component* c)
{
	//TODO:Check si ia en tenim un d'aquet tipus??
	c->SetGameObject(*this);	
	components.emplace_back(c);
}

/*void GameObject::Start()
{
	std::vector<Component*>::iterator it = components.begin();
	for (it; it != components.end(); it++)
		(*it)->Start();
}*/

void GameObject::Update(float dt)
{
	std::vector<Component*>::iterator it = components.begin();
	for (it; it != components.end(); it++)
		(*it)->Update(dt);
}
