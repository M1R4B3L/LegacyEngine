#include "Application.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "Importer.h"
#include "scene.h"
#include "cimport.h"
#include "postprocess.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"
#include "GameObjects.h"
#include "il.h"
#include "ilu.h"
#include "ilut.h"

void Importer::Meshes::ImportFbx(const char* fbxPath)
{
	//TODO: return a bool if any error?
	LOG("Importing Mesh");
	const aiScene* scene = aiImportFile(fbxPath, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		ParseFbxNode(scene->mRootNode, scene, fbxPath);
	}
	else
		LOG("Error opening file: %s ", fbxPath);
	//TODO: Destroy the FBX
}

void Importer::Meshes::ParseFbxNode(aiNode * node, const aiScene * scene, const char* fbxPath, GameObject*parentGo) 
{

	if (strcmp(node->mName.C_Str(), "RootNode") == 0) {

		std::string tmp = fbxPath;
		std::string name = tmp.substr(tmp.find_last_of("/")+1);
		node->mName = name;
	}

	GameObject* go = nullptr;
	
	aiVector3D translation, scaling;

	aiQuaternion aiRotation;

	node->mTransformation.Decompose(scaling, aiRotation, translation);

	float3 pos(translation.x, translation.y, translation.z);
	float3 scale(scaling.x, scaling.y, scaling.z);
	Quat rot(aiRotation.x, aiRotation.y, aiRotation.z, aiRotation.w);
	Quat	tempRotation;
	float3 scale1 = { 1, 1, 1 };

	while (strstr(node->mName.C_Str(), "_$AssimpFbx$_") != nullptr && node->mNumChildren == 1)
	{
		node = node->mChildren[0];
		node->mTransformation.Decompose(scaling, aiRotation, translation);

		tempRotation = Quat(aiRotation.x, aiRotation.y, aiRotation.z, aiRotation.w);

		pos.x += translation.x;
		pos.y += translation.y;
		pos.z += translation.z;

		scale.x *= scaling.x;
		scale.y *= scaling.y;
		scale.z *= scaling.z;

		rot = rot * tempRotation;
		
	}
	
	go = App->scene->CreateGameObject(node->mName.C_Str(), parentGo);

	if (!go->HasComponent(ComponentType::Transform))
	{
		ComponentTransform* transformComponent = new ComponentTransform(go, pos, scale, rot);
		transformComponent->SetLocalTransform(pos, scale, rot);

		//LOG("Draw: %s", this->name.c_str());
	}

	if (scene->mNumCameras > 0)
	{
		//Componen
	}

	if (node->mNumMeshes > 0) {
		for (int i = 0; i < node->mNumMeshes; ++i)
		{
			//Create a game object with its name and the parent of the go is this node
			//TODO: use the name of the node or the name of the mesh ?

			LOG("%s", node->mName.C_Str());

			//Create and attach all the components this node will need (using the scene check if it has a texture for adding a component material and add the transform and mesh component)
			aiMesh* nodeMesh = scene->mMeshes[node->mMeshes[i]];

			//SaveMesh(nodeMesh);

			Mesh ourMesh;
			ourMesh.numTexcoords = 0;
			//Loading Vertex Positions
			ourMesh.numVertices = nodeMesh->mNumVertices;
			ourMesh.vertexData = new float[ourMesh.numVertices * 3];
			memcpy(ourMesh.vertexData, nodeMesh->mVertices, sizeof(float) * ourMesh.numVertices * 3);
			LOG("New Mesh with %d vertices", ourMesh.numVertices);
			//load indices
			if (nodeMesh->HasFaces()) {
				ourMesh.numIndices = nodeMesh->mNumFaces * 3;
				ourMesh.indexData = new uint[ourMesh.numIndices];
				for (int j = 0; j < nodeMesh->mNumFaces; ++j) {
					if (nodeMesh->mFaces[j].mNumIndices != 3) {
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
						memcpy(&ourMesh.indexData[j * 3], nodeMesh->mFaces[j].mIndices, 3 * sizeof(uint));
				}
			}
			//Loading Normals
			if (nodeMesh->HasNormals()) {
				ourMesh.numNormals = ourMesh.numVertices * 3;
				ourMesh.normalsData = new float[ourMesh.numNormals];
				memcpy(ourMesh.normalsData, nodeMesh->mNormals, sizeof(float) * ourMesh.numNormals);
			}
			if (nodeMesh->HasTextureCoords(0)) {
				ourMesh.numTexcoords = ourMesh.numVertices * 2;
				ourMesh.texturecoordsData = new float[ourMesh.numTexcoords];

				for (int j = 0; j < ourMesh.numVertices; j++)
				{
					ourMesh.texturecoordsData[j * 2] = nodeMesh->mTextureCoords[0][j].x;
					ourMesh.texturecoordsData[j * 2 + 1] = nodeMesh->mTextureCoords[0][j].y;
				}
			}

			//TODO: can i add components to the game objects on their constructors ?
			LOG("SettingUp %s transform", go->GetName());


			ComponentMesh* meshComponent = new ComponentMesh(App->renderer3D->VAOFromMesh(ourMesh), ourMesh.numVertices, ourMesh.numIndices);
			go->AddComponent(meshComponent);

			//Adding Materials
			if (scene->HasMaterials())
			{
				aiMaterial* material = scene->mMaterials[nodeMesh->mMaterialIndex];
				if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) //TODO: Import more than one texture on a mesh (still not suported on the shader level)
				{
					aiString name;
					material->GetTexture(aiTextureType_DIFFUSE, 0, &name);

					std::string fileName = name.C_Str();
					fileName = fileName.substr(fileName.find_last_of("/""\\") + 1);
					std::string filePath = fbxPath;
					std::string newfilePath = filePath.substr(0,filePath.find_last_of("/") + 1);
					newfilePath = newfilePath + fileName;

					uint textureID = Textures::Import(newfilePath.c_str());

					if (textureID)
					{
						ILinfo ilInfo;
						iluGetImageInfo(&ilInfo);

						float2 size = { (float)ilInfo.Width, (float)ilInfo.Height };

						ComponentMaterial* materialComponent = new ComponentMaterial(textureID);
						go->AddComponent(materialComponent);
					}
				}

			}
		}
	}
	for (unsigned int j = 0; j < node->mNumChildren; j++)
	{
		//parse all the childs of this node calling this same function recursively
		ParseFbxNode(node->mChildren[j], scene, fbxPath, go);
	}

}

