#include "Importer.h"
#include "GL/glew.h"
#include "scene.h"
#include "cimport.h"
#include "postprocess.h"
#include "il.h"
#include "ilu.h"
#include "ilut.h"

std::vector<Mesh> Importer::Meshes::Import(const char* file_path)
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
			
			if (scene->mMeshes[i]->HasNormals()) {
				ourMesh.normals = new float[ourMesh.num_vertex * 3];
				memcpy(ourMesh.normals, scene->mMeshes[i]->mNormals, sizeof(float) * ourMesh.num_vertex * 3);
			}

			if (scene->mMeshes[i]->HasTextureCoords(0)) {
				ourMesh.texturecoords = new float[ourMesh.num_vertex * 2];

				for (unsigned int j = 0; j < ourMesh.num_vertex; j++)
				{
					ourMesh.texturecoords[j * 2] = scene->mMeshes[i]->mTextureCoords[0][j].x;
					ourMesh.texturecoords[j * 2 + 1] = scene->mMeshes[i]->mTextureCoords[0][j].y;
				}
			}
			//Loading possible Diffuse texture files
			for (int i = 0; i < scene->mNumMaterials; ++i) {
				uint numTextures = scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE);
				for (int j = 0; j < numTextures; ++j) {
					aiString path;
					scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path);
					//Problem: Storing multiple textures (I can use a vector)
					ourMesh.difuseTexture = Importer::Textures::Import(path.C_Str());
				}
			}

			//generate the glbuffers
			Importer::Meshes::setupmesh(ourMesh);
			LoadedMeshes.push_back(ourMesh);
		}
	}
	else
		LOG("Warning loading Meshes in file %s: File has no meshes or textures to load", file_path);

	return LoadedMeshes;
}

void Importer::Meshes::setupmesh(Mesh& mesh)
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
	
	//Problem: what happens if the model doesn't have textures or normals ??
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)(sizeof(float)*3));

	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)(sizeof(float) * 6));
	

	glBindVertexArray(0);
}

void Importer::Textures::Init() //Problem: Where do we call these function. Do we create a modul Importer?
{
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
}

uint Importer::Textures::Import(const char* imagePath) {
	//ILuint ImageName to keep track of your image and to use o modify it bind it
	//If i no longer need the image bound with a name i can call ilDeleteImages()
	uint ImageName;
	ilGenImages(1, &ImageName);
	ilBindImage(ImageName);
	if (ilLoadImage(imagePath)) {
		LOG("Error loading the image")
		ILenum Error;
		while ((Error = ilGetError()) != IL_NO_ERROR) {
			LOG("Importing image error %d: %s/n", Error, iluErrorString(Error));
		}
		return 0;
	}
	return ImageName;
}

/* Without using ilutGlBindTexImage
uint Importer::Textures::GenerateTexture(uint& ImageName)
{
	if (ImageName != 0)
	{
		uint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//load and generate the texture
		//Use devil here to get all the texture data
		ilBindImage(ImageName);
		ILubyte* data = ilGetData();
		ILuint width = ilGetInteger(IL_IMAGE_WIDTH);
		ILuint height = ilGetInteger(IL_IMAGE_HEIGHT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);

		//glBindTexture(GL_TEXTURE_2D, 0); Cal ??
		//
		//Free the image data after 
		ilDeleteImages(1, &ImageName);
		return texture;
	}
	else {
		LOG("Can't generate texture because image data null");
		return 0;
	}
}
*/