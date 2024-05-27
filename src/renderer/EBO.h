#pragma once

#include <glad/glad.h>
#include "ErrorChecker.h"

class EBO
{
public:
	EBO(const GLuint* data, GLuint count);
	~EBO();

	inline GLuint GetCount() const { return m_Count; }

	void Bind() const;
	void Unbind() const;
	void Delete() const;

private:
	GLuint m_BufferID;
	GLuint m_Count;
};