#pragma once
#include "Resource.h"
#include "Dependencies/MathGeoLib/MathGeoLib.h"

struct Mesh {
    ~Mesh() 
    {
        if (indexData != nullptr) 
        {
            delete[] indexData;
            indexData = nullptr;
        }
        if (vertexData != nullptr) 
        {
            delete[] vertexData;
            vertexData = nullptr;
        }
        if (texturecoordsData != nullptr) 
        {
            delete[] texturecoordsData;
            texturecoordsData = nullptr;
        }
        if (normalsData != nullptr) 
        {
            delete[] normalsData;
            normalsData = nullptr;
        }
    }
    unsigned int VAO; //vertex array object
    unsigned int VBO; //vertex buffer object
    unsigned int numVertices;
    unsigned int numTexcoords;
    unsigned int numNormals;
    unsigned int EBO; //element buffer object
    unsigned int numIndices;
    unsigned int* indexData = nullptr; //TODO: S'han de borrar els buffers !!!
    float* vertexData = nullptr;
    float* normalsData = nullptr;
    float* texturecoordsData = nullptr;
    unsigned int textureBuffer = 0;

    AABB aabb;

};

class ResourceMesh : public Resource
{
public:
	ResourceMesh(unsigned int id);
	~ResourceMesh();
	bool LoadInMemory() override;
	//void Load(const JSON_Object* config) override;
public:
	unsigned int numIndices = 0;
	unsigned int numVertices = 0;
	unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;
    unsigned int textureBuffer = 0;
    //unsigned int normalsBuffer = 0;
	AABB aabb;
};