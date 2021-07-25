#pragma once
#include "IShaderConfig.h"
#include "Shader.h"

class Light: public IShaderConfig
{
public:
	Light(Shader* _shader, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular);
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	Shader* shader;
	void applyToShader(std::string uniformName);
};

