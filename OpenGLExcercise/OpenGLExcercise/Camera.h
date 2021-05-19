#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};
class Camera
{
public:
	Camera(glm::vec3 position, glm::vec3 target , glm::vec3 worldup);
	Camera(glm::vec3 position, float yaw = -90.0f, float pitch = 0.0f, float roll=0.0f, glm::vec3 worldup=glm::vec3(0.0f,1,0.0f));
	~Camera();
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 worldUp;
	float yaw ;
	float pitch ;
	float roll = 0.0f;
	float speed = 2.5f;
	float zoom = 45.0f;

	glm::mat4 getViewMatrix();
private:
	// calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors();
};

