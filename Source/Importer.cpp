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
#include "ComponentCamera.h"
#include "GameObjects.h"
#include "il.h"
#include "ilu.h"
#include "ilut.h"
#include "ModuleResources.h"
#include "ModuleFileSystem.h"
#include "Globals.h"
#include "Resource.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "ResourceScene.h"


bool Importer::ImportDroped(const char* absFilepath)
{
	//Todo: check that we are not importing a directory!!!!
	std::string normalPath(App->fileSystem->NormalizePath(absFilepath));
	std::string absPath;
	std::string extension;
	std::string fileName;
	App->fileSystem->SplitFilePath(normalPath.c_str(), &absPath, &fileName, &extension);
	std::string relativePath;

	if (extension == "fbx" || extension == "FBX")
	{
		//TODO: if file already exists?
		/*std::string modelMeta = ASSETS_MODELS; modelMeta += fileName + ".meta";
		if (App->fileSystem->FileExists(modelMeta.c_str())) {
			LOG("File %s already imported", absFilepath);
			return true;
		}*/
		App->fileSystem->DuplicateFile(normalPath.c_str(), ASSETS_MODELS, relativePath);
		App->resources->ImportFile(normalPath.c_str(), Resource::Type::MODEL);
		return true;
	}
	else if (extension == "png" || extension == "tga")
	{
		/*std::string textureMeta = ASSETS_TEXTURES; textureMeta += fileName + ".meta";
		if (App->fileSystem->FileExists(textureMeta.c_str())) {
			LOG("File %s already imported", absFilepath);
			return true;
		}*/
		//TODO: if file exists remove the lib file (meta maybe not because it will get rewriten) before reimporting
		App->fileSystem->DuplicateFile(normalPath.c_str(), ASSETS_TEXTURES, relativePath);
		App->resources->ImportFile(relativePath.c_str(), Resource::Type::TEXTURE);
		return true;
	}
	else if (extension == "scene") 
	{
		/*std::string sceneMeta = ASSETS_SCENES; sceneMeta += fileName + ".meta";
		if (App->fileSystem->FileExists(sceneMeta.c_str())) {
			LOG("File %s already imported", absFilepath);
			return true;
		}*/
		App->fileSystem->DuplicateFile(normalPath.c_str(), ASSETS_SCENES, relativePath);
		App->resources->ImportFile(normalPath.c_str(), Resource::Type::SCENE);
	}

	return false;
}

void Importer::Models::ImportFbx(const char* assetPath, char** libBuffer, unsigned int& libSize, char** metaBuffer, unsigned int& metaSize, ResourceModel* resource)
{
	//TODO: return a bool if any error?
	LOG("Importing Mesh");
	const aiScene* scene = aiImportFile(assetPath, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		GameObject* rootObject = ParseFbxNode(scene->mRootNode, scene, assetPath, nullptr);

		aiReleaseImport(scene);

		JSON_Value* rootValue = json_value_init_object();
		JSON_Object* node = json_value_get_object(rootValue);
		json_object_set_value(node, "GameObjects", json_value_init_array());
		JSON_Array* arry = json_object_get_array(node, "GameObjects");

		rootObject->Save(arry);
		metaSize = json_serialization_size_pretty(rootValue);
		*metaBuffer = new char[metaSize];
		json_serialize_to_buffer_pretty(rootValue, *metaBuffer, metaSize);
		json_value_free(rootValue);
	}
	else
		LOG("Error opening file: %s ", assetPath);
}

