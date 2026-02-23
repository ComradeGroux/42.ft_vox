#pragma once

#include "glm.hpp"

class Camera {
	private:
		glm::vec3	_position;
		glm::vec3	_front;
		glm::vec3	_up;
		glm::vec3	_right;

		float	_yaw;
		float	_pitch;

		void	_updateVectors();

	public:
		Camera(glm::vec3 position, float yaw, float pitch);
		~Camera(void);

		glm::mat4	getViewMatrix() const;
		glm::mat4	getProjectionMatrix(float aspectRatio) const;

		void	move(glm::vec3 delta);
		void	rotate(float yawOffset, float pitchOffset);

		glm::vec3	getPosition() const;
		glm::vec3	getFront() const;
		glm::vec3	getRight() const;
};
