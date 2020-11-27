#ifndef __ModuleSceneIntro_H__
#define __ModuleSceneIntro_H__

#include "Module.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2

class GameObject;

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

	const GameObject* GetRootObject() const;

	void SetGameObjectSelected(GameObject* gameObject);

private:

	GameObject* root = nullptr;
	//GameObject * selected;
};
#endif // __ModuleSceneIntro_H__