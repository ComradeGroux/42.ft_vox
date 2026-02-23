#pragma once

#include "Camera.hpp"
#include <GLFW/glfw3.h>

void	mouseCallback(GLFWwindow* window, double xpos, double ypos);

class InputManager {
	private:
		Camera& _camera;
		bool	_firstMouse;
		float	_lastX;
		float	_lastY;

	public:
		InputManager(Camera& camera);
		~InputManager(void);

		void	processKeyboard(GLFWwindow* window, float deltaTime);
		void	processMouse(float offsetX, float offsetY);
		void	setFirstMouse(bool value) { _firstMouse = value; }
};
