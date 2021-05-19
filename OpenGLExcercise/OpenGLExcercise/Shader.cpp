#include "Shader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

Shader::Shader(const char* vertexFilePath, const char* fragmentFilePath)
{
	std::ifstream vertexFile;
	std::ifstream fragmentFile;

	vertexFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
	fragmentFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
	try
	{
		std::stringstream vertexStringStream;
		std::stringstream fragmentStringStream;
		vertexFile.open(vertexFilePath);
		fragmentFile.open(fragmentFilePath);
		if (!vertexFile.is_open() || !fragmentFile.is_open()) {
			throw std::exception("open file filed.");
		}
		vertexStringStream << vertexFile.rdbuf();
		fragmentStringStream << fragmentFile.rdbuf();
		vertexString = vertexStringStream.str();
		fragmentString = fragmentStringStream.str();
		vertexSource = vertexString.c_str();
		fragmentSource = fragmentString.c_str();
		unsigned int vertex, fragment;
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexSource, NULL);
		glCompileShader(vertex);
		checkCompileError(vertex, CheckType::shader);

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentSource, NULL);
		glCompileShader(fragment);
		checkCompileError(fragment, CheckType::shader);
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileError(ID, CheckType::program);
		glDeleteShader(fragment);
		glDeleteShader(vertex);
		printf(vertexSource);
		printf(fragmentSource);

	}
	catch (const std::exception& err)
	{
		printf(err.what());
	}

}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::checkCompileError(const unsigned int id, CheckType checkType)
{
	int success;
	char infoLog[512];
	switch (checkType)
	{
	case CheckType::program:
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(id, 512, NULL, infoLog);
			std::cout << "ERROR::program::COMPILATION_FAILED\n" << infoLog << std::endl;
		};
		break;
	case CheckType::shader:
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(id, 512, NULL, infoLog);
			std::cout << "ERROR::shader::COMPILATION_FAILED\n" << infoLog << std::endl;
		};
		break;
	default:
		break;
	}
}
