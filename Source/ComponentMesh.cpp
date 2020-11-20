#include "ComponentMesh.h"
#include "Application.h"
#include "ModuleRenderer3D.h"


ComponentMesh::ComponentMesh(std::vector<Mesh> imported_meshes) : Component(nullptr,ComponentType::Mesh,true), meshes(imported_meshes) {}

void ComponentMesh::Update(float dt)
{
	std::vector<Mesh>::iterator it;
	for (it = meshes.begin(); it != meshes.end(); it++) {
		App->renderer3D->Draw((*it));
	}
}
