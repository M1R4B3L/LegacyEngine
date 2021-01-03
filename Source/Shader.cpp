#include "Shader.h"
#include "Globals.h"
#include "GL/glew.h"
#include "ModuleFileSystem.h"
#include "Application.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	char* vertexCode = nullptr;
	char* fragmentCode = nullptr;
	//char** a = PHYSFS_getSearchPath();
	//physfs_mkdir
	//physfs_SET
	//addpath and create dir
	App->fileSystem->Load(vertexPath, &vertexCode);
	App->fileSystem->Load(fragmentPath, &fragmentCode);

	if(vertexCode != nullptr && fragmentCode != nullptr)
	{
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

void Shader::setMat4(const std::string& name, float* value) const
{
	unsigned int transformLoc = glGetUniformLocation(ID, name.c_str());
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value);
}

void Shader::setFloat4(const std::string& name, float value1, float value2, float value3, float value4) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3, value4);
}
