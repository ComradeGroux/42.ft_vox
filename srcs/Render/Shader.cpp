#include "Shader.hpp"
#include "GLError.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
	GLuint vertex	= _compileShader(vertexPath, GL_VERTEX_SHADER);
	GLuint fragment	= _compileShader(fragmentPath, GL_FRAGMENT_SHADER);

	_program = glCreateProgram();
	cgl(glAttachShader(_program, vertex));
	cgl(glAttachShader(_program, fragment));
	cgl(glLinkProgram(_program));
	_checkErrors(_program, true);

	cgl(glDeleteShader(vertex));
	cgl(glDeleteShader(fragment));
}

Shader::~Shader()
{
	glDeleteProgram(_program);
}

void	Shader::use() const
{
	cgl(glUseProgram(_program));
}

void	Shader::setInt(const std::string& name, int value) const
{
	cgl(glUniform1i(glGetUniformLocation(_program, name.c_str()), value));
}

void	Shader::setFloat(const std::string& name, float value) const
{
	cgl(glUniform1f(glGetUniformLocation(_program, name.c_str()), value));
}

void	Shader::setMat4(const std::string& name, const glm::mat4& value) const
{
	cgl(glUniformMatrix4fv(glGetUniformLocation(_program, name.c_str()), 1, GL_FALSE, &value[0][0]));
}

void	Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
	cgl(glUniform3fv(glGetUniformLocation(_program, name.c_str()), 1, &value[0]));
}

GLuint  Shader::_compileShader(const std::string& path, GLenum type) const
{
	std::ifstream	file(path);
	if (!file.is_open())
	{
		std::cerr << "[Shader] Failed to open file: " << path << std::endl;
		return 0;
	}
	std::stringstream	buffer;
	buffer << file.rdbuf();
	std::string	source = buffer.str();
	const char*	src    = source.c_str();

	GLuint	shader = glCreateShader(type);
	cgl(glShaderSource(shader, 1, &src, nullptr));
	cgl(glCompileShader(shader));
	_checkErrors(shader, false);

	return shader;
}

void	Shader::_checkErrors(GLuint id, bool isProgram) const
{
	GLint	success;

	if (isProgram)
	{
		cgl(glGetProgramiv(id, GL_LINK_STATUS, &success));
		if (!success)
		{
			GLint	logLength;
			cgl(glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength));
			std::string log(logLength, '\0');
			cgl(glGetProgramInfoLog(id, logLength, nullptr, &log[0]));
			std::cerr << "[Shader] Linking error:\n" << log << std::endl;
		}
	}
	else
	{
		cgl(glGetShaderiv(id, GL_COMPILE_STATUS, &success));
		if (!success)
		{
			GLint	logLength;
			cgl(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength));
			std::string log(logLength, '\0');
			cgl(glGetShaderInfoLog(id, logLength, nullptr, &log[0]));
			std::cerr << "[Shader] Compilation error:\n" << log << std::endl;
		}
	}
}
