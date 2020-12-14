#pragma once
#include "Resource.h"

class ResourceTexture : public Resource
{
public:
	ResourceTexture(unsigned int id);
	~ResourceTexture();
	bool LoadInMemory() override;
public:
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int gpuID = 0;
};