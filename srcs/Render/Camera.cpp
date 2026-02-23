#include "Camera.hpp"
#include "gtc/matrix_transform.hpp"
#include "WorldConstants.hpp"

Camera::Camera(glm::vec3 position, float yaw, float pitch): _position(position), _up(glm::vec3(0.0f, 1.0f, 0.0f)), _yaw(yaw), _pitch(pitch)
{
	_updateVectors();
}

Camera::~Camera(void)
{
}

void	Camera::_updateVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	front.y = sin(glm::radians(_pitch));
	front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));

	_front = glm::normalize(front);
	_right = glm::normalize(glm::cross(_front, glm::vec3(0.0f, 1.0f, 0.0f)));
	_up = glm::normalize(glm::cross(_right, _front));
}

void	Camera::move(glm::vec3 delta)
{
	_position += delta;
}

void	Camera::rotate(float yawOffset, float pitchOffset)
{
	_yaw += yawOffset;
	_pitch += pitchOffset;

	if (_pitch > 89.0f)
		_pitch = 89.0f;
	if (_pitch < -89.0f)
		_pitch = -89.0f;

	_updateVectors();
}

glm::mat4	Camera::getViewMatrix() const
{
	return glm::lookAt(_position, _position + _front, _up);
}

glm::mat4	Camera::getProjectionMatrix(float aspectRatio) const
{
	return glm::perspective(glm::radians(FOV), aspectRatio, NEAR_PLANE, FAR_PLANE);
}

glm::vec3	Camera::getPosition() const
{
	return _position;
}

glm::vec3	Camera::getFront() const
{
	return _front;
}

glm::vec3	Camera::getRight() const
{
	return _right;
}
