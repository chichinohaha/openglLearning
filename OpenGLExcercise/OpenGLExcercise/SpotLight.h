#pragma once
#include "PointLight.h"
#include "Revolvable.h"
class SpotLight :
	public PointLight,
	public Revolvable
{
public:
    float cutOff;
    float outerCutOff;
	SpotLight(Shader* _shader, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular, glm::vec3 _position, glm::vec3 _direction, glm::vec3 _rotation, float _constant, float _linear, float _quadratic, float _cutOff, float _outerCutOff);
	void applyToShader(std::string uniformName);
};

