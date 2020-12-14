#pragma once
#include <string>
#include "parson.h"

class Resource
{
public:
	enum Type {
		TEXTURE,
		MESH,
		SCENE,
		MODEL,
		//shader
		unknown
	};

	Resource(unsigned int uid, Resource::Type type);
	virtual ~Resource() {}
	Resource::Type GetType() const;
	unsigned int GetUID() const;
	unsigned int GetReferenceCount() const;
	void DecreaseReferenceCount();
	virtual bool LoadInMemory() { return true; }

protected:
	unsigned int uid = 0;
	Type type = Type::unknown;
	unsigned int referenceCount = 0;
};