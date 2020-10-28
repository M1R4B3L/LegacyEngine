#include "Meshes.h"
#include "GL/glew.h"
#include "scene.h"
#include "cimport.h"
#include "postprocess.h"

std::vector<Mesh> LoadFileMeshes(const char* file_path)
{
	std::vector<Mesh> LoadedMeshes;
	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes()) {
		for (int i = 0; i < scene->mNumMeshes; ++i) {
			Mesh ourMesh;
			//load vertices
			ourMesh.num_vertex = scene->mMeshes[i]->mNumVertices;
			ourMesh.vertex = new float[ourMesh.num_vertex * 3];
			memcpy(ourMesh.vertex, scene->mMeshes[i]->mVertices, sizeof(float) * ourMesh.num_vertex * 3);
			LOG("New Mesh with %d vertices", ourMesh.num_vertex);
			//load indices
			if (scene->mMeshes[i]->HasFaces()) {
				ourMesh.num_indices = scene->mMeshes[i]->mNumFaces * 3;
				ourMesh.index = new uint[ourMesh.num_indices];
				for (int j = 0; j < scene->mMeshes[i]->mNumFaces; ++j) {
					if (scene->mMeshes[i]->mFaces[j].mNumIndices != 3) {
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
						memcpy(&ourMesh.index[j * 3], scene->mMeshes[i]->mFaces[j].mIndices, 3 * sizeof(uint));
				}
			}
			/*
			if (scene->mMeshes[i]->HasNormals()) {
				ourMesh.num_vertex = scene->mMeshes[i]->mNumVertices;
				ourMesh.normals = new float[ourMesh.num_vertex * 3];
				memcpy(ourMesh.normals, scene->mMeshes[i]->mNormals, sizeof(float) * ourMesh.num_vertex * 3);
			}
			if (scene->mMeshes[i]->HasTextureCoords(0)) {
				ourMesh.num_vertex = scene->mMeshes[i]->mNumVertices;
				ourMesh.texturecoords = new float[ourMesh.num_vertex * 2];

				for (unsigned int i = 0; i < ourMesh.num_vertex; i++)
				{
					ourMesh.texturecoords[i * 2] = scene->mMeshes[i]->mTextureCoords[0][i].x;
					ourMesh.texturecoords[i * 2 + 1] = scene->mMeshes[i]->mTextureCoords[0][i].y;
				}
			}
			*/
			//generate the glbuffers
			setupmesh(ourMesh);
			LoadedMeshes.push_back(ourMesh);
		}
	}
	else
		LOG("Error loading scene %s", file_path);

	return LoadedMeshes;
}

void setupmesh(Mesh& mesh)
{
	glGenVertexArrays(1, &mesh.VAO);
	glGenBuffers(1, &mesh.VBO);
	glGenBuffers(1, &mesh.EBO);

	glBindVertexArray(mesh.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);

	glBufferData(GL_ARRAY_BUFFER, mesh.num_vertex * sizeof(float) * 3, &mesh.vertex[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.num_indices * sizeof(unsigned int) ,&mesh.index[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	/*
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	*/

	glBindVertexArray(0);
}
