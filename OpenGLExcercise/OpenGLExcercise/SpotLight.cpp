#include "SpotLight.h"

SpotLight::SpotLight(Shader* _shader, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular, glm::vec3 _position, glm::vec3 _direction, glm::vec3 _rotation, float _constant, float _linear, float _quadratic, float _cutOff, float _outerCutOff):
	PointLight( _shader,  _ambient, _diffuse,  _specular, _position, _constant,_linear,_quadratic),
	Revolvable(_rotation),
	cutOff(_cutOff),
	outerCutOff(_outerCutOff)
{
}

void SpotLight::applyToShader(std::string uniformName)
{
	__super::applyToShader(uniformName);
	shader->setVec3(uniformName + ".direction", direction);
	shader->setFloat(uniformName + ".cutOff", glm::radians(cutOff));
	shader->setFloat(uniformName + ".outerCutOff", glm::radians(outerCutOff));
}
