#include "Material.h"

Material::Material(Shader* _shader, float _shininess, Shader::ImageSlot _slot, Shader::ImageSlot _specularSlot):
	shader(_shader),
	specular(_specularSlot),
	shininess(_shininess),
	diffuse(_slot)
{
}

void Material::applyToShader(std::string uniformName)
{
	shader->setInt(uniformName + ".specular", specular);
	shader->setInt(uniformName + ".diffuse", diffuse);
	shader->setFloat(uniformName + ".shininess", shininess);
}
