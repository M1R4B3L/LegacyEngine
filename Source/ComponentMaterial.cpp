#include "ComponentMaterial.h"
#include "Application.h"
#include "Resource.h"
#include "ResourceTexture.h"
#include "ModuleResources.h"


ComponentMaterial::ComponentMaterial() : Component(ComponentType::Material), resourceID(0), resource(nullptr){}

ComponentMaterial::ComponentMaterial(unsigned int texture): Component(ComponentType::Material), resourceID(texture), resource((ResourceTexture*)App->resources->RequestResource(resourceID, Resource::Type::TEXTURE)){}

ComponentMaterial::ComponentMaterial(ResourceTexture* inputResource): Component(ComponentType::Material), resourceID(inputResource->GetUID()), resource(inputResource) {}

ComponentMaterial::~ComponentMaterial() {
	if (resourceID != 0)
		App->resources->UnrequestResource(resourceID);
}

const unsigned int ComponentMaterial::GetID() const
{
	return resourceID;
}

const ResourceTexture* ComponentMaterial::GetResource() const
{
	return resource;
}

bool ComponentMaterial::ChangeResource(unsigned int id)
{
	ResourceTexture* newResource = (ResourceTexture*)App->resources->RequestResource(id, Resource::Type::TEXTURE);
	if (newResource == nullptr)
		return false;

	App->resources->UnrequestResource(resourceID);
	resource = newResource;
	resourceID = id;

	return true;
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
	ChangeResource(json_object_get_number(componentObj, "ResourceUID"));
}