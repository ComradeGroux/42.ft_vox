#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "GLError.hpp"
#include "WorldConstants.hpp"

Texture::Texture(const std::string& path)
{
	int width, height, channels;
	unsigned char*	data = stbi_load(path.data(), &width, &height, &channels, 0);
	if (data == NULL)
	{
		std::cerr << "Error: Failed to charge texture: " << path << std::endl;
		return;
	}

	auto chan = channels == 4 ? GL_RGBA : GL_RGB;

	cgl(glGenTextures(1, &_id));
	cgl(glBindTexture(GL_TEXTURE_2D, _id));
	cgl(glTexImage2D(GL_TEXTURE_2D, 0, chan, width, height, 0, chan, GL_UNSIGNED_BYTE, data));
	cgl(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	cgl(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

	cgl(glBindTexture(GL_TEXTURE_2D, 0));
	stbi_image_free(data);
}

Texture::~Texture(void)
{
	cgl(glDeleteTextures(1, &_id));
}

void	Texture::bind(int slot) const
{
	cgl(glActiveTexture(GL_TEXTURE0 + slot));
	cgl(glBindTexture(GL_TEXTURE_2D, _id));
}
