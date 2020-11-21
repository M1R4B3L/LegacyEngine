#include "ComponentMaterial.h"

ComponentMaterial::ComponentMaterial(unsigned int texture): Component(ComponentType::Material), textureID(texture) {}

ComponentMaterial::~ComponentMaterial() {}

const unsigned int ComponentMaterial::GetID() const
{
	return textureID;
}
