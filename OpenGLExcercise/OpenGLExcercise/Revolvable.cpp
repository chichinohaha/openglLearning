#include "Revolvable.h"

Revolvable::Revolvable(glm::vec3 _rotation):
	rotation(_rotation)
{
	updateDirection();
}

void Revolvable::updateDirection()
{
	direction = glm::vec3(0, 0, 1.0f);
	direction = glm::rotateX(direction, glm::radians(rotation.x));
	direction = glm::rotateY(direction, glm::radians(rotation.y));
	direction = glm::rotateZ(direction, glm::radians(rotation.z));
}
