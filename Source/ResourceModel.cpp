#include "ResourceModel.h"

ResourceModel::ResourceModel(unsigned int id):Resource(id, Resource::Type::MODEL){}

ResourceModel::~ResourceModel(){}

bool ResourceModel::LoadInMemory()
{
	return false;
}