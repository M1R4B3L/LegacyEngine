#ifndef __GameObjects_H__
#define __GameObjects_H__

#include <vector>
#include <string>
#include "Dependencies/MathGeolib/MathGeoLib.h"
#include "parson.h"

class Component;
enum class ComponentType;

class GameObject
{
public:
	GameObject();
	GameObject(GameObject* parent, unsigned int UID, const char* iName = "noName", float3 transf = float3::zero, float3 scale = float3::one, Quat rot = Quat::identity);
	GameObject(GameObject* parent, const char* iName = "noName",float3 transf = float3::zero,float3 scale = float3::one, Quat rot = Quat::identity);
	~GameObject();

	// Add a new component to this game object
	void AddComponent(Component* c);
	bool RemoveComponent(Component* c);

	//void Start();
	void Update(float dt);
	void Draw();


	GameObject* GetParent() const;
	void SetParent(GameObject* newParent);
	bool IsActive() const;

	const Component* GetComponent(ComponentType type) const;
	bool HasComponent(ComponentType type) const;
	std::vector<GameObject*> GetChildren() const;

	const char* GetName() const;
	void SetName(char* newName);

	void GenerateAABB();

	void Save(JSON_Array*GOarry) const;
	void Load();

	unsigned int GetID()const;

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