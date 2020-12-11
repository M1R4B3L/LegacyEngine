#include "ComponentMesh.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "Application.h"
#include "Dependencies/MathGeolib/MathGeoLib.h"


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
