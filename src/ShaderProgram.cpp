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
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);		//referencia para guardar el vertex shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //asigno el codigo fuente al vertex shader
	glCompileShader(vertexShader);								//compilo el shader

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//creo el programa y lo enlazo con los shaders
	ID = glCreateProgram();		//referencia para guardar el programa
	glAttachShader(ID, vertexShader);	//enlazo el vertex shader
	glAttachShader(ID, fragmentShader);	//enlazo el fragment shader
	glLinkProgram(ID);					//enlazo el programa

	//elimino los shaders ya que no los necesito mas porque ya estan enlazados
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

const GLuint ShaderProgram::GetID() {
	return ID;
}

void ShaderProgram::Use() {
	glUseProgram(ID);
}

void ShaderProgram::Delete() {
	glDeleteProgram(ID);
}

ShaderProgram::~ShaderProgram() {
	Delete();
}