#include "InputManager.hpp"

#include "glm.hpp"
#include "WorldConstants.hpp"

void	mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	InputManager*	inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
	if (!inputManager)
		return;

	inputManager->processMouse((float)xpos, (float)ypos);
}

InputManager::InputManager(Camera& camera) : _camera(camera), _firstMouse(true), _lastX(0.0f), _lastY(0.0f)
{
}

InputManager::~InputManager(void)
{
}

void	InputManager::processKeyboard(GLFWwindow* window, float deltaTime)
{
	glm::vec3	movement(0.0f);

	if (glfwGetKey(window, KEY_FORWARD) == GLFW_PRESS)
		movement += _camera.getFront();
	if (glfwGetKey(window, KEY_BACKWARD) == GLFW_PRESS)
		movement -= _camera.getFront();
	if (glfwGetKey(window, KEY_LEFT) == GLFW_PRESS)
		movement -= _camera.getRight();
	if (glfwGetKey(window, KEY_RIGHT) == GLFW_PRESS)
		movement += _camera.getRight();
	if (glfwGetKey(window, KEY_UP) == GLFW_PRESS)
		movement += glm::vec3(0.0f, 1.0f, 0.0f);
	if (glfwGetKey(window, KEY_DOWN) == GLFW_PRESS)
		movement -= glm::vec3(0.0f, 1.0f, 0.0f);
	if (glfwGetKey(window, KEY_QUIT) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glm::length(movement) > 0.0f)
	{
		float speed = PLAYER_SPEED;
		if (glfwGetKey(window, KEY_SPRINT) == GLFW_PRESS)
			speed = PLAYER_RUNNING_SPEED;
		_camera.move(glm::normalize(movement) * speed * deltaTime);
	}
}

void	InputManager::processMouse(float offsetX, float offsetY)
{
	if (_firstMouse)
	{
		_lastX = offsetX;
		_lastY = offsetY;
		_firstMouse = false;
		return;
	}

	float   deltaX = (offsetX - _lastX) * MOUSE_SENSITIVITY;
	float   deltaY = (_lastY  - offsetY) * MOUSE_SENSITIVITY;

	_lastX = offsetX;
	_lastY = offsetY;

	_camera.rotate(deltaX, deltaY);
}
