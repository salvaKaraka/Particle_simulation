#include "EBO.h"

EBO::EBO(const GLuint* data, GLuint count) 
	: m_Count{count}
{
	GLCall(glGenBuffers(1, &m_BufferID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * count, data, GL_STATIC_DRAW));
}

EBO::~EBO() {
	Delete();
}

void EBO::Bind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_BufferID));
}

void EBO::Unbind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void EBO::Delete() const {
	GLCall(glDeleteBuffers(1, &m_BufferID));
}