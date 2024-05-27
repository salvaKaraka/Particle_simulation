#include "VAO.h"

VAO::VAO()
{
	GLCall(glGenVertexArrays(1, &m_ArrayID));
}

VAO::~VAO()
{
	Delete();
}

void VAO::LinkAttribute(const VBO& vb, GLuint index, GLuint count, GLenum type, GLboolean normalized, GLsizeiptr stride, unsigned int offset)
{
	vb.Bind();
	GLCall(glEnableVertexAttribArray(index));
	GLCall(glVertexAttribPointer(index, count, type, normalized, stride, (void*)offset));
}

void VAO::Bind() const
{

	GLCall(glBindVertexArray(m_ArrayID));
}

void VAO::Unbind() const
{
	GLCall(glBindVertexArray(0));
}

void VAO::Delete() const
{
	GLCall(glDeleteVertexArrays(1, &m_ArrayID));
}