#pragma once
#include "Resource.h"
#include <string>
class GameObject;

class ResourceScene : public Resource
{
public:
	ResourceScene(unsigned int id);
	ResourceScene(unsigned int id, GameObject* root);
	~ResourceScene();
	bool LoadInMemory() override;
	bool SaveResource() override;

public:
	GameObject* root = nullptr;
};