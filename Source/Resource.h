#pragma once
#include <string>
#include "parson.h"

class Resource
{
public:
	enum class Type: unsigned int {
		TEXTURE,
		MESH,
		SCENE,
		MODEL,
		SCRIPT,
		//shader
		unknown
	};

	Resource(unsigned int uid, Resource::Type type);
	virtual ~Resource() {}
	Resource::Type GetType() const;
	unsigned int GetUID() const;
	unsigned int GetReferenceCount() const;
	void DecreaseReferenceCount();
	void IncreaseReferenceCount();
	virtual bool LoadInMemory() { return true; }
	virtual bool SaveResource() { return true; }

protected:
	unsigned int uid = 0;
	Type type = Type::unknown;
private:
	unsigned int referenceCount = 0;
};