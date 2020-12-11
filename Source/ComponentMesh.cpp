#include "ComponentMesh.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "Application.h"
#include "Dependencies/MathGeolib/MathGeoLib.h"
#include "ModuleFileSystem.h"


ComponentMesh::ComponentMesh(): Component(ComponentType::Mesh),VAO(0),numIndices(0),numVertices(0)
{
}

ComponentMesh::ComponentMesh(unsigned int iVAO, unsigned int vertices, unsigned int indices,AABB _aabb) : Component(ComponentType::Mesh), VAO(iVAO), numIndices(indices), numVertices(vertices), aabb(_aabb){}

ComponentMesh::~ComponentMesh() { App->renderer3D->DeleteBuffer(&VAO); }

const unsigned int ComponentMesh::GetVAO() const
{
	return VAO;
}

const unsigned int ComponentMesh::GetNumIndices() const
{
	return numIndices;
}

const unsigned int ComponentMesh::GetNumVertex() const
{
	return numVertices;
}

const AABB& ComponentMesh::GetAABB() const
{
	return aabb;
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
	//TODO:!!!! SAVE DELS RESOURCES
	//json_object_set_number(jsonCM, "ResourceUID", ResourceUID);
}

void ComponentMesh::Load(JSON_Object* componentObj)
{
	//TODO:!!!! LOAD DELS RESOURCES
	//Resource/filesystem->Load(json_object_get_number(componentObj, "ResourceUID"));
}
