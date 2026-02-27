#pragma once

#include "Shader.hpp"
#include "glm.hpp"

class Skybox {
	private:
		GLuint	_vao;
		GLuint	_vbo;
		GLuint	_ebo;

		Shader	_shader;
	
	public:
		Skybox(const std::string& vertexPath, const std::string& fragmentPath);
		~Skybox(void);

		void	draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& lightDir, const float& lightIntensity) const;
};
