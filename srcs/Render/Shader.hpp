#pragma once

#include "glad.h"
#include "glm.hpp"

#include <string>

class Shader {
	private:
		GLuint	_program;

		GLuint	_compileShader(const std::string& path, GLenum type) const;
		void	_checkErrors(GLuint id, bool isProgram) const;
		
	public:
		Shader(const std::string& vertexPath, const std::string& fragmentPath);
		~Shader();

		void	use() const;

		void	setInt(const std::string& name, int value) const;
		void	setFloat(const std::string& name, float value) const;
		void	setMat4(const std::string& name, const glm::mat4& value) const;
		void	setVec3(const std::string& name, const glm::vec3& value) const;
};
