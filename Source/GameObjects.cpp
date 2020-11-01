#include "GameObjects.h"
#include "Components.h"

GameObject::GameObject() : m_Name("No name")
{
}

GameObject::GameObject(const std::string&& name) : m_Name(name) {}

GameObject::~GameObject()
{
	std::vector<Component*>::iterator component = m_Components.begin();
	for (component;component!= m_Components.end();component++)	
		delete (*component);
	m_Components.clear();
}

void GameObject::AddComponent(Component* c)
{
	c->SetGameObject(*this);	
	m_Components.emplace_back(c);
}

void GameObject::Start()
{
	std::vector<Component*>::iterator it = m_Components.begin();
	for (it; it != m_Components.end(); it++)
		(*it)->Start();
}

void GameObject::Update(float dt)
{
	std::vector<Component*>::iterator it = m_Components.begin();
	for (it; it != m_Components.end(); it++)
		(*it)->Update(dt);
}
