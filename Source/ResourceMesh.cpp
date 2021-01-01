#include "ResourceMesh.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "Importer.h"
#include "ModuleRenderer3D.h"

ResourceMesh::ResourceMesh(unsigned int id) : Resource(id, Resource::Type::MESH)
{
}

ResourceMesh::~ResourceMesh()
{
	//TODO: //glDisableMesh()
	App->renderer3D->DeleteMeshBuffers(VAO, VBO, EBO, textureBuffer);
}

bool ResourceMesh::LoadInMemory()
{
	char* fileBuffer = nullptr;
	std::string path = MESHES_PATH;
	path += std::to_string(uid);
	App->fileSystem->Load(path.c_str(), &fileBuffer);
	Mesh ourMesh;
	Importer::Meshes::LoadMesh(fileBuffer, &ourMesh);
	delete[] fileBuffer;
	fileBuffer = nullptr;
	numVertices = ourMesh.numVertices;
	numIndices = ourMesh.numIndices;
	App->renderer3D->GenMeshBuffers(&ourMesh);
	VAO = ourMesh.VAO;
	VBO = ourMesh.VBO;
	EBO = ourMesh.EBO;
	textureBuffer = ourMesh.textureBuffer;
	ourMesh.aabb.SetNegativeInfinity();
	ourMesh.aabb.Enclose((float3*)ourMesh.vertexData, ourMesh.numVertices);
	aabb = ourMesh.aabb;
	type = Resource::Type::MESH;
	return true;
}