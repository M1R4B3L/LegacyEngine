#include "ComponentMesh.h"
#include "Application.h"
#include "Application.h"
#include "ModuleResources.h"


ComponentMesh::ComponentMesh(): Component(ComponentType::Mesh), resourceID(0), activeMesh(true){}

ComponentMesh::ComponentMesh(unsigned int uid) : Component(ComponentType::Mesh), resourceID(uid), activeMesh(true) { App->resources->RequestResource(resourceID, Resource::Type::MESH); }

ComponentMesh::~ComponentMesh() 
{
	if (resourceID != 0)
		App->resources->UnrequestResource(resourceID);
}

const unsigned int ComponentMesh::GetID() const
{
	return resourceID;
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
		App->resources->RequestResource(resourceID, Resource::Type::MESH);
}
