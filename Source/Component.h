#ifndef __Components_H__
#define __Components_H__
#include "parson.h"

class GameObject;
enum class ComponentType {
	Mesh,
	Transform,
	Material,
	Camera,
	Unknown
};

class Component
{
public:
	Component(ComponentType iType);
	Component(GameObject* go, ComponentType iType);
	virtual	~Component() {}

	virtual bool IsActive() const;
	virtual void SetActive(bool active);

	virtual void Start() {}
	virtual void Update(float dt) {}
	virtual void Save(JSON_Array* componentsArry) const {}
	virtual void Load(JSON_Object* componentObj) {}

	GameObject& GetGameObject();
	const GameObject& GetGameObject() const;
	const ComponentType GetType() const;
	void SetGameObject(GameObject& go);


private:
	GameObject* gameObject;
	ComponentType type;
	bool activeComponent = true;
};
#endif