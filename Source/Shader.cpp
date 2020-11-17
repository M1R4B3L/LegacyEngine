#include "Shader.h"
#include "physfs.h"
#include "Globals.h"
#include "GL/glew.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	char* vertexCode = nullptr;
	char* fragmentCode = nullptr;
	//char** a = PHYSFS_getSearchPath();
	//physfs_mkdir
	//physfs_SET
	//addpath and create dir
	PHYSFS_File* vertexFile = PHYSFS_openRead(vertexPath);
	PHYSFS_File* fragmentFile = PHYSFS_openRead(fragmentPath);

	if (vertexFile != nullptr && fragmentFile != nullptr) {

		PHYSFS_sint32 vertexSize = (PHYSFS_sint32)PHYSFS_fileLength(vertexFile);
		if (vertexSize > 0) {
			vertexCode = new char[vertexSize]; //TODO: ns pk al llegir el file m'afageix 4 characters "y"rara al final que no em deixen compilar el shader
			unsigned int readedVertex = (unsigned int)PHYSFS_readBytes(vertexFile, vertexCode, vertexSize);
			if (readedVertex != vertexSize)
			{
				LOG("Shader error while reading from vertex file %s: %s\n", vertexPath, PHYSFS_getLastError());
			}
		}

		PHYSFS_sint32 fragmentSize = (PHYSFS_sint32)PHYSFS_fileLength(fragmentFile);
		if (fragmentSize > 0) {
			fragmentCode = new char[fragmentSize];
			unsigned int readedFragment = (unsigned int)PHYSFS_readBytes(fragmentFile, fragmentCode, fragmentSize);
			if (readedFragment != fragmentSize)
			{
				LOG("Shader error while reading from fragment file %s: %s\n", vertexPath, PHYSFS_getLastError());
			}
		}
		if (PHYSFS_close(vertexFile) == 0)
			LOG("Shader error while closing vertex file %s: %s\n", vertexPath, PHYSFS_getLastError());
		if (PHYSFS_close(fragmentFile) == 0)
			LOG("Shader error while closing fragment file %s: %s\n", fragmentPath, PHYSFS_getLastError());

		unsigned int vertex, fragment;
		int success;
		char infoLog[512];

		//vertex shader
		//LOG("vertex shader: %s", vertexCode);
		vertex = glCreateShader(GL_VERTEX_SHADER); //Necesitare el glgetshadermacro() si no sse quin tipus de shader es
		glShaderSource(vertex, 1, &vertexCode, NULL);
		glCompileShader(vertex);

		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			LOG("ERROR::SHADER::VERTEX::COMPILATION_FAILED : %s\n", infoLog);
		};

		//fragment shader
		//LOG("Fragment shader: %s\n", fragmentCode);
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentCode, NULL);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			LOG("ERROR::SHADER::VERTEX::COMPILATION_FAILED: %s\n", infoLog);
		};

		// shader Program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);

		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			LOG("ERROR::SHADER::PROGRAM::LINKING_FAILED\n", infoLog);
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
		RELEASE_ARRAY(vertexCode);
		RELEASE_ARRAY(fragmentCode);
	}
	else {
		LOG("Error opening shader files");
	}
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
