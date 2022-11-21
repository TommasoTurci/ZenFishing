#include "ShaderMaker.h"
#include <iostream>
#include <fstream>

#pragma warning(disable:4996)
char* ShaderMaker::readShaderSource(const char* shaderFile)
{
	FILE* fp = fopen(shaderFile, "rb");

	if (fp == NULL) { return NULL; }

	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	char* buf = new char[size + 1];
	fread(buf, 1, size, fp);

	buf[size] = '\0';
	fclose(fp);

	return buf;
}

GLuint ShaderMaker::createProgram(char* vertexfilename, char *fragmentfilename)
{
	GLenum ErrorCheckValue = glGetError();
	int success;
	char infoLog[512];


	glClearColor(0.0, 0.0, 0.0, 0.0);
	GLchar* VertexShader = readShaderSource(vertexfilename);

	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, (const char**)&VertexShader, NULL);

	glCompileShader(vertexShaderId); 
	
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	const GLchar* FragmentShader = readShaderSource(fragmentfilename);
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, (const char**)&FragmentShader, NULL);
	glCompileShader(fragmentShaderId);

	 
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	ErrorCheckValue = glGetError();
	cout << ErrorCheckValue;

	if (ErrorCheckValue != GL_NO_ERROR)
	{
		fprintf(
			stderr,
			"ERROR: Could not create the shaders: %s \n",
			gluErrorString(ErrorCheckValue)
		);

		exit(-1);
	}
	GLuint programId = glCreateProgram();

	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);
	
	return programId;
}
