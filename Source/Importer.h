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
class ResourceScene;

namespace Importer {
    bool ImportDroped(const char* filepath);
    namespace Meshes {
        char* SaveMesh(aiMesh* mesh);
        void LoadMesh(char* buffer, Mesh* mesh);
        unsigned int SaveMeshLib(aiMesh* mesh, const char* name);
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
    namespace Scenes {
        void Import(const char* scenePath, char** libBuffer, unsigned int& libSize, char** metaBuffer, unsigned int& metaSize, ResourceScene* resource);
    }
}