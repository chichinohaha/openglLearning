#pragma once
#include <string>
enum class CheckType
{
	program,
	shader,
};

class Shader
{
public:
	Shader(const char* vertexFile, const char* fragmentFile);
	std::string vertexString;
	std::string fragmentString;
	const char* vertexSource;
	const char* fragmentSource;
	unsigned int shaderProgram;
public:
	void use();
	void checkCompileError(const unsigned int id, CheckType checkType);


};

