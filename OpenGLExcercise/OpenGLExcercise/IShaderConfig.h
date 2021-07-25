#pragma once
#include "Shader.h"
class IShaderConfig
{
public:
	Shader* shader;
	virtual void applyToShader(std::string uniformName) = 0;
	virtual ~IShaderConfig(){};
};

