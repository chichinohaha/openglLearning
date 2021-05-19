#include "Camera.h"

Camera::Camera(glm::vec3 _position, glm::vec3 _target, glm::vec3 _worldup)
{
	position = _position;
	worldUp = _worldup;
	front = glm::normalize(_target - position);
	right = glm::normalize( glm::cross(front, _worldup));
	up = glm::normalize(glm::cross(right, front));


}

Camera::Camera(glm::vec3 _position, float _yaw, float _pitch, float _roll, glm::vec3 _worldup)
{
	position = _position;
	worldUp = _worldup;
	yaw = _yaw;
	pitch = _pitch;
	roll = _roll;
	updateCameraVectors();
}

Camera::~Camera()
{
}

glm::mat4 Camera::getViewMatrix()
{
	updateCameraVectors();
	return glm::lookAt(position,position+front,up);
}

void Camera::updateCameraVectors()
{		
	// calculate the new Front vector
	glm::vec3 _front;
	_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	_front.y = sin(glm::radians(pitch));
	_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(_front);
	// also re-calculate the Right and Up vector
	right = glm::normalize(glm::cross(_front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	up = glm::normalize(glm::cross(right, _front));
}
