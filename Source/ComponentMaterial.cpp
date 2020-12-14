#include "ComponentMaterial.h"
#include "Application.h"
#include "ModuleResources.h"


ComponentMaterial::ComponentMaterial() : Component(ComponentType::Material), resourceID(0)
{
}

ComponentMaterial::ComponentMaterial(unsigned int texture): Component(ComponentType::Material), resourceID(texture)
{
	App->resources->RequestResource(resourceID, Resource::Type::TEXTURE);
}

ComponentMaterial::~ComponentMaterial() {
	if (resourceID != 0)
		App->resources->UnrequestResource(resourceID);
}

const unsigned int ComponentMaterial::GetID() const
{
	return resourceID;
}

void ComponentMaterial::Save(JSON_Array* componentsArry) const
{
	json_array_append_value(componentsArry, json_value_init_object());
	JSON_Object* jsonCMa = json_array_get_object(componentsArry, json_array_get_count(componentsArry) - 1);
	json_object_set_number(jsonCMa, "Type", (int)ComponentType::Material);
	json_object_set_number(jsonCMa, "ResourceUID", resourceID);
}

void ComponentMaterial::Load(JSON_Object* componentObj)
{
	resourceID = json_object_get_number(componentObj, "ResourceUID");
	if (resourceID != 0)
		App->resources->RequestResource(resourceID, Resource::Type::TEXTURE);
}
