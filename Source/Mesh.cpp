#include "GL/glew.h"
#include "Globals.h"
#include "Mesh.h"
#include "scene.h"
#include "cimport.h"
#include "postprocess.h"

/*bool Mesh::LoadMesh(const std::string& Filename)
{
    // Release the previously loaded mesh (if it exists)
    Clear();

    bool Ret = false;
    const aiScene* scene = aiImportFile(Filename.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

    if (scene && scene->HasMeshes()) {
        // Use scene->mNumMeshes to iterate on scene->mMeshes array
        //em falta definir el typo de arrays m_Entries
        for (int i = 0; i < scene->mNumMeshes; ++i) {
            m_Entries[i] = scene->mMeshes[i];
            m_Entries[i].num_vertices = scene->mMeshes[i]->mNumVertices;
            m_Entries[i].vertices = new float[m_Entries[i].num_vertices * 3];
            memcpy(m_Entries[i].vertices, scene->mMeshes[i]->mVertices, sizeof(float) * m_Entries[i].num_vertices * 3);
            LOG("New mesh with %d vertices", m_Entries[i].num_vertices);
        }
        aiReleaseImport(scene);
    }
    else {
        LOG("Error parsing '%s': '%s'\n", Filename.c_str(), aiGetErrorString());
    }

    return Ret;
}*/

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices): vertices(vertices), indices(indices)
{
    setupMesh();
}

void Mesh::Draw()
{
    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::setupMesh()
{
    //fer un importer k llegeix dades i omple dades Reb i et retorna un vector
    //el glew no te k surtir aki (cal fer una funcio encapsulant la creacio dels buffers)
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    /*
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    */

    glBindVertexArray(0);
}

void Model::Draw()
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw();
}

void Model::loadModel(std::string path)
{
    const aiScene* scene = aiImportFile(path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        LOG( "ERROR::ASSIMP::%s", aiGetErrorString());
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);

    aiReleaseImport(scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    //std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        vertex.Position[0] = mesh->mVertices[i].x;
        vertex.Position[1] = mesh->mVertices[i].y;
        vertex.Position[2] = mesh->mVertices[i].z;
        // process vertex positions, normals and texture coordinates
        vertices.push_back(vertex);
    }
    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    /*
    // process material
    if (mesh->mMaterialIndex >= 0)
    {
        [...]
    }
    */
    return Mesh(vertices, indices);//, textures);
}