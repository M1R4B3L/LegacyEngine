#pragma once
#include "Resource.h"

class ResourceModel : public Resource
{
public:
	ResourceModel(unsigned int id);
	~ResourceModel();
	bool LoadInMemory() override;
public:
	//Model data
};