void Importer::Textures::Init() //Problem: Where do we call these function. Do we create a modul Importer?
{
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
}

unsigned int Importer::Textures::Import(const char* imagePath) {
	//ILuint ImageName to keep track of your image and to use o modify it bind it
	//If i no longer need the image bound with a name i can call ilDeleteImages()
	LOG("LoadingImage");
	uint ImageName;
	ilGenImages(1, &ImageName);
	ilBindImage(ImageName);

	if (!ilLoadImage(imagePath)) {
		LOG("Error loading the image");
		ILenum Error;
		while ((Error = ilGetError()) != IL_NO_ERROR) {
			LOG("Importing image error %d: %s", Error, iluErrorString(Error));
		}
		return 0;
	}

	/*if (ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE)) //TODO: Cal convertir les textures a RGBA
		LOG("Succesfully converted image to RGBA");*/

	uint texture = ilutGLBindTexImage();
	ilutGLBindMipmaps();
	ilDeleteImages(1, &ImageName);
	return texture;
}

/*// Without using ilutGlBindTexImage
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
}*/


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

void Importer::ImportTransform(aiNode* node, GameObject* gameObject)
{
	

}

bool Importer::ImportDroped(const char* absFilepath)
{
	std::string absPath = absFilepath;
	std::replace(absPath.begin(), absPath.end(), '\\', '/');
	std::string extension = absPath.substr(absPath.find_last_of(".") + 1);

	if (extension == "fbx" || extension == "FBX") 
	{
		Meshes::ImportFbx(absPath.c_str());
		return true;
	}
	else if (extension == "png" || extension == "tga") 
	{
		App->renderer3D->dropedTexture = Textures::Import(absPath.c_str());

		return true;
	}

	return false;
}

