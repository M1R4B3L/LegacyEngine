#pragma once
#include <vector>
#include "Globals.h"

struct Mesh;
class GameObject;
class aiNode;
class aiScene;
class aiMesh;
class ResourceTexture;
class ResourceModel;

namespace Importer {
    bool ImportDroped(const char* filepath);
    namespace Meshes {
        void ImportFbx(const char* fbxPath);
        void ParseFbxNode(aiNode * node, const aiScene * scene, const char * path,GameObject* parentGo = nullptr);
        std::vector<Mesh> Import(const char* absFilepath);
        char* SaveMesh(aiMesh* mesh);
        void LoadMesh(char* buffer, Mesh* mesh);
        unsigned int SaveMeshLib(aiMesh* mesh);
    }
    namespace Models {
        void ImportFbx(const char* assetPath, char** libBuffer, unsigned int& libSize, char** metaBuffer, unsigned int& metaSize, ResourceModel* resource);
        GameObject* ParseFbxNode(aiNode* node, const aiScene* scene, const char* fbxPath, GameObject* parentGo);
    }
    namespace Textures {
        void Init();
        void Import(const char * imgPath, char** libBuffer,unsigned int& libSize, char**metaBuffer, unsigned int& metaSize, ResourceTexture* resource);
        //uint GenerateTexture(const char* imagePath);
        uint checkerImage();
        uint SaveTexture(const char* imagePath, char** fileBuffer);
        uint LoadTexture(char* buffer, uint size);
    }
}