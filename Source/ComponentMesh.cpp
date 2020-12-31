#include "ComponentMesh.h"
#include "Application.h"
#include "ModuleResources.h"
#include "ResourceMesh.h"


ComponentMesh::ComponentMesh(): Component(ComponentType::Mesh), resourceID(0), resource(nullptr), activeMesh(true){}

ComponentMesh::ComponentMesh(unsigned int uid) : Component(ComponentType::Mesh), resourceID(uid), resource((ResourceMesh*)App->resources->RequestResource(resourceID, Resource::Type::MESH)), activeMesh(true) {}

ComponentMesh::~ComponentMesh() 
{
	if (resourceID != 0)
		App->resources->UnrequestResource(resourceID);
}

const unsigned int ComponentMesh::GetID() const
{
	return resourceID;
}

const ResourceMesh* ComponentMesh::GetResource() const
{
	return resource;
}

bool ComponentMesh::ChangeResource(unsigned int id)
{
	ResourceMesh* newResource = (ResourceMesh*)App->resources->RequestResource(id, Resource::Type::MESH);
	if (newResource == nullptr)
		return false;

	App->resources->UnrequestResource(resourceID);
	resource = newResource;
	resourceID = id;

	return true;
}

bool ComponentMesh::IsActive() const
{
	return activeMesh;
}

void ComponentMesh::SetActive(bool active)
{
	activeMesh = active;
}

void ComponentMesh::Save(JSON_Array* componentsArry) const
{
	json_array_append_value(componentsArry, json_value_init_object());
	JSON_Object* jsonCM = json_array_get_object(componentsArry, json_array_get_count(componentsArry) - 1);
	json_object_set_number(jsonCM, "Type", (int)ComponentType::Mesh);
	json_object_set_number(jsonCM, "ResourceUID", resourceID);
	json_object_set_boolean(jsonCM, "Active", activeMesh);
}

void ComponentMesh::Load(JSON_Object* componentObj)
{
	activeMesh = json_object_get_boolean(componentObj, "Active");
	resourceID = json_object_get_number(componentObj, "ResourceUID");
	if(resourceID != 0)
		resource = (ResourceMesh*)App->resources->RequestResource(resourceID, Resource::Type::MESH);
}
