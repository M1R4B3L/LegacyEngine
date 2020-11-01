#include "Importer.h"
#include "GL/glew.h"
#include "scene.h"
#include "cimport.h"
#include "postprocess.h"
#include "ComponentMesh.h"
#include "GameObjects.h"
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
				ourMesh.num_normals = ourMesh.num_vertex * 3;
				ourMesh.normals = new float[ourMesh.num_vertex * 3];
				memcpy(ourMesh.normals, scene->mMeshes[i]->mNormals, sizeof(float) * ourMesh.num_vertex * 3);
			}

			if (scene->mMeshes[i]->HasTextureCoords(0)) {
				ourMesh.num_texcoords = ourMesh.num_vertex * 2;
				ourMesh.texturecoords = new float[ourMesh.num_vertex * 2];

				for (unsigned int j = 0; j < ourMesh.num_vertex; j++)
				{
					ourMesh.texturecoords[j * 2] = scene->mMeshes[i]->mTextureCoords[0][j].x;
					ourMesh.texturecoords[j * 2 + 1] = scene->mMeshes[i]->mTextureCoords[0][j].y;
				}
			}
			//Loading possible Diffuse texture files
			for (int k = 0; k < scene->mNumMaterials; ++k) {
				uint numTextures = scene->mMaterials[k]->GetTextureCount(aiTextureType_DIFFUSE);
				for (int j = 0; j < numTextures; ++j) {
					aiString path;
					scene->mMaterials[k]->GetTexture(aiTextureType_DIFFUSE, 0, &path);
					//Problem: Storing multiple textures (I can use a vector)
					ourMesh.difuseTexture = Importer::Textures::GenerateTexture(path.C_Str());
				}
			}
			//ourMesh.difuseTexture = 234293;

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
	glBindVertexArray(mesh.VAO);
	
	glGenBuffers(1, &mesh.VBO);
	glGenBuffers(1, &mesh.EBO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);

	glBufferData(GL_ARRAY_BUFFER, mesh.num_vertex * sizeof(float) * 3, mesh.vertex, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.num_indices * sizeof(unsigned int) ,mesh.index, GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	
	if (mesh.num_normals > 0) {
		uint NormalsBuffer;
		glGenBuffers(1, &NormalsBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, NormalsBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.num_normals * 2, mesh.normals, GL_STATIC_DRAW);

		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
	}
	
	if (mesh.num_texcoords > 0) {
		uint TexCoordBuffer;
		glGenBuffers(1, &TexCoordBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, TexCoordBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.num_texcoords * 2, mesh.texturecoords, GL_STATIC_DRAW);

		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
	}
	

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
	//"C:/Users/User/Desktop/LegacyEngine/Baker_house/Baker_house.png"
	if (!ilLoadImage(imagePath)) {
		LOG("Error loading the image");
		ILenum Error;
		while ((Error = ilGetError()) != IL_NO_ERROR) {
			LOG("Importing image error %d: %s/n", Error, iluErrorString(Error));
		}
		return 0;
	}
	//ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	return ImageName;
}

// Without using ilutGlBindTexImage
uint Importer::Textures::GenerateTexture(const char* imagePath)
{
	uint ImageName;
	ilGenImages(1, &ImageName);
	ilBindImage(ImageName);
	//"C:/Users/User/Desktop/LegacyEngine/Baker_house/Baker_house.png"
	if (!ilLoadImage(imagePath)) {
		LOG("Error loading the image");
		ILenum Error;
		while ((Error = ilGetError()) != IL_NO_ERROR) {
			LOG("Importing image error %d: %s/n", Error, iluErrorString(Error));
		}
		return 0;
	}
	
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


uint Importer::Textures::checkerImage() {

	GLubyte checkerImage[64][64][4];
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}

	uint textureID = 234293;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);
	
	return textureID;
}