#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "Core/Logger.h"
#include "Renderer.h"

namespace nc {
	Texture::~Texture() {
		if(this->texture != NULL) {
			glDeleteTextures(1, &texture);
		}
	}

	bool Texture::Create(std::string filename, ...) {
		va_list args;

		va_start(args, filename);

		Renderer& renderer = va_arg(args, Renderer);

		va_end(args);

		return Load(filename, renderer);
	}

	bool Texture::Load(const std::string& filename, Renderer& renderer) {
		int channels = 0;

		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load(filename.c_str(), &(this->size.x), &(this->size.y), &channels, 0);

		if(data == nullptr) {
			WARNING_LOG("Could not create surface: " << filename);
			return false;
		}

		glGenTextures(1, &(this->texture));
		glBindTexture(this->target, this->texture);

		GLenum internalFormat = (channels == 4) ? GL_RGBA8 : GL_RGB8;
		GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

		glTexStorage2D(this->target, 1, internalFormat, this->size.x, this->size.y);
		glTexSubImage2D(this->target, NULL, 0, 0, this->size.x, this->size.y, format, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);

		return true;
	}
}