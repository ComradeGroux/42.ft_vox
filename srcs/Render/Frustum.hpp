#pragma once

#include "glm.hpp"

struct Plane {
	glm::vec3	normal;
	float		distance;
};

class Frustum {
	private:
		Plane	_planes[6];

		void	_extractPlane(int index, const glm::vec4& row);

	public:
		Frustum(void);
		~Frustum(void);

		void	update(const glm::mat4& view, const glm::mat4& projection);
		bool	isBoxVisible(const glm::vec3& min, const glm::vec3& max) const;
};
