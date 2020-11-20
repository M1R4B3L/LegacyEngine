#pragma once

#include "Component.h"
#include "Globals.h"
#include <vector>

class Component;

struct Mesh {
    uint VAO; //vertex array object
    uint VBO; //vertex buffer object
    uint num_vertex;
    uint num_texcoords;
    uint num_normals;
    uint EBO; //element buffer object
    uint num_indices;
    uint* index = nullptr;
    float* vertex = nullptr;
    float* normals;
    float* texturecoords;
    uint difuseTexture = 0;
};

class ComponentMesh : public Component
{
public:
    ComponentMesh(std::vector<Mesh> imported_meshes);
    void Update(float dt) override;
private:
    std::vector<Mesh> meshes;
};