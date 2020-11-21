#ifndef __ModuleSceneIntro_H__
#define __ModuleSceneIntro_H__

#include "Module.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2

class GameObject;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void UpdateAllGameObjects(float dt);
	void DrawAllGameObjects();
	GameObject* CreateGameObject(GameObject*parent, const char* name);

	const GameObject* GetRootObject() const;

private:

	GameObject* root = nullptr;
	//GameObject * selected;
};
#endif // __ModuleSceneIntro_H__