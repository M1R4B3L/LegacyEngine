#ifndef __Components_H__
#define __Components_H__

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