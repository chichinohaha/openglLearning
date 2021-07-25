#include "Light.h"

Light::Light(Shader* _shader, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular) :
	shader(_shader),
	ambient(_ambient),
	diffuse(_diffuse),
	specular(_specular)
{
}

void Light::applyToShader(std::string uniformName)
{
	shader->setVec3(uniformName + ".ambient", ambient);
	shader->setVec3(uniformName + ".diffuse", diffuse);
	shader->setVec3(uniformName + ".specular", specular);
}
