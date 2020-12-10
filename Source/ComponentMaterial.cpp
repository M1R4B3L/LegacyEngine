#include "ComponentMaterial.h"
#include "Application.h"
#include "ModuleRenderer3D.h"

ComponentMaterial::ComponentMaterial(unsigned int texture): Component(ComponentType::Material), textureID(texture) {}

ComponentMaterial::~ComponentMaterial() { App->renderer3D->DeleteTexture(&textureID); }

const unsigned int ComponentMaterial::GetID() const
{
	return textureID;
}
