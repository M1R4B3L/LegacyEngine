#pragma once
#include<string>
#include <vector>

class aiNode;
class aiMesh;
class aiScene;

struct Vertex {
    //mathgeolib float 3
	float Position[3];
	//vec3 Normal;
	//vec2 TexCoords;
};

/*struct Texture {
	unsigned int id;
	std::string type;
};*/

class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	//std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);// std::vector<Texture> textures);
	void Draw();//Shader& shader);
private:
	unsigned int VAO, VBO, EBO;

	void setupMesh();
};

class Model
{
public:
    Model(char* path)
    {
        loadModel(path);
    }
    void Draw();//Shader& shader);
private:
    // model data
    std::vector<Mesh> meshes;
    std::string directory;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    //std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
    //    std::string typeName);
};