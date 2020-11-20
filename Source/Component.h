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
	//Component();
	Component(GameObject* go, ComponentType iType, bool iActive);
	virtual	~Component() {}

	virtual void Start() {}
	virtual void Update(float dt) {}
	virtual void Enable(){}
	virtual void Disable(){}
	const GameObject& GetGameObject() const;	
	GameObject& GetGameObject();
	void SetGameObject(GameObject& go);
private:
	GameObject* gameObject;
	ComponentType type;
	bool active;
};
#endif