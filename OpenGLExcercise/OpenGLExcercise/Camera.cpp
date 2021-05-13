#include "Camera.h"

Camera::Camera(glm::vec3 _position, glm::vec3 _target, glm::vec3 _worldup)
{
	position = _position;
	worldUp = _worldup;
	forward = glm::normalize(_target - position);
	right = glm::normalize( glm::cross(forward, _worldup));
	up = glm::normalize(glm::cross(right, forward));


}

Camera::Camera(glm::vec3 _position, float yaw, float pitch, float roll, glm::vec3 _worldup)
{
	position = _position;
	worldUp = _worldup;
	const float pitchAngle = glm::radians(pitch);
	const float yawAngle = glm::radians(yaw);
	const float rollAngle = glm::radians(roll);
	forward.x = glm::cos(pitchAngle) * glm::cos(yawAngle);
	forward.y = glm::sin(pitchAngle);
	forward.z = glm::cos(pitchAngle) * glm::sin(yawAngle);
	forward = glm::normalize(forward);
	right = glm::normalize(glm::cross(forward, _worldup));
	up = glm::normalize(glm::cross(right, forward));
}

Camera::~Camera()
{
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(position,position+forward,up);
}