GameObject* Importer::Models::ParseFbxNode(aiNode* node, const aiScene* scene, const char* fbxPath, GameObject* parentGo)
{

	if (strcmp(node->mName.C_Str(), "RootNode") == 0) {

		std::string tmp = fbxPath;
		std::string name = tmp.substr(tmp.find_last_of("/") + 1);
		node->mName = name;
	}

	GameObject* go = nullptr;

	aiVector3D translation, scaling;

	aiQuaternion aiRotation;

	node->mTransformation.Decompose(scaling, aiRotation, translation);

	float3 pos(translation.x, translation.y, translation.z);
	float3 scale(scaling.x, scaling.y, scaling.z);
	Quat rot(aiRotation.x, aiRotation.y, aiRotation.z, aiRotation.w);
	Quat tempRotation;
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

	go = new GameObject(parentGo, node->mName.C_Str());


	if (!go->HasComponent(ComponentType::Transform))
	{
		ComponentTransform* transformComponent = new ComponentTransform(go, pos, scale, rot);
		//transformComponent->SetLocalTransform(pos, scale, rot);
	}

	//Detectar si es una camera
	aiCamera camera;

	camera.mName.C_Str();
	if ((strstr(node->mName.C_Str(), "Camera") != nullptr))
	{
		ComponentCamera* cameraComponent = new ComponentCamera(go);
	}

	if (node->mNumMeshes > 0) {
		//TODO: more than 1 mesh !!!! citybuilding 17
		for (int i = 0; i < node->mNumMeshes; ++i)
		{
			//Create a game object with its name and the parent of the go is this node
			//TODO: use the name of the node or the name of the mesh ?

			LOG("%s", node->mName.C_Str());

			//Create and attach all the components this node will need (using the scene check if it has a texture for adding a component material and add the transform and mesh component)

			//ResourceManager::ImportFile(assetPath)

			aiMesh* nodeMesh = scene->mMeshes[node->mMeshes[i]];
			Mesh ourMesh;
			unsigned int resourceID = Importer::Meshes::SaveMeshLib(nodeMesh, node->mName.C_Str());

			ComponentMesh* meshComponent = new ComponentMesh(resourceID);
			go->AddComponent(meshComponent);

			//Adding Materials
			if (scene->HasMaterials())
			{
				aiMaterial* material = scene->mMaterials[nodeMesh->mMaterialIndex];
				if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) //TODO: Import more than one texture on a mesh (still not suported on the shader level)
				{
					aiString name;
					material->GetTexture(aiTextureType_DIFFUSE, 0, &name);
					/*TODO: MES DE UNA TEXTURA EN UN MESH!!!!! (cal fer el shader tmbe i posarli 2 components material)
					for (int i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); ++i) {
						material->GetTexture(aiTextureType_DIFFUSE, i, &name);
						//processar cada textura !!!! 
					}*/

					std::string texturefbxRelativePath = App->fileSystem->NormalizePath(name.C_Str());
					std::string fbxFilePath;
					App->fileSystem->SplitFilePath(fbxPath, &fbxFilePath);
					std::string texturePath = fbxFilePath + texturefbxRelativePath;


					//uint textureID = Textures::Import(newfilePath.c_str());
					std::string assetsPath;
					std::string fileName;
					App->fileSystem->SplitFilePath(texturePath.c_str(), &assetsPath, &fileName);
					assetsPath = ASSETS_TEXTURES + fileName + ".meta";
					if (!App->fileSystem->FileExists(assetsPath.c_str()))
					{
						std::string relativePath;
						if (App->fileSystem->DuplicateFile(texturePath.c_str(), ASSETS_TEXTURES, relativePath)) 
						{
							unsigned int resourceID = App->resources->ImportFile(relativePath.c_str(), Resource::Type::TEXTURE);

							ComponentMaterial* materialComponent = new ComponentMaterial(resourceID);
							go->AddComponent(materialComponent);
						}
					}
					else 
					{
						char* metafile = nullptr;
						App->fileSystem->Load(assetsPath.c_str(), &metafile);
						JSON_Value* rootValue = json_parse_string(metafile);
						if (rootValue) {
							JSON_Object* node = json_value_get_object(rootValue);
							unsigned int resourceID = json_object_get_number(node, "LIBUID");

							ComponentMaterial* materialComponent = new ComponentMaterial((ResourceTexture*)App->resources->RequestResource(resourceID, Resource::Type::TEXTURE));
							
							go->AddComponent(materialComponent);
							json_value_free(rootValue);
						}
						delete[] metafile;
						metafile = nullptr;
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

	return go;
}

void Importer::Textures::Init() //Problem: Where do we call these function. Do we create a modul Importer?
{
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
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

char* Importer::Meshes::SaveMesh(aiMesh* mesh)
{
	uint numVertices = mesh->mNumVertices;
	uint numIndices = 0;
	uint numTextureCoordinates = 0;
	uint numNormals = 0;
	
	if (mesh->HasFaces())
		numIndices = mesh->mNumFaces * 3;
	if (mesh->HasTextureCoords(0))
		numTextureCoordinates = numVertices * 2;
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
		uint * udata = new uint[numIndices];
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

	return fileBuffer;
}

unsigned int Importer::Meshes::SaveMeshLib(aiMesh* mesh, const char* name)
{
	//Creating lib file from aiMesh
	uint numVertices = mesh->mNumVertices;
	uint numIndices = 0;
	uint numTextureCoordinates = 0;
	uint numNormals = 0;

	if (mesh->HasFaces())
		numIndices = mesh->mNumFaces * 3;
	if (mesh->HasTextureCoords(0))
		numTextureCoordinates = numVertices * 2;
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
		uint* udata = new uint[numIndices];
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

	//Saving lib file
	std::string path = MESHES_PATH;
	unsigned int id = App->resources->GenerateNewUID();
	Resource* resource = new Resource(id,Resource::Type::MESH);
	App->resources->AddMeshResource(id,resource);
	std::string fileName = std::to_string(id);
	path += fileName;
	App->fileSystem->Save(path.c_str(), fileBuffer, size);
	delete[] fileBuffer;
	fileBuffer = nullptr;

	//Creating and saving metafile
	JSON_Value* rootValue = json_value_init_object();
	JSON_Object* node = json_value_get_object(rootValue);
	json_object_set_number(node, "LIBUID", id);
	size_t Size = json_serialization_size_pretty(rootValue);
	char* buffer = new char[Size];
	json_serialize_to_buffer_pretty(rootValue, buffer, Size);

	path = ASSETS_MESHES;
	path += name;
	path += ".mesh";
	App->fileSystem->Save(path.c_str(), buffer, Size);
	
	json_value_free(rootValue);
	delete[] buffer;
	buffer = nullptr;

	return id;
}

void Importer::Meshes::LoadMesh(char* buffer, Mesh* ourMesh)
{
	char* cursor = buffer;

	uint ranges[4];
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);
	cursor += bytes;

	ourMesh->numIndices = ranges[0];
	ourMesh->numVertices = ranges[1];
	ourMesh->numTexcoords = ranges[2];
	ourMesh->numNormals = ranges[3];

	if (ourMesh->numIndices) {
		bytes = sizeof(uint) * ourMesh->numIndices;
		ourMesh->indexData = new uint[ourMesh->numIndices];
		memcpy(ourMesh->indexData, cursor, bytes);
		cursor += bytes;
	}

	bytes = sizeof(float) * ourMesh->numVertices * 3;
	ourMesh->vertexData = new float[ourMesh->numVertices * 3];
	memcpy(ourMesh->vertexData, cursor, bytes);
	cursor += bytes;

	if (ourMesh->numTexcoords) {
		bytes = sizeof(float) * ourMesh->numTexcoords;
		ourMesh->texturecoordsData = new float[ourMesh->numTexcoords];
		memcpy(ourMesh->texturecoordsData, cursor, bytes);
		cursor += bytes;
	}

	if (ourMesh->numNormals) {
		bytes = sizeof(float) * ourMesh->numNormals;
		ourMesh->normalsData = new float[ourMesh->numNormals];
		memcpy(ourMesh->normalsData, cursor, bytes);
		cursor += bytes;
	}
	cursor = nullptr;
}

void Importer::Textures::Import(const char* imgPath, char** libBuffer,unsigned int& libSize, char** metaBuffer,unsigned int& metaSize, ResourceTexture* resource)
{
	uint ImageName;
	ilGenImages(1, &ImageName);
	ilBindImage(ImageName);

	if (!ilLoadImage(imgPath)) {
		LOG("Error loading the image");
		ILenum Error;
		while ((Error = ilGetError()) != IL_NO_ERROR) {
			LOG("Importing image error %d: %s", Error, iluErrorString(Error));
		}
	}

	ILuint size;
	ILubyte* data;

	ilSetInteger(IL_DXTC_DATA_FORMAT, IL_DXT5);

	size = ilSaveL(IL_DDS, nullptr, 0);

	if (size > 0)
	{
		data = new ILubyte[size];

		if (ilSaveL(IL_DDS, data, size) > 0)
			*libBuffer = (char*)data;
		else
		{
			*libBuffer = nullptr;
		}
	}
	
	libSize = size;

	JSON_Value* rootValue = json_value_init_object();
	JSON_Object* node = json_value_get_object(rootValue);
	json_object_set_number(node, "LIBUID", resource->GetUID());
	json_object_set_number(node, "Type", (unsigned int)resource->GetType());
	json_object_set_number(node, "BufferSize", size);
	json_object_set_string(node, "ImageExtension", "DDS");
	json_object_set_number(node, "ImageExtensionDefine", IL_DDS);
	json_object_set_string(node, "DXT", "DXT5");
	json_object_set_number(node, "DXTDefine", IL_DXT5);
	metaSize = json_serialization_size_pretty(rootValue);
	*metaBuffer = new char[metaSize];
	json_serialize_to_buffer_pretty(rootValue, *metaBuffer, metaSize);
	json_value_free(rootValue);
}

ILuint Importer::Textures::SaveTexture(const char * imagePath, char** fileBuffer)
{
	uint ImageName;
	ilGenImages(1, &ImageName);
	ilBindImage(ImageName);

	if (!ilLoadImage(imagePath)) {
		LOG("Error loading the image");
		ILenum Error;
		while ((Error = ilGetError()) != IL_NO_ERROR) {
			LOG("Importing image error %d: %s", Error, iluErrorString(Error));
		}
	}
	
	ILuint size;
	ILubyte* data;

	ilSetInteger(IL_DXTC_DATA_FORMAT, IL_DXT5);

	size = ilSaveL(IL_DDS, nullptr, 0);

	if (size > 0)
	{
		data = new ILubyte[size];

		if (ilSaveL(IL_DDS, data, size) > 0)
			*fileBuffer = (char*)data;
		else
		{
			*fileBuffer = nullptr;
		}
	}
	return size;
}

uint Importer::Textures::LoadTexture(char* buffer, uint size)
{
	ILuint il_image = 0;
	ilGenImages(1, &il_image);
	ilBindImage(il_image);
	ilLoadL(IL_DDS, buffer, size);
	uint texture = ilutGLBindTexImage();
	ilutGLBindMipmaps();
	ilDeleteImages(1, &il_image);
	return texture;
}

void Importer::Scenes::Import(const char* scenePath, char** libBuffer, unsigned int& libSize, char** metaBuffer, unsigned int& metaSize, ResourceScene* resource)
{
	//Copying the file to lib directory
	libSize = App->fileSystem->Load(scenePath, libBuffer);

	//Generating metafile
	JSON_Value* rootValue = json_value_init_object();
	JSON_Object* node = json_value_get_object(rootValue);
	json_object_set_number(node, "LIBUID", resource->GetUID());
	metaSize = json_serialization_size_pretty(rootValue);
	*metaBuffer = new char[metaSize];
	json_serialize_to_buffer_pretty(rootValue, *metaBuffer, metaSize);
	json_value_free(rootValue);
}
