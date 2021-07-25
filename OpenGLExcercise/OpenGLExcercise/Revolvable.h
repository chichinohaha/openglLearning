#pragma once
#include "gtx\rotate_vector.hpp"
class Revolvable
{
public:
	Revolvable(glm::vec3 _rotation);
public:
	glm::vec3 rotation;
	glm::vec3 direction;
	void updateDirection();

};

