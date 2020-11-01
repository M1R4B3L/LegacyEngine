#pragma once
#include <vector>
#include "Globals.h"

struct Mesh;
class GameObject;

namespace Importer {
    namespace Meshes {

        std::vector<Mesh> Import(const char* filePath);
        void setupmesh(Mesh& mesh);
    }
    namespace Textures {
        void Init();
        uint Import(const char* imagePath);
        uint GenerateTexture(const char* imagePath);
        uint checkerImage();
    }
}
