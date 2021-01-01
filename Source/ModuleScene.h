#ifndef __ModuleSceneIntro_H__
#define __ModuleSceneIntro_H__

#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include "Dependencies/MathGeoLib/Algorithm/Random/LCG.h"

class GameObject;
class ResourceScene;
class ComponentTransform;

class ModuleScene : public Module
{
public:
	ModuleScene(bool startEnabled = true);
	~ModuleScene();

	bool Start();
	update_status Update(float dt);	
	bool CleanUp();

	void UpdateAllGameObjects(float dt);
	void DrawAllGameObjects();
	GameObject* CreateGameObject(const char* name, GameObject* parent = nullptr);
	GameObject* CreateTransformGameObject(const char* name, GameObject* parent, ComponentTransform* transform = nullptr);
	void SetParent(GameObject* gameObject, GameObject* newParent);

	GameObject* GetRootObject() const;
	GameObject* GetSelectedObject() const;

	void SetGameObjectSelected(GameObject* gameObject);
	void SetGameObjectUnselected();
	
	void DeleteGameObject(GameObject* gameObject, bool root = true);

	void SaveScene();
	bool LoadScene(unsigned int ID);

	unsigned int GetRandomInt();
	GameObject* FindGOFromUID(GameObject* currGO, unsigned int UID);

	void CreateCamera(const char* name);

	unsigned int GetResourceId() const;
	const ResourceScene* GetResource() const;
	const char* GetSceneName() const;
	bool ChangeResource(unsigned int id);

private:

	GameObject* root = nullptr;
	GameObject* selectedObject = nullptr;
	unsigned int resourceID = 0;
	ResourceScene* resource = nullptr;
	std::string sceneName;

	LCG random;
};
#endif // __ModuleSceneIntro_H__