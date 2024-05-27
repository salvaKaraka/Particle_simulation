#include "VBO.h"

VBO::VBO(GLfloat* vertices, GLsizeiptr size) {
	GLCall(glGenBuffers(1, &m_BufferID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_BufferID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));
}

VBO::~VBO() {
	Delete();
}

void VBO::Bind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_BufferID));
}

void VBO::Unbind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VBO::Delete() const {
	GLCall(glDeleteBuffers(1, &m_BufferID));
}