#pragma once
#include "ErrorChecker.h"
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <unordered_map>

std::string get_file_contents(const char* path);

class ShaderProgram {
private:
	GLuint m_programID;
	std::unordered_map<std::string, GLint> m_uniformLocationCache;
	
	GLint getUniformLocation(const std::string& name);
public:
	ShaderProgram(const char* vertexFile, const char* fragmentFile);
	~ShaderProgram();

	// Set uniforms
	void setUniform1f(const std::string& name, GLfloat value);
	void setUniform2f(const std::string& name, GLfloat x, GLfloat y);
	void setUniform1i(const std::string& name, GLint value);
	void setUniform2i(const std::string& name, GLint x, GLint y);
	void setUniformMat4f(const std::string& name, const glm::mat4& matrix);

	const GLuint GetID() const;
	void Use() const;
	void Delete();
};