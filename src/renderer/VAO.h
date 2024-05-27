#pragma once
#include "ErrorChecker.h"
#include "VBO.h"
#include <glad/glad.h>

class VAO
{
public:
	VAO();
	~VAO();


	void LinkAttribute(const VBO& vb, GLuint index, GLuint count, GLenum type, GLboolean normalized, GLsizeiptr stride, unsigned int offset);
	void Bind() const;
	void Unbind() const;
	void Delete() const;

private:
	GLuint m_ArrayID;
};