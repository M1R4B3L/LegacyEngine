#pragma once

#include "Component.h"
#include "Globals.h"

class Component;

struct Mesh {
    uint VAO; //vertex array object
    uint VBO; //vertex buffer object
    uint numVertex;
    uint numTexcoords;
    uint numNormals;
    uint EBO; //element buffer object
    uint numIndices;
    uint* index = nullptr; //TODO: S'han de borrar els buffers !!!
    float* vertex = nullptr;
    float* normals;
    float* texturecoords;
    uint difuseTexture = 0;
};

class ComponentMesh : public Component
{
public:
    ComponentMesh() = delete;
    ComponentMesh(unsigned int iVAO, unsigned int vertices, unsigned int indices);
    ~ComponentMesh();

    const unsigned int GetVAO() const;
    const unsigned int GetNumIndices() const;
    
private:
    unsigned int VAO;
    unsigned int numIndices;
    unsigned int numVertices;
};