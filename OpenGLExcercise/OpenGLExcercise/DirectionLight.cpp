#include "DirectionLight.h"

DirectionLight::DirectionLight(Shader* _shader, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular, glm::vec3 _rotation):
	Light(_shader, _ambient, _diffuse, _specular),
	Revolvable(_rotation)
{
}

void DirectionLight::applyToShader(std::string uniformName)
{
	__super::applyToShader(uniformName);
	shader->setVec3(uniformName + ".direction", direction);
}

