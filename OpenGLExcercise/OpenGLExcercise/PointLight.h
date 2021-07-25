#pragma once
#include "Light.h"
class PointLight :
	public Light
{
public:
	glm::vec3 position;
	float constant;
	float linear;
	float quadratic;
	PointLight(Shader* _shader, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular, glm::vec3 _position, float _constant=1.0f, float _linear=0.09f, float _quadratic=0.032f);
	void applyToShader(std::string uniformName);
};

