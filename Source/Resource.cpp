#include "Resource.h"

Resource::Resource(unsigned int uid, Resource::Type type) : uid(uid), type(type){}

Resource::Type Resource::GetType() const
{
	return type;
}

unsigned int Resource::GetUID() const
{
	return uid;
}

unsigned int Resource::GetReferenceCount() const
{
	return referenceCount;
}

void Resource::DecreaseReferenceCount()
{
	if(referenceCount > 0)
		referenceCount -= 1;
}

void Resource::IncreaseReferenceCount()
{
	referenceCount += 1;
}

