#pragma once
#include <vector>
#include "Globals.h"
// Stream log messages to Debug window
/*
struct aiLogStream stream;
stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
aiAttachLogStream(&stream);
*/

//struct ourmesh!!!!
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
};

//crear un metode que lodegi un file
//aig de crear la llibreria physfs
//amb el metode load tot a la struct de ourMesh (vertex i index)
std::vector<Mesh> LoadFileMeshes(const char* file_path);

void setupmesh(Mesh& mesh);
//Fer un Draw() Method al modul renderer que rebi un ourMesh i el dibuixi (glDrawElements())
//Fer un loop a la scene que dibuixi tots els meshes

//EXTRA
//ficar en una window la scena
//ordenar els moduls de imgui i implementar la seva funcionalitat
//Implementar el drag and drop
//ficar llibreria physfx
//physfx al lodejar un asset ferne una copia en una nova carpeta i lodejar la copia (implementar un file system (mirar homework del loading files))
//dibuixar les normals els colors i les texture coordinates
