#include "ShaderProgram.h"

std::string get_file_contents(const char* path) {
	std::ifstream in(path, std::ios::binary);
	if (in) {
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return contents;
	}
	throw(errno);
}

ShaderProgram::ShaderProgram(const char* vertexFile, const char* fragmentFile) {
	std::string vertexShaderCode = get_file_contents(vertexFile);
	std::string fragmentShaderCode = get_file_contents(fragmentFile);

	const char* vertexShaderSource = vertexShaderCode.c_str();
	const char* fragmentShaderSource = fragmentShaderCode.c_str();

	// compilo los shaders
	GLCall(GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER));		//referencia para guardar el vertex shader
	GLCall(glShaderSource(vertexShader, 1, &vertexShaderSource, NULL)); //asigno el codigo fuente al vertex shader
	GLCall(glCompileShader(vertexShader));								//compilo el shader

	GLCall(GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER));
	GLCall(glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL));
	GLCall(glCompileShader(fragmentShader));

	int iIsOk = 0;
	GLCall(glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &iIsOk));
	if (iIsOk == GL_FALSE) {
		int length;
		GLCall(glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(vertexShader, length, &length, message));
		std::cout << "Failed to compile vertex shader!" << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(vertexShader));
	}

	GLCall(glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &iIsOk));
	if (iIsOk == GL_FALSE) {
		int length;
		GLCall(glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(fragmentShader, length, &length, message));
		std::cout << "Failed to compile fragment shader!" << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(fragmentShader));
	}

	//creo el programa y lo enlazo con los shaders
	GLCall(m_programID = glCreateProgram());		//referencia para guardar el programa
	GLCall(glAttachShader(m_programID, vertexShader));	//enlazo el vertex shader
	GLCall(glAttachShader(m_programID, fragmentShader));	//enlazo el fragment shader
	GLCall(glLinkProgram(m_programID));					//enlazo el programa

	int isLinked = 0;
	GLCall(glGetProgramiv(m_programID, GL_LINK_STATUS, &isLinked));
	if (isLinked == GL_FALSE) {
		int length;
		GLCall(glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetProgramInfoLog(m_programID, length, &length, message));
		std::cout << "Failed to link shader program!" << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteProgram(m_programID));
	}

	//elimino los shaders ya que no los necesito mas porque ya estan enlazados
	GLCall(glDeleteShader(vertexShader));
	GLCall(glDeleteShader(fragmentShader));
}


//uniforms
void ShaderProgram::setUniform1f(const std::string& name, GLfloat value) {
	GLCall(glUniform1f(getUniformLocation(name.c_str()), value));
}

void ShaderProgram::setUniform2f(const std::string& name, GLfloat x, GLfloat y) {
	GLCall(glUniform2f(getUniformLocation(name.c_str()), x, y));
}

void ShaderProgram::setUniform1i(const std::string& name, GLint value) {
	GLCall(glUniform1i(getUniformLocation(name.c_str()), value));
}

void ShaderProgram::setUniform2i(const std::string& name, GLint x, GLint y) {
	GLCall(glUniform2i(getUniformLocation(name.c_str()), x, y));
}

void ShaderProgram::setUniformMat4f(const std::string& name, const glm::mat4& matrix) {
	GLCall(glUniformMatrix4fv(getUniformLocation(name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix)));
}

GLint ShaderProgram::getUniformLocation(const std::string& name) 
{

	if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
		return m_uniformLocationCache[name];

	GLCall(GLint location = glGetUniformLocation(m_programID, name.c_str()));
	if (location == -1) {
		std::cout << "Warning: uniform '" << name << "' doesn't exist in shader program!" << std::endl;
	}
	m_uniformLocationCache[name] = location;
	return location;
}

const GLuint ShaderProgram::GetID() const {
	return m_programID;
}

void ShaderProgram::Use() const{
	GLCall(glUseProgram(m_programID));
}

void ShaderProgram::Delete() {
	GLCall(glDeleteProgram(m_programID));
}

ShaderProgram::~ShaderProgram() {
	Delete();
}