#include "ResourceTexture.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "Globals.h"
#include <string>
#include "Importer.h"
#include "ModuleRenderer3D.h"

ResourceTexture::ResourceTexture(unsigned int id) : Resource(id, Resource::Type::TEXTURE){}

ResourceTexture::~ResourceTexture() { App->renderer3D->DeleteTexture(&gpuID); }

bool ResourceTexture::LoadInMemory()
{
	char* fileBuffer = nullptr;
	std::string path = TEXTURES_PATH;
	path += std::to_string(uid);
	unsigned int size = App->fileSystem->Load(path.c_str(), &fileBuffer);
	gpuID = Importer::Textures::LoadTexture(fileBuffer, size);
	delete[] fileBuffer;
	fileBuffer = nullptr;
	return true;
}