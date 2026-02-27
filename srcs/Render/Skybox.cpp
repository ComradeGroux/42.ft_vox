#include "Skybox.hpp"

#include "GLError.hpp"

Skybox::Skybox(const std::string& vertexPath, const std::string& fragmentPath) : _vao(0), _vbo(0), _ebo(0), _shader(vertexPath, fragmentPath)
{
	float	vertices[8 * 3] = {
		1,  1,  1,	// 0
		1,  1, -1, // 1
		-1,  1, -1, // 2
		-1,  1,  1, // 3

		1, -1,  1, // 4
		1, -1, -1, // 5
		-1, -1, -1,  // 6
		-1, -1,  1 // 7
	};

	uint32_t	indices[6 * 2 * 3] = {
		0, 1, 2,
		0, 2, 3,

		4, 5, 6,
		4, 6, 7,

		3, 2, 6,
		3, 6, 7,

		0, 1, 5,
		0, 5, 4,

		1, 5, 6,
		1, 6, 2,

		0, 3, 4,
		3, 7, 4
	};

	cgl(glGenVertexArrays(1, &_vao));
	cgl(glGenBuffers(1, &_vbo));
	cgl(glGenBuffers(1, &_ebo));

	cgl(glBindVertexArray(_vao));

	cgl(glBindBuffer(GL_ARRAY_BUFFER, _vbo));
	cgl(glBufferData(GL_ARRAY_BUFFER,
					8 * 3 * sizeof(float),
					vertices,
					GL_STATIC_DRAW));

	cgl(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo));
	cgl(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
					6 * 2 * 3 * sizeof(uint32_t),
					indices,
					GL_STATIC_DRAW));

	cgl(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0));
	cgl(glEnableVertexAttribArray(0));

	cgl(glBindVertexArray(0));
}

Skybox::~Skybox(void)
{
	cgl(glDeleteVertexArrays(1, &_vao));
	cgl(glDeleteBuffers(1, &_vbo));
	cgl(glDeleteBuffers(1, &_ebo));
}

void	Skybox::draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& lightDir, const float& lightIntensity) const
{
	cgl(glDisable(GL_CULL_FACE));
	cgl(glDepthFunc(GL_LEQUAL));

	_shader.use();
	_shader.setMat4("uView", view);
	_shader.setMat4("uProjection", projection);
	_shader.setVec3("uLightDir", lightDir);
	_shader.setFloat("uLightIntensity", lightIntensity);

	cgl(glBindVertexArray(_vao));
	cgl(glDrawElements(GL_TRIANGLES, 6 * 2 * 3, GL_UNSIGNED_INT, 0));
	cgl(glBindVertexArray(0));

	cgl(glDepthFunc(GL_LESS));
	cgl(glEnable(GL_CULL_FACE));
}
