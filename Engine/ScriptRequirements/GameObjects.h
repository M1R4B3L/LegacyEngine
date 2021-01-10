#ifndef __GameObjects_H__
#define __GameObjects_H__

#include <vector>
#include <string>
#include "MathGeolib/MathGeoLib.h"
#include "parson.h"

class Component;
enum class ComponentType;

class __declspec(dllexport) GameObject
{
public:
	GameObject();
	GameObject(GameObject* parent, unsigned int UID, const char* iName = "noName");
	GameObject(GameObject* parent, const char* iName = "noName");
	~GameObject();

	// Add a new component to this game object
	void AddComponent(Component* c);
	bool RemoveComponent(Component* c);

	void Start();
	void Update(float dt);
	void Draw();


	GameObject* GetParent() const;
	void SetParent(GameObject* newParent);
	bool IsActive() const;

	Component* GetComponent(ComponentType type);
	bool HasComponent(ComponentType type) const;

	const char* GetName() const;
	void SetName(char* newName);

	void GenerateAABB();

	void Save(JSON_Array*GOarry) const;
	void Load();

	unsigned int GetID()const;
	AABB GetAABB();

private:
	GameObject* parent = nullptr;
	std::string name;

	AABB aabb;
	OBB obb;
	unsigned int uid;

	//TODO: Change children to private
public:
	bool showAABB = true;
	bool showOBB = true;
	bool activeGameObject = true;
	std::vector<Component*> components;
	std::vector<GameObject*> children;

};


#endif //__GameObjects_H__