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
	Component(GameObject* go, ComponentType iType, bool iActive);
	virtual	~Component() {}

	virtual void Start() {}
	virtual void Update(float dt) {}
	virtual void Enable(){}
	virtual void Disable(){}
	GameObject& GetGameObject();
	const GameObject& GetGameObject() const;
	const ComponentType GetType() const;
	void SetGameObject(GameObject& go);
private:
	GameObject* gameObject;
	ComponentType type;
	bool active;
};
#endif