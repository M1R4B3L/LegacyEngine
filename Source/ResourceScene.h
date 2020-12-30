#pragma once
#include "Resource.h"
#include <string>
class GameObject;

class ResourceScene : public Resource
{
public:
	ResourceScene(unsigned int id);
	~ResourceScene();
	bool LoadInMemory() override;
	bool SaveResource() override;

public:
	GameObject* root = nullptr;
	std::string sceneName;
};