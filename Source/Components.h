#ifndef __Components_H__
#define __Components_H__

class GameObject;
enum class ComponentType {
	Mesh,
	Transform,
	Material,
	Unknown
};

class Component
{
public:
	Component();
	virtual	~Component() {}

	virtual void Start() {}
	virtual void Update(float dt) {}
	virtual void Enable(){}
	virtual void Disable(){}
	GameObject* Owner;
	const GameObject& GetGameObject() const;	
	GameObject& GetGameObject();
	void SetGameObject(GameObject& go);
private:
	GameObject* m_GameObject;
	ComponentType type;
	bool active;
};

//	2D	position:	just	x,y	coordinates
class TransformComponent : public Component
{
public:
	float	x, y;
};

class MaterialsComponent : public Component 
{

};

#endif