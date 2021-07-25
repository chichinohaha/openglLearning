#pragma once
#include "IShaderConfig.h"
#include "Shader.h"

class Material: public IShaderConfig
{
public:
	Material(Shader* _shader,float _shininess, Shader::ImageSlot _diffuseSlot,Shader::ImageSlot _specularSlot);
	Shader* shader;
	float shininess;
	Shader::ImageSlot diffuse;
	Shader::ImageSlot specular;
	void applyToShader(std::string uniformName);
};

