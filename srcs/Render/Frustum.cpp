#include "Frustum.hpp"

#include "gtc/matrix_access.hpp"

Frustum::Frustum(void)
{
}

Frustum::~Frustum(void)
{
}

void	Frustum::_extractPlane(int index, const glm::vec4& row)
{
	_planes[index].normal.x = row.x;
	_planes[index].normal.y = row.y;
	_planes[index].normal.z = row.z;
	_planes[index].distance = row.w;

	float	length = glm::length(_planes[index].normal);
	_planes[index].normal   /= length;
	_planes[index].distance /= length;
}

void	Frustum::update(const glm::mat4& view, const glm::mat4& projection) 
{
	glm::mat4	clip = projection * view;

	glm::vec4	row0 = glm::row(clip, 0);
	glm::vec4	row1 = glm::row(clip, 1);
	glm::vec4	row2 = glm::row(clip, 2);
	glm::vec4	row3 = glm::row(clip, 3);

	_extractPlane(0, row3 + row0); // gauche
	_extractPlane(1, row3 - row0); // droite
	_extractPlane(2, row3 + row1); // bas
	_extractPlane(3, row3 - row1); // haut
	_extractPlane(4, row3 + row2); // near
	_extractPlane(5, row3 - row2); // far
}

bool	Frustum::isBoxVisible(const glm::vec3& min, const glm::vec3& max) const
{
	glm::vec3	positive;
	for (int i = 0; i < 6; i++)
	{
		if (_planes[i].normal.x >= 0)
			positive.x = max.x;
		else
			positive.x = min.x;

		if (_planes[i].normal.y >= 0)
			positive.y = max.y;
		else
			positive.y = min.y;

		if (_planes[i].normal.z >= 0)
			positive.z = max.z;
		else
			positive.z = min.z;

		if (glm::dot(_planes[i].normal, positive) + _planes[i].distance < 0.0f)
			return false;
	}
	return true;
}
