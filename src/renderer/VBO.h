#pragma once

#include <glad/glad.h>
#include "ErrorChecker.h"

class VBO
{
public:
	VBO(GLfloat* vertices, GLsizeiptr size);
	~VBO();

	void Bind() const;
	void Unbind() const;
	void Delete() const;

private:
	GLuint m_BufferID;
};