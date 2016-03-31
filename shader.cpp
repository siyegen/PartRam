#include "shader.h"

Shader::Shader(const GLchar * vertexPath, const GLchar * fragmentPath, const GLchar* geometryPath) {
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;

	// setup exceptions for ifstream
	vShaderFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
	fShaderFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
	gShaderFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);

	try {
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream, gShaderStream;
		// read into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close fh
		vShaderFile.close();
		fShaderFile.close();
		// convert to GLchar
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		if (geometryPath) {
			gShaderFile.open(geometryPath);
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryCode = gShaderStream.str();
		}

	} catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_READ" << std::endl;
		return;
	}
	// Should clean these up
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();
	const GLchar* gShaderCode = nullptr;

	GLuint vertex, fragment, geometry;

	// Vertex
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);
	checkShaderError(vertex);

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);
	checkShaderError(fragment);

	if (geometryPath) {
		gShaderCode = geometryCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, nullptr);
		glCompileShader(geometry);
		checkShaderError(geometry);
	}

	Program = glCreateProgram();
	glAttachShader(Program, vertex);
	glAttachShader(Program, fragment);
	if (geometryPath) {
		glAttachShader(Program, geometry);
	}
	glLinkProgram(Program);
	checkLinkError(Program);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometryPath) {
		glDeleteShader(geometry);
	}
}

void Shader::Use() {
	glUseProgram(Program);
}

bool Shader::checkShaderError(GLuint glIdentifier) {
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(glIdentifier, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(glIdentifier, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		return true;
	}
	return false;
}

bool Shader::checkLinkError(GLuint program) {
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(program, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::LINK_FAILED\n" << infoLog << std::endl;
		return true;
	}
	return false;
}
