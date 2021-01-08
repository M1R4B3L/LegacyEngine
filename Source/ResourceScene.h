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
	bool SaveResourceAt(const char* path) override;
	bool LoadResourceFrom(const char* path)override;

public:
	GameObject* root = nullptr;
};