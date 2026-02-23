#pragma once

#include "glad.h"
#include <string>

class Texture {
	private:
		GLuint	_id;
		int		_width;
		int		_height;

	public:
		Texture(const std::string& path);
		~Texture(void);

		void	bind(int slot = 0) const;
};
