#include "ComponentMesh.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "GL/glew.h"
#include "Application.h"

ComponentMesh::ComponentMesh(unsigned int iVAO, unsigned int vertices, unsigned int indices) : Component(ComponentType::Mesh), VAO(iVAO), numIndices(indices), numVertices(vertices) {}

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

bool ComponentMesh::IsActive() const
{
	return activeMesh;
}

void ComponentMesh::SetActive(bool active)
{
	activeMesh = active;
}
