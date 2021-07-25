#pragma once
#include "Light.h"
#include "gtx\rotate_vector.hpp"
#include "Revolvable.h"
class DirectionLight :
	public Light,
	public Revolvable
{
public:
	DirectionLight(Shader* _shader, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular,glm::vec3 _rotation);
	void applyToShader(std::string uniformName);
};

