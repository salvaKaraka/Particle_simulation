#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string get_file_contents(const char* path);

class ShaderProgram {
private:
	GLuint ID;
public:
	ShaderProgram(const char* vertexFile, const char* fragmentFile);
	~ShaderProgram();

	const GLuint GetID();
	void Use();
	void Delete();
};