void SaveMesh(aiMesh* mesh)
{
	uint numVertices = mesh->mNumVertices;
	uint numIndices = 0;
	uint numTextureCoordinates = 0;
	uint numNormals = 0;
	
	if (mesh->HasFaces())
		numIndices = mesh->mNumFaces * 3;
	if (mesh->HasTextureCoords(0))
		uint numTextureCoordinates = numVertices * 2;
	if (mesh->HasNormals())
		numNormals = numVertices * 3;

	uint ranges[4] = { numIndices, numVertices, numTextureCoordinates, numNormals };
	uint size = sizeof(ranges) + sizeof(uint) * numIndices + sizeof(float) * numVertices * 3 + sizeof(float) * numTextureCoordinates + sizeof(float) * numNormals;

	char* fileBuffer = new char[size];
	char* cursor = fileBuffer;

	//save ranges
	uint bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);
	cursor += bytes;

	uint* udata = nullptr;
	//save indices
	if (numIndices) {
		uint * data = new uint[numIndices];
		for (int j = 0; j < mesh->mNumFaces; ++j) {
			if (mesh->mFaces[j].mNumIndices != 3) {
				LOG("WARNING, geometry face with != 3 indices!");
			}
			else
				memcpy(&udata[j * 3], mesh->mFaces[j].mIndices, 3 * sizeof(uint));
		}
		bytes = sizeof(uint) * numIndices;
		memcpy(cursor, udata, bytes);
		cursor += bytes;
		delete[] udata;
		udata = nullptr;
	}

	//save vertices
	bytes = sizeof(float) * numVertices * 3;
	memcpy(cursor, mesh->mVertices, bytes);
	cursor += bytes;

	//save textcoords
	float* fdata = nullptr;
	if (numTextureCoordinates) {

		fdata = new float[numTextureCoordinates];

		for (int j = 0; j < numVertices; j++)
		{
			fdata[j * 2] = mesh->mTextureCoords[0][j].x;
			fdata[j * 2 + 1] = mesh->mTextureCoords[0][j].y;
		}
		bytes = sizeof(float) * numTextureCoordinates;
		memcpy(cursor, fdata, bytes);
		cursor += bytes;
		delete[] fdata;
		fdata = nullptr;
	}

	//save normals
	if (numNormals) {
		bytes = sizeof(float) * numNormals;
		memcpy(cursor, mesh->mNormals, bytes);
		cursor += bytes;
	}
}

Mesh LoadMesh(char* buffer)
{
	Mesh ourMesh;
	char* cursor = buffer;

	uint ranges[4];
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);
	cursor += bytes;

	ourMesh.numIndices = ranges[0];
	ourMesh.numVertices = ranges[1];
	ourMesh.numTexcoords = ranges[2];
	ourMesh.numNormals = ranges[3];

	bytes = sizeof(uint) * ourMesh.numIndices;
	ourMesh.indexData = new uint[ourMesh.numIndices];
	memcpy(ourMesh.indexData, cursor, bytes);
	cursor += bytes;

	bytes = sizeof(float) * ourMesh.numVertices * 3;
	ourMesh.vertexData = new float[ourMesh.numVertices];
	memcpy(ourMesh.vertexData, cursor, bytes);
	cursor += bytes;

	bytes = sizeof(float) * ourMesh.numTexcoords;
	ourMesh.texturecoordsData = new float[ourMesh.numTexcoords];
	memcpy(ourMesh.texturecoordsData, cursor, bytes);
	cursor += bytes;

	bytes = sizeof(float) * ourMesh.numNormals;
	ourMesh.texturecoordsData = new float[ourMesh.numNormals];
	memcpy(ourMesh.normalsData, cursor, bytes);
	cursor += bytes;

	return ourMesh;
	//App->renderer3D->VAOFromMesh(ourMesh);
}
