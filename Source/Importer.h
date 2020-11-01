#pragma once
#include <vector>
#include "Globals.h"



struct Mesh {
    uint VAO; //vertex array object
    uint VBO; //vertex buffer object
    uint num_vertex;
    float* vertex = nullptr;
    uint EBO; //element buffer object
    uint num_indices;
    uint* index = nullptr;
    float* normals;
    float* texturecoords;
    uint difuseTexture = 0;
};

namespace Importer {
    namespace Meshes {

        std::vector<Mesh> Import(const char* filePath);

        void setupmesh(Mesh& mesh);
    }
    namespace Textures {
        void Init();
        uint Import(const char* imagePath);
        uint GenerateTexture(uint& imageName);
    }
}
