#include "PointLight.h"

PointLight::PointLight(Shader* _shader, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular,glm::vec3 _position, float _constant, float _linear, float _quadratic):
	Light(_shader, _ambient, _diffuse, _specular),
	position(_position),
	constant(_constant),
	linear(_linear),
	quadratic(_quadratic)
{
}

void PointLight::applyToShader(std::string uniformName)
{
	__super::applyToShader(uniformName);
	shader->setVec3(uniformName + ".position", position);
	shader->setFloat(uniformName + ".constant", constant);
	shader->setFloat(uniformName + ".linear", linear);
	shader->setFloat(uniformName + ".quadratic", quadratic);
}
