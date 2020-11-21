#pragma once
#include <vector>
#include "Globals.h"

struct Mesh;
class GameObject;
class aiNode;
class aiScene;

namespace Importer {
    bool ImportDroped(const char* filepath);
    namespace Meshes {
        void ImportFbx(const char* fbxPath);
        void ParseFbxNode(aiNode * node, const aiScene * scene, const char * path, GameObject* parentGo = nullptr);
        std::vector<Mesh> Import(const char* absFilepath);
    }
    namespace Textures {
        void Init();
        uint DropedImage();
        uint Import(const char* imagePath);
        //uint GenerateTexture(const char* imagePath);
        uint checkerImage();
    }
}
