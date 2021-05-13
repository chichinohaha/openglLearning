#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
class Camera
{
public:
	Camera(glm::vec3 position, glm::vec3 target , glm::vec3 worldup);
	Camera(glm::vec3 position, float yaw, float pitch, float roll, glm::vec3 worldup);
	~Camera();
	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 worldUp;
	float yaw;
	float pitch;
	float roll;

	glm::mat4 getViewMatrix();
};

