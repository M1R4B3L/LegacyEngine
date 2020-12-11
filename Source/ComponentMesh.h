#pragma once

#include "Component.h"
#include "Globals.h"
#include "Dependencies/MathGeolib/Geometry/AABB.h"

class Component;



struct Mesh {
    uint VAO; //vertex array object
    uint VBO; //vertex buffer object
    uint numVertices;
    uint numTexcoords;
    uint numNormals;
    uint EBO; //element buffer object
    uint numIndices;
    uint* indexData = nullptr; //TODO: S'han de borrar els buffers !!!
    float* vertexData = nullptr;
    float* normalsData = nullptr;
    float* texturecoordsData = nullptr;
    uint difuseTexture = 0;

    AABB aabb;

};

class ComponentMesh : public Component
{
public:
    ComponentMesh();
    ComponentMesh(unsigned int iVAO, unsigned int vertices, unsigned int indices, AABB aabb);
    ~ComponentMesh();

    const unsigned int GetVAO() const;
    const unsigned int GetNumIndices() const;
    const unsigned int GetNumVertex() const;
    const AABB& GetAABB() const;

 
    bool IsActive()const override;
    void SetActive(bool active) override;

    void Save(JSON_Array* componentsArry)const override;
    void Load(JSON_Object* componentObj) override;
    
private:
    unsigned int VAO;
    unsigned int numIndices;
    unsigned int numVertices;
    
    AABB aabb;

public:
    bool activeMesh = true;

};