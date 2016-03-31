#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class Shader {
public:
	// The program ID
	GLuint Program;
	// Basic constructor 
	// Constructor reads and builds the shader
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath=nullptr);
	// Use the program
	void Use();
private:
	bool checkShaderError(GLuint shader);
	bool checkLinkError(GLuint program);
};

#endif